#include <iostream>
#include <sstream>
#include <iomanip> 
#include <time.h>
#include <chrono>
#include <thread>
#include <functional>
#define _USE_MATH_DEFINES
#include <math.h>
#include <Poco/Logger.h>
#include <Poco/WindowsConsoleChannel.h>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/SocketAddress.h>      

#include "parseCsv.h"
//remixer lib
#include "mops2track.h"
#include "mailslot.h"

using Poco::Logger;
using namespace remixerLib;

//some constant convertions===================================
static const float kts_2_m_per_sec = 1852.0f / 3600;		//OK
static const float ft_per_min_2_m_per_sec = 1.0f/196.85f;	//OK
static const float deg_2_rad = 3.141592653589793f / 180.0f;	//OK	
static const float rad_2_deg = 1 / deg_2_rad;				//OK
static const float nmi_2_m = 1852;							//OK
static const float ft_2_m = 1.0f/3.280839895f;				//OK			
static const uint64_t secs_2_usecs = 1000000;				//OK
static const double usec_2_secs = 1 / secs_2_usecs;			//OK
//============================================================

auto hs_mns_secs = [](int toa) {
	const int hours = toa / 3600;
	const int minutes = (toa - hours * 3600) / 60;
	const int seconds = toa - hours * 3600 - minutes * 60;
	return std::make_tuple(hours, minutes, seconds); };

//stand alone mode (deprecated)
auto
mops2track::processIntruderCsvFile(const std::string &fname, 
	const std::string &ofname,
	unsigned delay,	
	const std::string &server, int port, 
	source_data src_data,
	const std::string &callsign,
	const std::string &loglevel) noexcept -> int {

	auto &logger = Poco::Logger::get("remixer");

	logger.setChannel(_consoleChannel);
	const auto loglevel0 = logger.getLevel();
	logger.setLevel(loglevel);

	logger.trace("========" + std::string(__FUNCTION__) + "========");

	if (fname.empty()) {
		logger.error("CSV file empty name.");
		logger.setLevel(loglevel0);
		return -1;
	}

	parseCsv iCsv(fname);
	if (!iCsv.fileIsOpen()) {
		logger.error("Can't open csv file %s for reading", fname);
		logger.setLevel(loglevel0);
		return -1;
	}

	//daa::net::udp<trackICD::track_buffer> my_udp(server, port);
	Poco::Net::SocketAddress sa(server, port);
	Poco::Net::DatagramSocket	dgs;
	dgs.connect(sa);

	//output binary file
	std::ofstream oFile;

	if (!ofname.empty())
		oFile.open(ofname, std::ios::out | std::ios::binary);

	//source type
	source_data src = src_data;
	if (src_data == source_data::unknown) {
		if (fname.find("ADSB") != std::string::npos)  
			src = source_data::adsb;
		else if (fname.find("RADAR") != std::string::npos) 
			src = source_data::radar;
		else if (fname.find("CAMERA") != std::string::npos) 
			src = source_data::camera;
		else if (fname.find("Truth") != std::string::npos) 
			src = source_data::fused;
	}
	
	logger.trace("processing csv input file %s...", fname);
	iCsv.readHeader();

	auto i = 0;
	for (ROW row; iCsv.readRow(row); i++) {
		//process row and puts data into track
		trackICD::track track;
		ownship_stateICD::aircraft_state ownshipState;
		if (mapsRow(row, track, ownshipState, src, callsign, loglevel) < 0) {
			logger.notice("intruder row #%d has lat/lon zeroed values", i);
			continue;
		}

		//parse track struct to oBuffer
		trackICD::track_buffer oBuffer;
		trackICD::write_track(track, oBuffer);
		
		logger.trace("----->sending track id: %u...", track.track_id);
		
		//my_udp.send(oBuffer);//sending oBuffer packet to udp server
		dgs.sendBytes(reinterpret_cast<const char*>(&oBuffer), sizeof(oBuffer));

		//writing to binary file
		if (oFile.is_open()) oFile.write(reinterpret_cast<const char*>(&oBuffer), sizeof(oBuffer));
		
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	}

	if (oFile.is_open()) oFile.close();
	logger.setLevel(loglevel0);
	return 0;
}

//generates output kml file from ownship csv file
//TODO: uses next function in order to avoid duplicate code
auto
mops2track::processOwnshipCsvFile(const std::string &ifname, 
	const std::string &ofname,
	const std::string &iconHref,
	const std::string &color,
	bool whenTag,
	const std::string &date,
	const std::string &loglevel) noexcept -> int {

	auto &logger = Poco::Logger::get("remixer");
	const auto loglevel0 = logger.getLevel();
	logger.setLevel(loglevel);

	logger.debug("========" + std::string(__FUNCTION__) + "========");

	if (ifname.empty()) {
		logger.error("Input ownship csv file empty name.");
		logger.setLevel(loglevel0);
		return -1;
	}

	parseCsv iCsv(ifname);
	if (!iCsv.fileIsOpen()) {
		logger.error("Can't open input ownship csv file %s for reading", ifname);
		logger.setLevel(loglevel0);
		return -1;
	}

	std::ofstream oFile;	//output kml file
	if (!ofname.empty()) {
		oFile.open(ofname, std::ios::out);
	}
	else {
		logger.setLevel(loglevel);
		return -1;
	}

	logger.trace("processing csv input file %s...", ifname);

	oFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		<< "<kml xmlns = \"http://www.opengis.net/kml/2.2\"\n"
		<< " xmlns:gx = \"http://www.google.com/kml/ext/2.2\">\n"
		<< "<Folder>\n"
		<< "\t<Placemark>\n"
		<< "\t<name>" << ofname << "</name>\n"
		<< "\t<description>" << date << "</description>\n"
	//Style..............................................................
		<< "\t<Style>\n"
	//IconStyle..........................................................
		<< "\t\t<IconStyle>\n"

		<< "\t\t\t<color>" << color << "</color>\n";
	
	//Icon...............................................................
	if (!whenTag)
		oFile << "\t\t\t<Icon/>\n";
	else 
		oFile << "\t\t\t<Icon>"<<iconHref<<"</Icon>\n";

	oFile << "\t\t</IconStyle>\n"

	//LineStyle..............................................................
		<< "\t\t<LineStyle>\n"
		<< "\t\t\t<color>" << color << "</color>\n"
		<< "\t\t\t<gx:labelVisibility>1</gx:labelVisibility>\n"
		<< "\t\t</LineStyle>\n"

	//LabelStyle.............................................................
		<< "\t\t<LabelStyle>\n"
		<< "\t\t\t<color>"<<color<<"</color>\n"
		<< "\t\t\t<colorMode>normal</colorMode>\n"
		<< "\t\t\t<scale>0.5</scale>\n"
		<< "\t\t</LabelStyle>\n"

		<<"\t</Style>\n"

	//Track..................................................................
		<<"\t\t<gx:Track>\n"
		<<"\t\t\t<altitudeMode>absolute</altitudeMode>\n";

	iCsv.readHeader();

	auto i = 0;
	for (ROW row; iCsv.readRow(row);++i) {
		//process row and puts data into track
		trackICD::track track;
		ownship_stateICD::aircraft_state ownshipState;

		auto src= source_data::unknown;
		if (mapsRow(row, track, ownshipState, src, std::string("OWN"), loglevel) < 0) {
			logger.notice("ownship row #%d has lat/lon zeroed values...", i);
			continue;
		}

		logger.debug("writing track id: %u...", track.track_id);

		const int hours = ownshipState.toa/3600;
		const int minutes = (ownshipState.toa - hours*3600)/60;
		const int seconds = ownshipState.toa - hours*3600 - minutes*60;

		//<when>2018-05-28T02:02:09Z</when>
		char buf[16];
		sprintf(buf, "%02d:%02d:%02d", hours, minutes, seconds);

		if (whenTag)
			oFile << "\t\t\t<when>"<<date << buf	<< "Z</when>" << std::endl;
		oFile << std::setprecision(12)
			<< "\t\t\t<gx:coord>" << ownshipState.longitude << " "<< ownshipState.latitude
			<< " " << ownshipState.geometric_altitude << "</gx:coord>" << std::endl;
		
		//<gx:coord>-122.207881 37.371915 1000 < / gx:coord >

		//writing track to kml file
		//oFile.write((char*)&oBuffer, sizeof(oBuffer));
	}

	oFile<<"\t\t\t</gx:Track>\n"
		<<"\t\t</Placemark>\n"
		<<"\t</Folder>\n"
		<<"</kml>\n";

	oFile.close();
	logger.setLevel(loglevel0);
	return 0;
}

//returns true if OK
bool
mops2track::parseCsvFileAndSave(const std::string &iCsvFname,
	std::vector <trackICD::track> &oTrackVec,
	source_data src,
	const std::string &callsign,
	const std::string &loglevel) {

	auto &logger = Poco::Logger::get("remixer");
	const auto loglevel0 = logger.getLevel();
	logger.setLevel(loglevel);

	logger.trace("========" + std::string(__FUNCTION__) + "========");
	if (iCsvFname.empty()) {
		logger.warning("Empty Csv file name(src=%u), no action...", src);
		logger.setLevel(loglevel0);
		return false;
	}

	parseCsv iCsv(iCsvFname);
	if (!iCsv.fileIsOpen()) {
		logger.error("Can't open input csv file %s, no action...", iCsvFname);
		logger.setLevel(loglevel0);
		return false;
	}

	logger.debug("reading csv input file %s...", iCsvFname);
	iCsv.readHeader();
	auto i = 0;
	for (ROW row; iCsv.readRow(row);++i) {//process row and puts data into track
		trackICD::track track;
		ownship_stateICD::aircraft_state dropOwnshipState;//it's not used here!
		if (mapsRow(row, track, dropOwnshipState, src, callsign, loglevel) < 0) {
			logger.notice("%s, row #%d has lat/lon zeroed values....", iCsvFname, i);
			continue;
		}
		//std::cout << "toa: " << track.aircraft_state.toa << std::endl;
		oTrackVec.push_back(track);
	}
	logger.setLevel(loglevel0);
	
	return !oTrackVec.empty();
}

int
mops2track::generateAlertRepot(const std::string &fname)
{
	std::ofstream oFile;	//output kml file
	if (!fname.empty())
		oFile.open(fname, std::ios::out);
	else return -1;

	for (auto x : _alertVec)
		oFile << x << std::endl;

	if (_alertVec.empty())
		oFile << "NO alerts generated at all" << std::endl;

	oFile.close();
	return 0;
}

auto mops2track::generateKml(
	const std::vector<ownship_stateICD::aircraft_state>& vec, 
	const std::string &oKmlfname, 
	const std::string &date,
	bool whenTag)  noexcept -> int
{
	if (vec.empty()) return -1;

	auto &logger = Poco::Logger::get("remixer");
	logger.debug("generating %s...", oKmlfname);
	std::ofstream oFile;	//output kml file
	if (!oKmlfname.empty())
		oFile.open(oKmlfname, std::ios::out);
	else return -1;
	
	oFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		<< "<kml xmlns = \"http://www.opengis.net/kml/2.2\"\n"
		<< " xmlns:gx = \"http://www.google.com/kml/ext/2.2\">\n"
		<< "<Folder>\n"
		<< "\t<Placemark>\n"
		<< "\t\t<name>" << oKmlfname << "</name>\n"
		<< "\t\t<description>" << date << "</description>\n"

	//Style....................................................................
		<< "\t\t<Style>\n"
	//IconStyle................................................................
		<< "\t\t\t<IconStyle>\n"
		<< "\t\t\t\t<color>" << _ownshipKMLopacity+_ownshipKMLcolor << "</color>\n";
	if (whenTag)
		oFile << "\t\t\t\t<Icon>\n"
			<< "\t\t\t\t<href>" << _ownshipKMLicon << "</href>\n"
			<<"\t\t\t\t</Icon>\n";
	else
		oFile << "\t\t\t\t<Icon/>\n";

	oFile << "\t\t\t</IconStyle>\n"

	//LineStyle................................................................
		<< "\t\t\t<LineStyle>\n"
		<< "\t\t\t\t<color>" << _ownshipKMLopacity+_ownshipKMLcolor << "</color>\n"
		<< "\t\t\t\t<colorMode>normal</colorMode>\n"
		<< "\t\t\t\t<scale>0,5</scale>\n"
		<< "\t\t\t</LineStyle>\n"
		<< "\t\t</Style>\n"

	//Track....................................................................
		<< "\t\t<gx:Track>\n"
		<< "\t\t\t<altitudeMode>absolute</altitudeMode>\n";

	const auto toa_offset = vec.at(0).toa;
	for (auto ownshipState: vec) {
		//ownship_stateICD::aircraft_state ownshipState = vec[i];
		const auto toa = ownshipState.toa - toa_offset;

		auto [hours, minutes, seconds] = hs_mns_secs(toa);
		//<when>2018-05-28T02:02:09Z</when>
		//TODO: check this
		char buf[16];
		sprintf(buf, "%02d:%02d:%02d", hours, minutes, seconds);
		if (whenTag)
			oFile << "\t\t\t<when>"<<date << buf << "Z</when>" << std::endl;

		oFile << std::setprecision(12)
			<< "\t\t\t<gx:coord>" << ownshipState.longitude << " " << ownshipState.latitude
			<< " " << ownshipState.geometric_altitude << "</gx:coord>" << std::endl;
	}

	oFile << "\t\t\t</gx:Track>\n"
		<< "\t\t</Placemark>\n"
		<< "\t</Folder>\n"
		<< "</kml>\n";

	oFile.close();
	return 0;
}

auto mops2track::generateKml(
	const std::vector<trackICD::track>& vec,
	const std::string &oKmlfname,
	const std::string &date,
	bool ownship,
	bool whenTag)  noexcept -> int
{
	if (vec.empty()) return -1;

	auto &logger = Poco::Logger::get("remixer");
	logger.debug("generating %s...", oKmlfname);
	std::ofstream oFile;	//output kml file
	if (!oKmlfname.empty())
		oFile.open(oKmlfname, std::ios::out);
	else return -1;

	auto color = _ownshipKMLopacity + _intruderKMLcolor;
	if (ownship) color = _ownshipKMLopacity + _ownshipKMLcolor;
	oFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		<< "<kml xmlns = \"http://www.opengis.net/kml/2.2\"\n"
		<< " xmlns:gx = \"http://www.google.com/kml/ext/2.2\">\n"
		<< "<Folder>\n"
		<< "\t<Placemark>\n"
		<< "\t\t<name>" << oKmlfname << "</name>\n"
		<< "\t\t<description>" << date << "</description>\n"

		//Style....................................................................
		<< "\t\t<Style>\n"
		//IconStyle................................................................
		<< "\t\t\t<IconStyle>\n"
		<< "\t\t\t\t<color>" << color << "</color>\n";
	if (whenTag)
		oFile << "\t\t\t\t<Icon>\n"
		<< "\t\t\t\t<href>" << _intruderKMLicon << "</href>\n"
		<< "\t\t\t\t</Icon>\n";
	else
		oFile << "\t\t\t\t<Icon/>\n";

	oFile << "\t\t\t</IconStyle>\n"

		//LineStyle................................................................
		<< "\t\t\t<LineStyle>\n"
		<< "\t\t\t\t<color>" << color << "</color>\n"
		<< "\t\t\t\t<colorMode>normal</colorMode>\n"
		<< "\t\t\t\t<scale>0,5</scale>\n"
		<< "\t\t\t</LineStyle>\n"
		<< "\t\t</Style>\n"

		//Track....................................................................
		<< "\t\t<gx:Track>\n"
		<< "\t\t\t<altitudeMode>absolute</altitudeMode>\n";

	const auto toa_offset = vec.at(0).aircraft_state.toa;
	for (auto track : vec) {
		//ownship_stateICD::aircraft_state ownshipState = vec[i];
		auto toa = track.aircraft_state.toa - toa_offset;

		auto [hours, minutes, seconds] = hs_mns_secs(toa);

		//<when>2018-05-28T02:02:09Z</when>
		//TODO: check this
		char buf[16];
		sprintf(buf, "%02d:%02d:%02d", hours, minutes, seconds);
		if (whenTag)
			oFile << "\t\t\t<when>" << date << buf << "Z</when>" << std::endl;

		oFile << std::setprecision(12)
			<< "\t\t\t<gx:coord>" << track.aircraft_state.longitude 
			<< " " << track.aircraft_state.latitude
			<< " " << track.aircraft_state.geometric_altitude 
			<< "</gx:coord>" << std::endl;
	}

	oFile << "\t\t\t</gx:Track>\n"
		<< "\t\t</Placemark>\n"
		<< "\t</Folder>\n"
		<< "</kml>\n";

	oFile.close();
	return 0;
}

auto mops2track::generateCsv(
	const std::vector<ownship_stateICD::aircraft_state>& vec,
	const std::string &csvfname,
	std::chrono::seconds toaOffset,
	const std::string &loglevel)  noexcept -> int
{
	auto &logger = Poco::Logger::get("remixer");
	auto loglevel0 = logger.getLevel();
	logger.setLevel(loglevel);

	logger.trace("=======" + std::string(__FUNCTION__) + "========");
	if (vec.empty()) {
		logger.setLevel(loglevel0);
		return -1;
	}
	
	logger.notice("generating %s...", csvfname);
	std::ofstream oFile;	//output csv file
	if (!csvfname.empty())
		oFile.open(csvfname, std::ios::out);
	else {
		logger.setLevel(loglevel0);
		return -1;
	}

	//Header
	oFile << "ToA(s),ICAO,Lat(deg),Lon(deg),Alt(ft),EWV(kts),NSV(kts),VR(ft/min),HDG(deg)" << std::endl;
	oFile << std::setprecision(12);
	const auto toa_offset = vec.at(0).toa;
	for (auto state : vec) 
	{
		oFile << (state.toa - toaOffset.count()) << ",1,"
			<< state.latitude << ","
			<< state.longitude << ","
			//<< state.geometric_altitude/ft_2_m << ","
			//<< state.ground_speed_east/kts_2_m_per_sec << ","
			//<< state.ground_speed_north / kts_2_m_per_sec << ","
			//<< state.geometric_vertical_rate/ft_per_min_2_m_per_sec << ","
			<< state.geometric_altitude<< ","
			<< state.ground_speed_east  << ","
			<< state.ground_speed_north << ","
			<< state.geometric_vertical_rate << ","
			<< state.true_heading 
			<< std::endl;
	}

	oFile.close();
	logger.setLevel(loglevel0);
	return 0;
}

auto
mops2track::generateCsv(const std::vector<trackICD::track> &vec,
	const std::string &csvfname,
	std::chrono::seconds toaOffset,
	const std::string &loglevel) noexcept -> int
{
	auto &logger = Poco::Logger::get("remixer");
	auto loglevel0 = logger.getLevel();
	logger.setLevel(loglevel);

	logger.trace("=======" + std::string(__FUNCTION__) + "========");
	if (vec.empty()) {
		logger.setLevel(loglevel0);
		return -1;
	}

	logger.notice("generating %s...", csvfname);
	std::ofstream oFile;	//output csv file
	if (!csvfname.empty())
		oFile.open(csvfname, std::ios::out);
	else 
	{
		logger.setLevel(loglevel0);
		return -1;
	}

	//Header
	oFile << "ToA(s),ICAO,Lat(deg),Lon(deg),Alt(ft),EWV(kts),NSV(kts),VR(ft/min),HDG(deg)" << std::endl;
	oFile << std::setprecision(12);
	const auto toa_offset = vec.at(0).aircraft_state.toa;
	
	for (auto track : vec) 
	{
		oFile << (track.aircraft_state.toa - toaOffset.count() )<< ",1,"
			<< track.aircraft_state.latitude << ","
			<< track.aircraft_state.longitude << ","
			//<< track.aircraft_state.geometric_altitude / ft_2_m << ","
			//<< track.aircraft_state.ground_speed_east / kts_2_m_per_sec << ","
			//<< track.aircraft_state.ground_speed_north / kts_2_m_per_sec << ","
			//<< track.aircraft_state.geometric_vertical_rate / ft_per_min_2_m_per_sec << ","
			<< track.aircraft_state.geometric_altitude << ","
			<< track.aircraft_state.ground_speed_east << ","
			<< track.aircraft_state.ground_speed_north << ","
			<< track.aircraft_state.geometric_vertical_rate << ","
			<< track.aircraft_state.true_heading
			<< std::endl;
	}

	oFile.close();
	logger.setLevel(loglevel0);
	return 0;
}

int
mops2track::generateKmlTrackFile(
	std::string const &fileName, 
	googlePoint const &gpoint,
	std::string const &name, 
	std::string const &iconHref, 
	std::string const &color,
	std::string const &desc,
	bool ownship,
	bool drawTrack)
{
	std::ofstream oFile;	
	if (!fileName.empty())
		oFile.open(fileName, std::ios::out);
	else 
		return -1;

	//Saves point
	if (ownship) _ownGoogleVec.push_back(gpoint);
	else _intGoogleVec.push_back(gpoint);

	oFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		<< "<kml xmlns = \"http://www.opengis.net/kml/2.2\"\n"
		<< " xmlns:gx = \"http://www.google.com/kml/ext/2.2\">\n"
		<< "<Document>\n"
		<< "\t<Placemark>\n"
		<< "\t\t<name>" << name << "</name>\n"
		<< "\t\t<description>" << desc << "</description>\n"
		//Style....................................................
		<< "\t\t<Style>\n"
		//IConStyle................................................
		<< "\t\t\t<IconStyle>\n"
		<< "\t\t\t\t<color>"<<color<<"</color>\n"
		<< "\t\t\t\t<Icon>\n"
		<< "\t\t\t\t\t<href>" << iconHref << "</href>\n"
		<< "\t\t\t\t</Icon>\n"
		<< "\t\t\t</IconStyle>\n"
		//LineStyle................................................
		<< "\t\t\t<LineStyle>\n"
		<< "\t\t\t\t<color>"<<color<<"</color>\n"
		<< "\t\t\t\t<gx:labelVisibility>1</gx:labelVisibility>\n"
		<< "\t\t\t\t<width>"<<_lineWidthKML<<"</width>\n"
		<< "\t\t\t</LineStyle>\n"
		//LabelStyle...............................................
		<< "\t\t\t<LabelStyle>\n"
		<< "\t\t\t\t<color>" << color << "</color>\n"
		<< "\t\t\t\t\t<colorMode>normal</colorMode>\n"
		<< "\t\t\t\t\t<scale>0.5</scale>\n"
		<< "\t\t\t</LabelStyle>\n"

		<<"\t\t</Style>\n";

	oFile << std::setprecision(12);
	if (drawTrack) {
		//draw a track from beginning until now
		oFile << "\t\t<gx:Track>\n"
				<< "\t\t\t<altitudeMode>absolute</altitudeMode>\n";

		auto trackdraw = [&oFile](googlePoint const &p) {
			oFile << "\t\t\t<gx:coord>"
				<< p._lon << " " << p._lat << " " << p._alt
				<< "</gx:coord>\n";
		};
		if (ownship)
			for (auto x : _ownGoogleVec) trackdraw(x);
		else
			for (auto x : _intGoogleVec) trackdraw(x);

		oFile << "\t\t</gx:Track>\n";
	}
	else//plot just a point
		oFile << "\t\t<Point>\n\t\t\t<coordinates>"
			<< gpoint._lon << "," << gpoint._lat << "," << gpoint._alt
			<< "</coordinates>\n\t\t</Point>\n";

	oFile	<< "\t</Placemark>\n"
		<<"</Document>\n"
		<<"</kml>\n";
		
	oFile.close();

	return 0;
}

int
mops2track::createKmlTrackFile(
	std::string const &fileName,
	long &pos,
	std::ofstream &oFile,
	std::string const &name,
	std::string const &color,
	std::string const &iconHref,
	std::string const &desc)
{
	if (!fileName.empty())
		oFile.open(fileName, std::ios::out);
	else
		return -1;

	oFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		<< "<kml xmlns = \"http://www.opengis.net/kml/2.2\"\n"
		<< " xmlns:gx = \"http://www.google.com/kml/ext/2.2\">\n"
		<< "<Document>\n"
		<< "\t<Placemark>\n"
		<< "\t\t<name>" << name << "</name>\n"
		<< "\t\t<description>" << desc << "</description>\n"
		//Style....................................................
		<< "\t\t<Style>\n"
		//IConStyle................................................
		<< "\t\t\t<IconStyle>\n"
		<< "\t\t\t\t<color>" << color << "</color>\n"
		<< "\t\t\t\t<Icon>\n"
		<< "\t\t\t\t\t<href>" << iconHref << "</href>\n"
		<< "\t\t\t\t</Icon>\n"
		<< "\t\t\t</IconStyle>\n"
		//LineStyle................................................
		<< "\t\t\t<LineStyle>\n"
		<< "\t\t\t\t<color>" << color << "</color>\n"
		<< "\t\t\t\t<gx:labelVisibility>1</gx:labelVisibility>\n"
		<< "\t\t\t\t<width>" << _lineWidthKML << "</width>\n"
		<< "\t\t\t</LineStyle>\n"
		//LabelStyle...............................................
		<< "\t\t\t<LabelStyle>\n"
		<< "\t\t\t\t<color>" << color << "</color>\n"
		<< "\t\t\t\t\t<colorMode>normal</colorMode>\n"
		<< "\t\t\t\t\t<scale>0.5</scale>\n"
		<< "\t\t\t</LabelStyle>\n"

		<< "\t\t</Style>\n";

	oFile << "\t\t<gx:Track>\n"
		<< "\t\t\t<altitudeMode>absolute</altitudeMode>\n";

	pos = oFile.tellp();
	oFile << "\t\t</gx:Track>\n"
		<< "\t</Placemark>\n"
		<< "</Document>\n"
		<< "</kml>\n";
	oFile.flush();

	return 0;
}

int
mops2track::updateKmlTrackFile(
	std::ofstream &oFile,
	googlePoint const &gpoint,
	long &pos)
{
	if (oFile.is_open() == false) return -1;

	if (pos != 0)  oFile.seekp(pos);
	oFile << std::setprecision(10);
	oFile << "\t\t\t<gx:coord>"
		<< gpoint._lon << " " << gpoint._lat << " " << gpoint._alt
		<< "</gx:coord>\n";

	pos = oFile.tellp();
	oFile<< "\t\t</gx:Track>\n"
		<< "\t</Placemark>\n"
		<< "</Document>\n"
		<< "</kml>\n";

	oFile.flush();
	return 0;
}

void
mops2track::mapsSource2Flags(unsigned in, trackICD::source &source) {
	source.UNFUSED_TRACK = false;
	source.FUSED_TRACK = false;
	source.EO_IR_CAMERA_TRACK = false;
	source.ADSB_TRACK = false;
	source.ACAS_TRACK = false;
	source.RADAR_TRACK = false;
	source.LIDAR = false;
	source.ACOUSTIC = false;
	source.UNKNOWN_SOURCE = false;
	
	switch (in) {
	case 0:	source.UNFUSED_TRACK = true;		break;
	case 1: source.FUSED_TRACK = true;			break;
	case 2: source.EO_IR_CAMERA_TRACK = true; 	break;
	case 3: source.ADSB_TRACK = true; 			break;
	case 4: source.ACAS_TRACK = true;    		break;
	case 5: source.RADAR_TRACK = true;			break;
	case 6: source.LIDAR = true;				break;
	case 7: source.ACOUSTIC = true;				break;
	case 8:
	default:source.UNKNOWN_SOURCE = true;	  	break;
	}
}

//deprecated
template <typename T>
inline void assignMag(std::string &str, bool &state_flag, T &value){
	std::istringstream iss(str);
	iss >> value;
	//value = aux;
	state_flag = true;
}

auto 
mops2track::mapsRow(std::map<std::string, std::string> const &i_row, 
				trackICD::track &track,
				ownship_stateICD::aircraft_state &ownshipState,
				source_data src, 
				const std::string &callsign,
				const std::string &loglevel,
				bool intruder) -> int {
	auto &logger = Poco::Logger::get("remixer");
	auto loglevel0 = logger.getLevel();
	logger.setLevel(loglevel);
	
	setDefaultValues(track);
	setDefaultValues(ownshipState);

	//callsign set, intruder 
	strncpy(track.callsign, callsign.c_str(), sizeof(track.callsign));

	//track id, intruder
	std::string aux = callsign;
	track.track_id = std::hash<std::string>{}(aux);

	track.aircraft_category = trackICD::HEAVY;//because traffic simulator sends this
	track.aircraft_model = trackICD::B777;//the same

	//Source and track id, just for intruder
	std::string source = "unknown source";
	if (src == source_data::adsb) {
		track.source_data.ADSB_TRACK = true;
		track.track_id += 1;
		track.aircraft_model = trackICD::airframe::B777;
		source = "adsb track";
	}
	else if (src == source_data::unfused) {
		track.source_data.UNFUSED_TRACK = true;
		track.track_id += 2;
		source = "unfused track";
	}
	else if (src == source_data::fused) {
		track.source_data.FUSED_TRACK = true;
		track.track_id += 3;
		source = "fused track";
	}
	else if (src == source_data::camera) {
		track.source_data.EO_IR_CAMERA_TRACK = true;
		track.track_id += 4;
		source = "eo ir camera";
	}
	else if (src == source_data::acas) {
		track.source_data.ACAS_TRACK = true;
		track.track_id += 5;
		source = "acas track";
	}
	else if (src== source_data::radar){
		track.source_data.RADAR_TRACK = true;
		track.track_id += 6;
		source = "radar track";
	}
	else if (src == source_data::lidar) {
		track.source_data.LIDAR = true;
		track.track_id += 7;
		source = "lidar";
	}
	else if (src == source_data::acoustic) {
		track.source_data.ACOUSTIC = true;
		track.track_id += 8;
		source = "acoustic";
	}
	else {
		track.source_data.UNKNOWN_SOURCE = true;
	}
	
	//auxiliary vars for futher Radar calculations below
	float rea=0, rng=0, rr=0, eaa=0, sra=0, rra=0;
	//auxilary vars for ADSB
	uint16_t nacp, nic, nacv;

	int k(0);//mmapping no matches number
	bool relative_altitude=false;//when exists RA(ft) we could read from csv

	double gndSpd = 0;//auxiliar value for aurora csv (output)
	//walk the hole map row: key -> value
	for (auto x : i_row)
	{
		std::string key(x.first), strVal(x.second);
#ifdef DEBUG
		std::cout << "KEY: " << key << ", VALUE:" << strVal << std::endl;
#endif
		const char *pkey = key.c_str();

		//converts string str to a value type
		auto setValue=[](auto const &str, auto &state_flag, auto &value) {
			state_flag = true;
			value = 0;
			if (str.empty() || !str.empty() && isblank(str.at(0))) return;
			std::istringstream iss(str);
			iss >> value;
		};

		auto iequals = [](const std::string& a, const std::string& b) {
			//auto n=(std::min)(a.length(), b.length());
			if (b.length() < a.length()) return false;
			auto n = a.length();

			return std::equal(a.begin(), a.begin() + n,
				b.begin(), [](char a, char b) {
				return tolower(a) == tolower(b);
			});
		};

		if ( !key.compare("t")){//trj csv
			setValue(strVal, track.aircraft_state.available_states.TOA, track.aircraft_state.toa);
			//ownship
			ownshipState.available_states.TOA = true;
			ownshipState.toa = track.aircraft_state.toa;
		}
		else if ( !key.compare("H") ) {//trj csv
			setValue(strVal, track.aircraft_state.available_states.GEOMETRIC_ALTITUDE, track.aircraft_state.geometric_altitude);
			track.aircraft_state.barometric_altitude = track.aircraft_state.geometric_altitude;
			track.aircraft_state.available_states.BAROMETRIC_ALTITUDE = true;
			ownshipState.geometric_altitude = track.aircraft_state.geometric_altitude;
			ownshipState.available_states.GEOMETRIC_ALTITUDE = true;
			ownshipState.barometric_altitude = ownshipState.geometric_altitude;
			ownshipState.available_states.BAROMETRIC_ALTITUDE = true;
#ifdef DEBUG
			//logger.trace("found \'Hh\', altitude to " + track.aircraft_state.geometric_altitude);
			std::cout << "H from trj (ownship) =" << ownshipState.geometric_altitude << std::endl;
			std::cout << "H from trj (track) =" << track.aircraft_state.geometric_altitude << std::endl;
#endif

		}
		else if (std::istringstream iss(strVal); iequals("SRC", key)){
			//SRC: source, overwrites SRC field (see above)
			unsigned ui; iss >> ui;
			if (track.source_data.UNKNOWN_SOURCE)
				mapsSource2Flags( ui, track.source_data);
		}
		else if (iequals("TID", key)) {
			uint32_t tid; iss >> tid;
			if (tid != 1) track.track_id = tid;
		}  
		else if (iequals("ToR", key)) {//ADSB, RADAR
			//Time of Report(sec)
			double t_aux; iss >> t_aux;//secs
			track.track_time = t_aux;//secs
		}
		else if ( iequals("ToA", key) ) {//ADSB, RADAR, Truth
			//Time of applicability(sec)
			//track.track_time = track.aircraft_state.toa
			setValue(strVal, track.aircraft_state.available_states.TOA, track.aircraft_state.toa);
			//ownship
			ownshipState.available_states.TOA = true;
			ownshipState.toa = track.aircraft_state.toa;
		}
		else if ( iequals("Lat", key) && !iequals("LatA", key) ) {//ADSB, RADAR
			//latitude(deg)
			if (strVal == "0") {
				std::cerr << "ignoring key:" << key << ", strVal:" << strVal << std::endl;
				return -1;
			}
			setValue(strVal, track.aircraft_state.available_states.LATITUDE, track.aircraft_state.latitude);
			if (abs(track.aircraft_state.latitude) < 0.0001) {
				std::cerr << "ignoring key:" << key << ", strVal:" << strVal << std::endl;
				return -1;
			}
			//ownship
			ownshipState.available_states.LATITUDE = true;
			ownshipState.latitude = track.aircraft_state.latitude;
		}
		else if ( iequals("Lon", key) ) {//ADSB, RADAR
			//longitude(deg)
			if (strVal == "0") {
				std::cerr << "ignoring key:" << key << ", strVal:" << strVal << std::endl;
				return -2;
			}
			setValue(strVal, track.aircraft_state.available_states.LONGITUDE, track.aircraft_state.longitude);
			if (abs(track.aircraft_state.longitude) < 0.0001) {
				std::cerr << "ignoring key:" << key << ", strVal:" << strVal << std::endl;
				return -2;
			}
			//ownship
			ownshipState.longitude = track.aircraft_state.longitude;
			ownshipState.available_states.LONGITUDE = true;
		}
		else if ( iequals("Alt", key) ) {//ADSB, RADAR
			//altitude(ft)
			setValue(strVal, track.aircraft_state.available_states.GEOMETRIC_ALTITUDE, track.aircraft_state.geometric_altitude);
			track.aircraft_state.geometric_altitude *= ft_2_m;//m
			track.aircraft_state.barometric_altitude = track.aircraft_state.geometric_altitude;
			track.aircraft_state.available_states.BAROMETRIC_ALTITUDE = true;
			//ownship
			ownshipState.geometric_altitude = track.aircraft_state.geometric_altitude;
			ownshipState.available_states.GEOMETRIC_ALTITUDE = true;
			ownshipState.barometric_altitude = track.aircraft_state.barometric_altitude;
			ownshipState.available_states.BAROMETRIC_ALTITUDE = true;
#ifdef DEBUG
			//logger.trace("found \'Hh\', altitude to " + track.aircraft_state.geometric_altitude);
			std::cout << "Alt from csv (ownship) =" << ownshipState.geometric_altitude << std::endl;
			std::cout << "Alt from csv (track) =" << track.aircraft_state.geometric_altitude << std::endl;
#endif
		}
		else if ( iequals("EWV", key) ) {//ADSB
			//East West Velocity(kts)
			setValue(strVal, track.aircraft_state.available_states.GROUND_SPEED_EAST, track.aircraft_state.ground_speed_east);
			track.aircraft_state.ground_speed_east *= kts_2_m_per_sec;//m/sec
			//ownship
			ownshipState.ground_speed_east = track.aircraft_state.ground_speed_east;
			ownshipState.available_states.GROUND_SPEED_EAST = true;
		}
		else if ( iequals("NSV", key) ) {//ADSB
			//North South Velocity(kts)
			setValue(strVal, track.aircraft_state.available_states.GROUND_SPEED_NORTH, track.aircraft_state.ground_speed_north);
			track.aircraft_state.ground_speed_north *= kts_2_m_per_sec;//m/sec;
			//ownship
			ownshipState.ground_speed_north = track.aircraft_state.ground_speed_north;
			ownshipState.available_states.GROUND_SPEED_NORTH = true;
		}
		else if ( iequals("VR(", key) || iequals("VS(", key) ) {//ADSB
			//vertical rate(ft/min) or VS(vertical speed)
			setValue(strVal, track.aircraft_state.available_states.GEOMETRIC_VERTICAL_RATE, track.aircraft_state.geometric_vertical_rate);
			//track.aircraft_state.geometric_vertical_rate *= ft_per_min_2_m_per_sec;//m per sec
			track.aircraft_state.pressure_vertical_rate = track.aircraft_state.geometric_vertical_rate;
			track.aircraft_state.available_states.PRESSURE_VERTICAL_RATE = true;
			//ownship
			ownshipState.geometric_vertical_rate = track.aircraft_state.geometric_vertical_rate;
			ownshipState.pressure_vertical_rate = track.aircraft_state.pressure_vertical_rate;
			ownshipState.available_states.GEOMETRIC_VERTICAL_RATE = true;
			ownshipState.available_states.PRESSURE_VERTICAL_RATE = true;
		}
		else if ( iequals("HDG", key) ) {//RADAR: C1_RADAR_NONE_Tracker_TVInt1
			//aircraft_state_t.true_heading
			assignMag(strVal, track.aircraft_state.available_states.TRUE_HEADING, track.aircraft_state.true_heading);
			track.aircraft_state.magnetic_heading = track.aircraft_state.true_heading;
			track.aircraft_state.available_states.MAGNETIC_HEADING = true;
			//ownship
			ownshipState.true_heading = track.aircraft_state.true_heading;
			ownshipState.available_states.TRUE_HEADING = true;
			ownshipState.magnetic_heading = track.aircraft_state.magnetic_heading;
			ownshipState.available_states.MAGNETIC_HEADING = true;
		}
		else if (iequals("RA(", key)) {//RADAR: C1_RADAR_NONE_Tracker_TVInt1
			//RA(ft): Relative altitude
			iss >> track.track_state.relative_altitude;//ft
			track.track_state.relative_altitude *= ft_2_m;
			relative_altitude = true;
#ifdef DEBUG
			//logger.trace("found \'Hh\', altitude to " + track.aircraft_state.geometric_altitude);
			std::cout << "relative altitude from csv:" << track.track_state.relative_altitude  << std::endl;
#endif
		}
		else if (iequals("BRG", key)) {//RADAR
			//BRG(deg) : relative bearing(deg)
			iss >> track.track_state.relative_bearing;
			track.track_state.bearing_invalid = true;
		}

		//TODO: check uncertainties...
		else if ( iequals("APA", key) ) {//RADAR track
			//(APA) absolute position accuracy (nmi)
			iss >> track.aircraft_state.horizonal_position_uncertainty;
			track.aircraft_state.horizonal_position_uncertainty *= nmi_2_m;//m
		}
		else if ( iequals("PAA", key) ) {//RADAR track
			//PAA(ft): presure altitude accuracy
			iss >> track.aircraft_state.pressure_altitude_uncertainty;
			track.aircraft_state.pressure_altitude_uncertainty *= ft_2_m;
		}
		else if ( iequals("GSA", key) ) {//RADAR track
			//GSA(kts): ground speed accuracy
			iss >> track.aircraft_state.ground_speed_uncertainty;
			track.aircraft_state.ground_speed_uncertainty *= kts_2_m_per_sec;
		}
		else if ( iequals("HDGA", key) ) {//RADAR track
			//HDGA(deg): heading track accuracy 
			iss >> track.aircraft_state.heading_uncertainty;
		}
		else if ( iequals( "RBA", key )) {//RADAR
			//RBA(deg): Relative Bearing Accuracy, RBA
			iss >> track.track_state.relative_bearing_uncertainty;
		}
		//end uncertainties

		//RADAR: some auxilary	values for futher calculations
		else if (iequals("REA", key)) {
			//REA(deg): Relative Elevation Angle
			iss >> rea;//deg
			rea *= deg_2_rad;//radians
		}
		else if (iequals("RNG", key)) {
			//RNG: slant range
			iss >> rng;	//nmi
			rng *= nmi_2_m; //m
		}
		else if (iequals("RR(", key)) {//RADAR
			//RR: range rate
			iss >> rr;//kts
			rr *= kts_2_m_per_sec;//m per sec
		}
		else if (iequals("EAA", key)) {//RADAR
			//EAA(deg) : Elevation Angle Accuracy, not used
			iss >> eaa;//deg
			eaa *= deg_2_rad;//radians
		}
		else if (iequals("SRA", key)) {//RADAR
			//SRA(nmi) : Slant Range Accuracy
			iss >> sra;//nmi
			sra *= nmi_2_m;//m
			//std::cout << "SRA(m): " << sra << std::endl;
		}
		else if (iequals("RRA", key)) {//RADAR
			//RRA(knots) : Range Rate Accuracy
			iss >> rra;
			rra *= kts_2_m_per_sec;//m/sec
		}
		else if (iequals("NACp", key)) {//ADSB
			//Navigation Accuracy Category for Position
			iss >> nacp;
			track.aircraft_state.horizonal_position_uncertainty = NACp2EPU(nacp);
		}
		else if (iequals("NIC", key)) {
			iss >> nic;
			track.aircraft_state.pressure_altitude_uncertainty = NIC2VPL(nic);
			track.aircraft_state.geometric_altitude_uncertainty = NIC2VPL(nic);
		}
		else if (iequals("NACv", key)) {
			iss >> nacv;
			track.aircraft_state.ground_speed_uncertainty = NACv2horVelErr(nacv);
		}
		//trj csv file generated by tp
		else if (!key.compare("lambda")) {
			iss >> ownshipState.longitude;
			ownshipState.longitude *= (180 / M_PI);
			if (ownshipState.longitude >= 180) 	ownshipState.longitude -= 360;
			ownshipState.available_states.LONGITUDE = true;
			track.aircraft_state.available_states.LONGITUDE = true;
			track.aircraft_state.longitude = ownshipState.longitude;
		}
		else if (!key.compare("phi")) {
			iss >> ownshipState.latitude;
			ownshipState.latitude *= (180 / M_PI);
			ownshipState.available_states.LATITUDE = true;
			track.aircraft_state.available_states.LATITUDE = true;
			track.aircraft_state.latitude = ownshipState.latitude;
		}
		//NSv:: wind_llsi
		else if (!key.compare("wind_llsi")) {
			iss >> ownshipState.ground_speed_north;
			ownshipState.available_states.GROUND_SPEED_NORTH = true;
			track.aircraft_state.available_states.GROUND_SPEED_NORTH = true;
			track.aircraft_state.ground_speed_north = ownshipState.ground_speed_north;
		}
		//EWv: wind_llsii
		else if (!key.compare("wind_llsii")) {
			iss >> ownshipState.ground_speed_east;
			ownshipState.available_states.GROUND_SPEED_EAST = true;
			ownshipState.ground_speed_east = ownshipState.ground_speed_east;
			track.aircraft_state.available_states.GROUND_SPEED_EAST = true;
			track.aircraft_state.ground_speed_east = ownshipState.ground_speed_east;
		}
		//VR: wind_llsiii
		else if (!key.compare("wind_llsiii")) {
			iss >> ownshipState.geometric_vertical_rate;
			ownshipState.available_states.GEOMETRIC_VERTICAL_RATE = true;
			track.aircraft_state.available_states.GEOMETRIC_VERTICAL_RATE = true;
			track.aircraft_state.geometric_vertical_rate = ownshipState.geometric_vertical_rate;
		}
		//chiwind
		else if (!key.compare("chiwind")) {
			iss >> ownshipState.heading_rate;
			ownshipState.available_states.HEADING_RATE = true;
			track.aircraft_state.heading_rate = ownshipState.heading_rate;
			track.aircraft_state.available_states.HEADING_RATE = true;
		}
		else if (!key.compare("GndSpd")) //aurora
		{
			iss >> gndSpd;//kt
		}
		else if ( iequals("VSpd", key) ) //aurora
		{
		   //vertical rate(ft/min) or VS(vertical speed)
		   setValue(strVal, track.aircraft_state.available_states.GEOMETRIC_VERTICAL_RATE, track.aircraft_state.geometric_vertical_rate);
		   track.aircraft_state.pressure_vertical_rate = track.aircraft_state.geometric_vertical_rate;
		   track.aircraft_state.available_states.PRESSURE_VERTICAL_RATE = true;
		   //ownship
		   ownshipState.geometric_vertical_rate = track.aircraft_state.geometric_vertical_rate;
		   ownshipState.pressure_vertical_rate = track.aircraft_state.pressure_vertical_rate;
		   ownshipState.available_states.GEOMETRIC_VERTICAL_RATE = true;
		   ownshipState.available_states.PRESSURE_VERTICAL_RATE = true;
		}
		else {
			//key not mapped yet...
#ifdef DEBUG
			logger.trace(key+" :\tNOT MAPPED. source:"+source);
#endif
			k++;
		}
	}//for(;;)

	{	//RADAR
		//relative_range, relative_altitude, relative_ground_speedn (radar) CALCULATION
		//using RNG, REA && RR 

		track.track_state.relative_range = rng * cos(rea);
		if (!relative_altitude)
			track.track_state.relative_altitude = track.track_state.relative_range*tan(rea);
				
		track.track_state.relative_range_uncertainty = round(abs(sra * cos(rea))+0.50);
		track.track_state.relative_altitude_uncertainty = round(abs(sra * sin(rea))+0.50);
		
		//std::cout << "track.track_state.relative_range_uncertainty:" << track.track_state.relative_range_uncertainty << std::endl;
		//std::cout << "track_state.relative_altitude_uncertainty:" << track.track_state.relative_altitude_uncertainty << std::endl;

		track.track_state.relative_ground_speed_north = rr*sin(track.track_state.relative_bearing*deg_2_rad);
		track.track_state.relative_ground_speed_east = rr*cos(track.track_state.relative_bearing*deg_2_rad);
		//ground track angle: arctg(EWV/NSV) or arct(ground_speed_north/ground_speed_east)
		track.aircraft_state.available_states.GROUND_TRACK_ANGLE = true;
		if (track.aircraft_state.ground_speed_east == 0)
			track.aircraft_state.ground_track_angle = 0;
		else
			track.aircraft_state.ground_track_angle = atan(track.aircraft_state.ground_speed_east / track.aircraft_state.ground_speed_north);
		track.aircraft_state.ground_track_angle *= rad_2_deg;//rad
	}
	
	{	//track_time: if TOR not availabe ===> track_time := TOA (secs)
		if (0 == track.track_time && track.aircraft_state.available_states.TOA)
			track.track_time = track.aircraft_state.toa;//secs
	}

	auto  hdg = track.aircraft_state.true_heading * deg_2_rad;
	//std::cout << "toa: "<< track.aircraft_state.toa<<", hdg: " << hdg<<", gndSpd:"<<gndSpd;
	if (!track.aircraft_state.available_states.GROUND_SPEED_NORTH) {
		track.aircraft_state.ground_speed_north = gndSpd * cos(hdg);
		track.aircraft_state.available_states.GROUND_SPEED_NORTH = true;
		//std::cout << ", ground speed north: " << track.aircraft_state.ground_speed_north;
	}
	if (!track.aircraft_state.available_states.GROUND_SPEED_EAST){
		track.aircraft_state.ground_speed_east = gndSpd * sin(hdg);
		track.aircraft_state.available_states.GROUND_SPEED_EAST = true;
		//std::cout << ", ground speed east: " << track.aircraft_state.ground_speed_east;
	}
	//std::cout << std::endl;
	
	logger.debug("=======>track_time("+ source+"):\t%ld (msecs)",  (long)track.track_time*1000);
	logger.setLevel(loglevel0);
	return i_row.size() - k;//number matches
}

void
mops2track::setDefaultValues(trackICD::track &track) {
	//default values for some things
	memset(&track, sizeof(track), 0);

	track.source_data.UNFUSED_TRACK = false;
	track.source_data.FUSED_TRACK = false;
	track.source_data.EO_IR_CAMERA_TRACK = false;
	track.source_data.ADSB_TRACK = false;
	track.source_data.ACAS_TRACK = false;
	track.source_data.RADAR_TRACK = false;
	track.source_data.LIDAR = false;
	track.source_data.ACOUSTIC = false;
	track.source_data.UNKNOWN_SOURCE = false;

	track.aircraft_state.available_states.TOA = false;    // bit at 0 position
	track.aircraft_state.available_states.LONGITUDE = false;    // bit at 1 position
	track.aircraft_state.available_states.LATITUDE = false;    // bit at 2 position
	track.aircraft_state.available_states.GEOMETRIC_ALTITUDE = false;    // bit at 3 position
	track.aircraft_state.available_states.BAROMETRIC_ALTITUDE = false;    // bit at 4 position
	track.aircraft_state.available_states.TRUE_HEADING = false;    // bit at 5 position
	track.aircraft_state.available_states.MAGNETIC_HEADING = false;    // bit at 6 position
	track.aircraft_state.available_states.GROUND_TRACK_ANGLE = false;    // bit at 7 position
	track.aircraft_state.available_states.TRUE_AIRSPEED = false;    // bit at 8 position
	track.aircraft_state.available_states.INDICATED_AISPEED = false;    // bit at 9 position
	track.aircraft_state.available_states.GROUND_SPEED_NORTH = false;    // bit at 10 position
	track.aircraft_state.available_states.GROUND_SPEED_EAST = false;    // bit at 11 position
	track.aircraft_state.available_states.GEOMETRIC_VERTICAL_RATE = false;    // bit at 12 position
	track.aircraft_state.available_states.PRESSURE_VERTICAL_RATE = false;    // bit at 13 position
	track.aircraft_state.available_states.HEADING_RATE = false;    // bit at 14 position
	track.aircraft_state.available_states.GROUND_TRACK_RATE = false;    // NO, bit at 15 position

	track.aircraft_category = trackICD::aircraft_category_t::NA;
	track.aircraft_model = trackICD::airframe::UNKNOWN_AIRFRAME;
	track.aircraft_state.mode = trackICD::flight_mode::CRUISE;
	track.aircraft_state.available_states.MODE = true;

	track.track_state.track_size = trackICD::track_size_t::UNKNOWN_TRACK_SIZE;
	track.track_state.bearing_invalid = true;

	track.track_time = 0;	//secs

	track.track_state.relative_bearing = 0;
	track.track_state.relative_range = 0;
	track.track_state.relative_altitude = 0;
	track.track_state.relative_ground_speed_north = 0;
	track.track_state.relative_ground_speed_east = 0;

	track.aircraft_state.horizonal_position_uncertainty = 0;
	track.aircraft_state.geometric_altitude_uncertainty = 0;
	track.aircraft_state.pressure_altitude_uncertainty = 0;
	track.aircraft_state.ground_speed_uncertainty = 0;
	track.aircraft_state.heading_uncertainty = 0;

	track.track_state.relative_range = 0;//Relative State variables as detailed in MOPS (section 2.2.3.2.2)
	track.track_state.relative_bearing = 0;
	track.track_state.relative_ground_speed_north = 0;
	track.track_state.relative_ground_speed_east = 0;
	track.track_state.relative_altitude = 0;
	track.track_state.relative_vertical_speed = 0;

	track.track_state.relative_range_uncertainty = 0;
	track.track_state.relative_bearing_uncertainty = 0;
	track.track_state.relative_ground_speed_north_uncertainty = 0;
	track.track_state.relative_ground_speed_east_uncertainty = 0;
	track.track_state.relative_altitude_uncertainty = 0;
	track.track_state.relative_vertical_speed_uncertainty = 0;

	//callsign="-End."
	track.callsign[0] = '-';
	track.callsign[1] = 'E';
	track.callsign[2] = 'n';
	track.callsign[3] = 'd';
	track.callsign[4] = '.';
	track.callsign[5] = 0;
}

void
mops2track::setDefaultValues(ownship_stateICD::aircraft_state &state) {
	//default values for some things
	memset(&state, sizeof(state), 0);

	//Bit mask of available states
	state.available_states.TOA		 = false;
	state.available_states.LONGITUDE = false;
	state.available_states.LATITUDE  = false;
	state.available_states.GEOMETRIC_ALTITUDE  = false;
	state.available_states.BAROMETRIC_ALTITUDE = false;
	state.available_states.TRUE_HEADING = false;
	state.available_states.MAGNETIC_HEADING = false;
	state.available_states.GROUND_TRACK_ANGLE = false;
	state.available_states.TRUE_AIRSPEED = false;
	state.available_states.INDICATED_AISPEED = false;
	state.available_states.GROUND_SPEED_NORTH = false;
	state.available_states.GROUND_SPEED_EAST = false;
	state.available_states.GEOMETRIC_VERTICAL_RATE = false;
	state.available_states.PRESSURE_VERTICAL_RATE = false;
	state.available_states.HEADING_RATE = false;
	state.available_states.GROUND_TRACK_RATE = false;
	state.available_states.MODE = false;

	//Time of applicability. Units=[sec]
	state.toa = 0;

	//Longitude in degrees referenced to WGS84. Units=[deg]
	state.longitude = 0;

	//Latitude in degrees referenced to WGS84. Units=[deg]
	state.latitude = 0;

	//height above WGS84 ellipsoid. Units=[m]
	state.geometric_altitude = 0;

	//Barometric pressure altitude relative to 1013.25 millibars. Units=[sec]
	state.barometric_altitude = 0;

	//Units=[deg]
	state.true_heading = 0;

	//Units=[deg]
	state.magnetic_heading;

	//Units=[deg]
	state.ground_track_angle = 0;

	//Units=[m/s]
	state.true_air_speed = 0;

	//Units=[m/s]
	state.indicated_air_speed = 0;

	//Units=[m/s]
	state.ground_speed_north = 0;

	//Units=[m/s]
	state.ground_speed_east = 0;

	//Units=[m/s]
	state.geometric_vertical_rate = 0;

	//Units=[m/s]
	state.pressure_vertical_rate = 0;

	//Units=[deg/s]
	state.heading_rate = 0;

	//Units=[deg/s]
	state.ground_track_rate = 0;

	//TODO: mode needs documentation!
	state.mode = ownship_stateICD::flight_mode::CRUISE;

	//Units=[m]
	state.horizonal_position_uncertainty = 0;

	//Units=[m]
	state.pressure_altitude_uncertainty = 0;

	//Units=[m]
	state.geometric_altitude_uncertainty = 0;

	//Units=[m/s]
	state.ground_speed_uncertainty = 0;

	//Units=[m/s]
	state.airspeed_uncertainty = 0;

	//Units=[deg]
	state.heading_uncertainty = 0;

	//Units=[deg]
	state.ground_track_angle_uncertainty = 0;

	//Units=[deg/s]
	state.heading_rate_uncertainty = 0;

	//Units=[deg/s]
	state.ground_track_rate_uncertainty = 0;
}

HANDLE mops2track::makeMailSlot()
{
	HANDLE hSlot = CreateMailslotW(lpRemixerSlotName,
		1024,      // maximum message size 
		0,         // no time-out for operations 
		(LPSECURITY_ATTRIBUTES)NULL); // default security

	auto &logger = Poco::Logger::get("remixer");
	if (hSlot == INVALID_HANDLE_VALUE)
	{
		logger.error("CreateMailslotW() failed with %d\n", GetLastError());
	}
	return hSlot;
}

//convertion: intruder track track to ownership state
std::optional<ownship_stateICD::aircraft_state>
mops2track::intruder2ownship(const trackICD::track &track) noexcept
{
	ownship_stateICD::aircraft_state ownshipState;

	setDefaultValues(ownshipState);

	//callsign set, ownship, TODO: parametrizar
	//strncpy(ownshipState., "OWNSHIP", strlen("OWNSHIP"));
	ownshipState.available_states.TOA = track.aircraft_state.available_states.TOA;
	ownshipState.toa = track.aircraft_state.toa;

	ownshipState.available_states.LATITUDE = track.aircraft_state.available_states.LATITUDE;
	ownshipState.latitude = track.aircraft_state.latitude;
	
	ownshipState.longitude = track.aircraft_state.longitude;
	ownshipState.available_states.LONGITUDE = track.aircraft_state.available_states.LONGITUDE;
	
	ownshipState.geometric_altitude = track.aircraft_state.geometric_altitude;
	ownshipState.available_states.GEOMETRIC_ALTITUDE = track.aircraft_state.available_states.GEOMETRIC_ALTITUDE;

	ownshipState.barometric_altitude = track.aircraft_state.barometric_altitude;
	ownshipState.available_states.BAROMETRIC_ALTITUDE = track.aircraft_state.available_states.BAROMETRIC_ALTITUDE;
	
	ownshipState.ground_speed_east = track.aircraft_state.ground_speed_east;
	ownshipState.available_states.GROUND_SPEED_EAST = track.aircraft_state.available_states.GROUND_SPEED_EAST;
	
	ownshipState.ground_speed_north = track.aircraft_state.ground_speed_north;
	ownshipState.available_states.GROUND_SPEED_NORTH = track.aircraft_state.available_states.GROUND_SPEED_NORTH;
	
	ownshipState.geometric_vertical_rate = track.aircraft_state.geometric_vertical_rate;
	ownshipState.available_states.GEOMETRIC_VERTICAL_RATE = track.aircraft_state.available_states.GEOMETRIC_VERTICAL_RATE;
	
	ownshipState.true_heading = track.aircraft_state.true_heading;
	ownshipState.available_states.TRUE_HEADING = track.aircraft_state.available_states.TRUE_HEADING;

	ownshipState.magnetic_heading = track.aircraft_state.magnetic_heading;
	ownshipState.available_states.MAGNETIC_HEADING = track.aircraft_state.available_states.MAGNETIC_HEADING;

	ownshipState.ground_track_angle = track.aircraft_state.ground_track_angle;
	ownshipState.available_states.GROUND_TRACK_ANGLE = track.aircraft_state.available_states.GROUND_TRACK_ANGLE;

	ownshipState.true_air_speed = track.aircraft_state.true_air_speed;
	ownshipState.available_states.TRUE_AIRSPEED = track.aircraft_state.available_states.TRUE_AIRSPEED;

	ownshipState.pressure_vertical_rate = track.aircraft_state.pressure_vertical_rate;
	ownshipState.available_states.PRESSURE_VERTICAL_RATE = track.aircraft_state.available_states.PRESSURE_VERTICAL_RATE;

	ownshipState.heading_rate = track.aircraft_state.heading_rate;
	ownshipState.available_states.HEADING_RATE = track.aircraft_state.available_states.HEADING_RATE;

	ownshipState.ground_track_rate = track.aircraft_state.ground_track_rate;
	ownshipState.available_states.GROUND_TRACK_RATE = track.aircraft_state.available_states.GROUND_TRACK_RATE;

	ownshipState.mode = (ownship_stateICD::flight_mode)track.aircraft_state.mode;
	ownshipState.available_states.MODE = track.aircraft_state.available_states.MODE;

	return ownshipState;
}