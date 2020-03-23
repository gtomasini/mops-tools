#pragma once
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <chrono>
#include <optional>

#include <Poco/WindowsConsoleChannel.h>
#include <Poco/SimpleFileChannel.h>
#include <Poco/Logger.h>
#include <Poco/AutoPtr.h>

#include "daa_datatypes_icd/trackICD.h"
#include "daa_datatypes_icd/ownship_stateICD.h"

namespace remixerLib{

//auto cmp = [](trackICD::track lhs, trackICD::track rhs) { return lhs.track_time < rhs.track_time; };

struct timeCompare{
	bool operator()(const trackICD::track& lhs, const trackICD::track& rhs)	{
		return lhs.track_time < rhs.track_time;
	}
};

typedef std::set<trackICD::track, timeCompare> trackSetType;

typedef struct googlePoint {
	googlePoint(double la, double lo, double al) :_lat(la), _lon(lo), _alt(al) {}
		double	_lat, _lon,	_alt;
} GOOGLE_POINT;

class mops2track;

typedef std::tuple<std::string, std::string, std::string, std::string> csvFilesTupleType;
typedef std::tuple<unsigned, unsigned, unsigned, unsigned> portsTupleType;

//see track.source_data.xxx
enum class source_data {
	adsb = 0, unfused, fused, camera, acas,
	radar, lidar, acoustic, unknown, ownship
};

//KML Icons, see http://kml4earth.appspot.com/icons.html#shapes
const inline static std::string KML_WHT_STARS_ICON = "http://maps.google.com/mapfiles/kml/paddle/wht-stars.png";
const inline static std::string KML_LTBLU_STARS_ICON = "http://maps.google.com/mapfiles/kml/paddle/ltblu-stars.png";
const inline static std::string KML_RED_STARS_ICON = "http://maps.google.com/mapfiles/kml/paddle/red-stars.png";
const inline static std::string KML_ARROW_ICON = "http://earth.google.com/images/kml-icons/track-directional/track-0.png";

/* KML Colours, the first byte is transparency: i.e. 0xff=0% 0x00=100%
	Red – ff0000ff.
	Yellow – ff00ffff.
	Blue – ffff0000.
	Green – ff00ff00.
	Purple – ff800080.
	Orange – ff0080ff.
	Brown – ff336699.
	Pink – ffff00ff.
*/


//opacity (first byte) at 80% (0x70)
const inline static std::string KML_OPACITY0 = "ff";
const inline static std::string KML_GREEN_COLOR = "00ff00";
const inline static std::string KML_RED_COLOR = "0000ff";
const inline static std::string KML_BLUE_COLOR = "ff0000";
const inline static std::string KML_YELLOW_COLOR = "00ffff";
const inline static std::string KML_ORANGE_COLOR = "1484ff";
const inline static std::string KML_BLACK_COLOR = "000000";
const inline static std::string KML_WHITE_COLOR = "ffffff";


struct Configuration {

	std::string truthCsv, adsbCsv, radarCsv, ownshipCsv;
	csvFilesTupleType csvFilesTuple{ truthCsv, adsbCsv, radarCsv, ownshipCsv };
	unsigned truthPort;
	unsigned adsbPort;
	unsigned radarPort;
	unsigned ownshipPort;
	portsTupleType portsTuple{ truthPort, adsbPort, radarPort, ownshipPort };
	time_t toaOffset = 0;//secs
	source_data src = source_data::unknown;
	std::string	udpServer = "127.0.0.1";
	std::string	callsign = "xxx";
	std::string	loglevel = "notice";
	std::string logFile = "remixer.ini";
	std::string obinfname;//output binary file
	std::string outdir = "out";	//output directory
	std::string alertsDir = "alerts";
	std::string googleDir = "google";
	std::string stage = "XX";

	//KML settings=====================================================
	std::string KMLopacity = KML_OPACITY0;
	std::string KMLownshipColor = KML_GREEN_COLOR;
	std::string KMLownshipAlert1Color = KML_YELLOW_COLOR;
	std::string KMLownshipAlert2Color = KML_ORANGE_COLOR;
	std::string KMLownshipAlert3Color = KML_RED_COLOR;
	std::string KMLownshipAvoidanceColor = KML_WHITE_COLOR;
	std::string KMLintruderColor = KML_BLUE_COLOR;
	std::string KMLownshipIcon = KML_ARROW_ICON;
	std::string KMLintruderIcon = KML_ARROW_ICON;
	std::string KMLlineWidth = "1";
	//=================================================================

	unsigned delay = 0;
	bool noSleep = false;

	unsigned startIndex = 0;
	unsigned minAlt = 1000;//meters, below this alt remixer will do nothing

	inline static unsigned rewindSize = 120;//past positions before alert
	inline static unsigned substageSize = 120;//substage vector size (#positions after alert)
};

//Static Class, I don't remember why :-(
class mops2track {
public:
	static void configure(Configuration const &conf) {
		_ownshipKMLicon			= conf.KMLownshipIcon;
		_intruderKMLicon		= conf.KMLintruderIcon;
		_ownshipKMLopacity		= conf.KMLopacity;
		_lineWidthKML			= conf.KMLlineWidth;
		_ownshipKMLcolor0		= conf.KMLownshipColor;
		_ownshipAvoidanceColor	= conf.KMLownshipAvoidanceColor;
		_ownshipKMLalert1Color	= conf.KMLownshipAlert1Color;
		_ownshipKMLalert2Color	= conf.KMLownshipAlert2Color;
		_ownshipKMLalert3Color	= conf.KMLownshipAlert3Color;
		_intruderKMLcolor		= conf.KMLintruderColor;
		_portsTuple				= conf.portsTuple;
		_udpServerAddress		= conf.udpServer;
		_callsign				= conf.callsign;
		_loglevel				= conf.loglevel;
		_stage					= conf.stage;
		_outdir					= conf.outdir;
		_alertsDir				= conf.alertsDir;
		_googleDir				= conf.googleDir;
		_noSleep				= conf.noSleep;
		_startIndex				= conf.startIndex;
		_minAlt					= conf.minAlt;//meters
		_rewindSize				= conf.rewindSize;//#positions before alert in order to save substage
		_substageSize			= conf.substageSize;//#positions after alert in order to save substage
	}

private:
	//kml update files
	inline static std::string googleOwnshipTrackFile = "google/ownshipTrack.kml";
	inline static std::string googleIntruderTrackFile = "google/intruderTrack.kml";

	typedef std::map<std::string, std::string> ROW;
	//csvFname tuple: truth, adsb, radar, ownship
	//port tupe: truth, adsb, radar, ownship
	
	inline static std::string _udpServerAddress="127.0.0.1";//Udp destination

	inline static std::string _loglevel="notice";
	inline static std::string _stage = "XX";
	inline static std::string _callsign= _stage+"_INTR";
	inline static std::string _outdir = "out";
	inline static std::string _alertsDir = "alerts";
	inline static std::string _googleDir = "google";

	inline static portsTupleType _portsTuple;
	//KML variables
	inline static auto _ownshipKMLopacity = KML_OPACITY0;
	inline static auto _ownshipKMLicon = KML_ARROW_ICON;
	inline static auto _intruderKMLicon = KML_ARROW_ICON;
	inline static auto _ownshipKMLcolor0 = KML_GREEN_COLOR;//initial color without alerts
	inline static auto _ownshipAvoidanceColor = KML_WHITE_COLOR;
	inline static auto _ownshipKMLalert1Color = KML_YELLOW_COLOR;
	inline static auto _ownshipKMLalert2Color = KML_ORANGE_COLOR;
	inline static auto _ownshipKMLalert3Color = KML_RED_COLOR;
	inline static auto _intruderKMLcolor = KML_BLUE_COLOR;
	inline static std::string _lineWidthKML = "1";
	inline static std::string _ownshipKMLdescription = "Ownship, NO alerts";
	inline static bool _noSleep = false;
	inline static HANDLE _hslot = INVALID_HANDLE_VALUE;
	inline static bool _breakFlag=false, _pauseFlag=false;
	inline static uint64_t _breakTime = 0;
	inline static bool _alertReceived	= false;
	inline static bool _alertPresent	= false;
	inline static bool _savingSubstage	= false;
	inline static bool _savingSubstageAlertPresentEnds = false;
	inline static unsigned _substageNum = 0;
	//TODO, put this stuf in config
	inline static unsigned _rewindSize = 60;//past positions before alert
	inline static unsigned _substageSize = 100;//substage vector size
	//TOOD, end
	inline static bool _avoidancePathReceived = false;
	inline static bool _noConsole = true;
	inline static unsigned _breakIndex = 0;
	inline static float _minAlt = 1000;//meters
	inline static unsigned _startIndex = 0;//from this index onwards the simulation works
	inline static std::vector <trackICD::track> _fullTrackVector;//huge(entire) vector which stores all intruder/ownship data from all input sources

#if 0
	inline static
	bool iequals(const std::string& a, const std::string& b){
		//auto n=(std::min)(a.length(), b.length());
		if (b.length() < a.length()) return false;
		auto n = a.length();

		return std::equal(a.begin(), a.begin()+n,
			b.begin(), [](char a, char b) {
				return tolower(a) == tolower(b);
		});
	}
#endif 

	static void
	mapsSource2Flags(unsigned i, trackICD::source &source);

	static auto
	mapsRow(std::map<std::string, std::string> const &i_row, 
		trackICD::track &track,
		ownship_stateICD::aircraft_state &ownshipState,
		source_data src,
		const std::string &callsign,
		const std::string &loglevel="notice",
		bool intruder=true) -> int;

public:
	static std::optional<ownship_stateICD::aircraft_state> 
	intruder2ownship(trackICD::track const &track) noexcept;

	//auto foo() const noexcept override -> int;
	static auto
	processIntruderCsvFile(const std::string &iCsvFname, 
		const std::string &oBinFname = "",
		unsigned delay=1000,
		const std::string &serverAddress = std::string("127.0.0.1"), 
		int port = 2200,
		source_data = source_data::unknown,
		const std::string &callsign = "H2_INTR",
		const std::string &loglevel = "notice") noexcept -> int;

	//generates kml output file from csv
	static auto
	processOwnshipCsvFile(const std::string &iCsvFname,
		const std::string &oKmlfname,
		const std::string &iconHref,
		const std::string &color,
		bool whenTag = true,
		const std::string &date = "2017-01-01",
		const std::string &loglevel = "notice" ) noexcept -> int;

	//generates alert report
	static int
	generateAlertRepot(const std::string &fname);

	static std::string generateKmlDate(time_t offset) {
		char aux[128];
		//Warning, tm_mon € [0, 11]
		struct tm gmt;
		time_t tt = offset;
		_gmtime64_s(&gmt, &tt);
		snprintf(aux, sizeof(aux), "%04d-%02d-%02dT", gmt.tm_year + 1900, gmt.tm_mon + 1, gmt.tm_mday);
		return std::string(aux);
	}

	//generates kml output file from vector
	static auto
	generateKml(const std::vector<ownship_stateICD::aircraft_state> &vec,
			const std::string &oKmlfname,
			const std::string &date, /*=std::string("2017-01-01T")*/ 
			bool whenTag=true) noexcept -> int;

	static auto
	generateKml(const std::vector<trackICD::track> &vec,
		const std::string &oKmlfname,
		const std::string &date, /*=std::string("2017-01-01T")*/
		bool ownship = true, 
		bool whenTag = true) noexcept -> int;

	//generates kml output file from ownship vector
	static auto
	generateCsv(const std::vector<ownship_stateICD::aircraft_state> &vec,
			const std::string &csvfname,
			std::chrono::seconds toaOffset,
			const std::string &loglevel = "notice") noexcept -> int;

	//generates kml output file from intruder vector
	static auto
	generateCsv(const std::vector<trackICD::track> &vec,
			const std::string &csvfname,
			std::chrono::seconds toaOffset,
			const std::string &loglevel = "notice") noexcept -> int;

	static int
	generateKmlTrackFile(
		std::string const &fileName,
		googlePoint const &gpoint,
		std::string const &name,
		std::string const &color,
		std::string const &iconHref, 
		std::string const &desc,
		bool ownship,
		bool drawTrack=true);

	static int
	createKmlTrackFile(
		std::string const &fileName,
		long &pos,
		std::ofstream &oFile,
		std::string const &name,
		std::string const &color,
		std::string const &iconHref,
		std::string const &desc);

	static int
	updateKmlTrackFile(
		std::ofstream &oFile,
		googlePoint const &gpoint,
		long &pos
	);

	static int
	closeKmlTrackFile(std::ofstream &oFile) {
		oFile.close();
		return 0;
	}

	static int
	remixer(const csvFilesTupleType &csvFilesTuple,
			std::chrono::seconds toaOffset, /* 1483228800 = 2017-01-01*/
			const std::string &logFile = "remixer",
			const std::string &oBinFname = "");

	static void alertPush(const trackICD::track &track, const char *str, unsigned i) {
		char aux[256];
		snprintf(aux, sizeof(aux), "\"%s\" mailslot-msg received, track_time=%lld, #track=%d ", str, track.track_time, i);
		_alertVec.push_back(aux);
	};
			

	//returns true if OK
	static bool
	parseCsvFileAndSave(const std::string &iCsvFname,
			std::vector <trackICD::track> &oTrackVec,
			source_data src,
			const std::string &callsign,
			const std::string &loglevel="notice");

	//TODO: define this function
	void writeBuffer(const trackICD::track_buffer &oBuffer);

	//return meters
	static inline float
	NACp2EPU(uint16_t nacp) {
		switch (nacp) {
		default:
		case 0:
		case 1: return 18.52f * 1000;//(10 Nautical Miles)
		case 2: return 7.408f * 1000;//(4 Nautical Miles)
		case 3: return 3.704f * 1000;//(2 Nautical Miles)
		case 4: return 1852.0f;// (1 Nautical Miles)
		case 5: return 926.0f; // (0.5 Nautical Miles)
		case 6: return 555.6f; //(0.3 Nautical Miles)
		case 7: return 185.2f; //(0.1 Nautical Miles)
		case 8: return 92.6f; //(0.05 Nautical Miles)
		case 9: return 30.0f; 
		case 10: return 10.0f;
		case 11: return 3.0f;
		}
	}

	//return meters
	static inline float
	NIC2VPL(uint16_t nic) {
		switch (nic) {
		default:
		case 9: return 112.0f;
		case 10: return 37.5f;
		case 11: return 7.5f;
		}
	}

	//return meters/sec
	static inline float
	NACv2horVelErr(uint16_t nacv) {
		switch (nacv) {
		default:
		case 0: return 100.0f;
		case 1: return 10.0f;
		case 2: return 3.0f;
		case 4: return 0.3f;
		}
	}

	static inline void
		setDefaultValues(trackICD::track &track);

	static inline void
		setDefaultValues(ownship_stateICD::aircraft_state &state);

private:
	static int sendUdpPacket(trackICD::track const &track, bool ownship, 
			std::ofstream &intObinFile, std::ofstream &ownshipObinFile, std::string &src );
	static int readMailSlot(unsigned index, trackICD::track const &track, bool &simulationBegins);
	inline static auto _ownshipKMLcolor = KML_GREEN_COLOR;//curretn ownship color
	static Poco::AutoPtr<Poco::WindowsColorConsoleChannel> _consoleChannel;
	static Poco::AutoPtr<Poco::SimpleFileChannel> _fileChannel;
	static HANDLE makeMailSlot();
	static std::vector<std::string>	_alertVec;
	static std::vector<GOOGLE_POINT> _ownGoogleVec, _intGoogleVec;//save google tracks
	//static Poco::Logger &_logger;
	static inline const std::string getYYYYMMDD_hhmmss() {
		SYSTEMTIME st;
		GetSystemTime(&st);
		char auxs[120];
		sprintf(auxs, "%04d%02d%02d_%02d%02d%02d",
			st.wYear, st.wMonth, st.wDay,
			st.wHour, st.wMinute, st.wSecond);
		return std::string(auxs);
	}

	static inline const auto setOwnshipKMLColor(char alert_level) {
		switch (alert_level) {
		case 0:
			return _ownshipKMLcolor0;

		case 1:
			return _ownshipKMLalert1Color;

		case 2:
			return _ownshipKMLalert2Color;

		case 3:
			return _ownshipKMLalert3Color;

		default:
			return _ownshipKMLcolor;
		}
	}
};

}//namespace



