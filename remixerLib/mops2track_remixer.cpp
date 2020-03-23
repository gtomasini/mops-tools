#include <algorithm>
#include <thread>
#include <iostream>
#include <fstream>
#include <Poco/Logger.h>
#include <Poco/WindowsConsoleChannel.h>
#include <Poco/AutoPtr.h>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/SocketAddress.h>

#include "mops2track.h"
#include "parseCsv.h"
#include "trackLib/trace.h"
#include "tp_server/mailslotServer.h"

//#include "trackLib\daa_iotools\daa_udp.h"

#ifdef _DEBUG
#define DEBUG
#endif 

using Poco::Logger;
using namespace remixerLib;

Poco::AutoPtr<Poco::WindowsColorConsoleChannel> mops2track::_consoleChannel = new Poco::WindowsColorConsoleChannel();
Poco::AutoPtr<Poco::SimpleFileChannel> mops2track::_fileChannel = new Poco::SimpleFileChannel();

auto setZeroes = [&](std::vector <trackICD::track> & v, std::string const &filename) {
	for (auto i = 0; i < v.size(); ++i)
		if ((v[i].aircraft_state.geometric_altitude < 1.0f) && (i > 0) && (i + 1 < v.size())) {
			TRACE("%s, row=%d, toa=%ld has %f altitude.", filename.c_str(), i, v[i].aircraft_state.toa,
				v[i].aircraft_state.geometric_altitude);
			std::cerr << "WARNING: " << filename << ", row: " << i << ", alt: " << v[i].aircraft_state.geometric_altitude << std::endl;
			v[i].aircraft_state.geometric_altitude
				= (v[i - 1].aircraft_state.geometric_altitude + v[i + 1].aircraft_state.geometric_altitude) / 2;
		}
};

std::vector<GOOGLE_POINT>	mops2track::_ownGoogleVec;
std::vector<GOOGLE_POINT>	mops2track::_intGoogleVec;//in order to save google tracks
std::vector<std::string>	mops2track::_alertVec;

auto ownshipStateReset = [](auto &ownshipState) 
{
	ownshipState.true_air_speed = 0;
	ownshipState.available_states.TRUE_AIRSPEED = TRUE;
	ownshipState.heading_rate = 0;
	ownshipState.available_states.HEADING_RATE = TRUE;
	ownshipState.ground_track_rate = 0;
	ownshipState.available_states.GROUND_TRACK_RATE = TRUE;
};

//main loop remixer function
int
mops2track::remixer(
	const csvFilesTupleType &csvFilesTuple,
	std::chrono::seconds toaOffset,
	const std::string &logFile,
	const std::string &oBinFname) 
{
	auto &logger= Poco::Logger::get("remixer");
	auto[truthCsv, adsbCsv, radarCsv, ownshipCsv] = csvFilesTuple;

#ifdef DEBUG
	std::cout << "remixer csv ifiles: truth=\"" << truthCsv << "\", adsb=\"" << adsbCsv
		<< "\", radar=\"" << radarCsv << "\", ownship=\"" << ownshipCsv << std::endl;
	TRACE("remixer csv ifiles: truth=%s, adsb=%s, radar=%s, ownship=%s", 
		truthCsv.c_str(), adsbCsv.c_str(), radarCsv.c_str(), ownshipCsv.c_str());
#endif
	
	if (!logFile.empty())
	{
		_fileChannel->setProperty("path", _outdir+"/"+_stage+logFile);
		_fileChannel->setProperty("rotation", "100 K");
		//_fileChannel->setProperty("archive", "timestamp");
		logger.setChannel(_fileChannel);
	}
	else {
		_noConsole = false;
		logger.setChannel(_consoleChannel);//if empty logs in concole
	}

	auto loglevel0 = logger.getLevel();
	logger.setLevel(_loglevel);

	_fullTrackVector.clear();

	//read INTRUDER data from three sources and put it in fullTrackVector
	auto st=parseCsvFileAndSave(truthCsv, _fullTrackVector, source_data::fused, _callsign, _loglevel);
	setZeroes(_fullTrackVector, truthCsv);

	std::vector <trackICD::track> tempV;
	tempV.clear();
	st|=parseCsvFileAndSave(adsbCsv, tempV, source_data::adsb, _callsign, _loglevel);
	setZeroes(tempV, adsbCsv);
	_fullTrackVector.insert(std::end(_fullTrackVector), std::begin(tempV), std::end(tempV));

	tempV.clear();
	st|=parseCsvFileAndSave(radarCsv, tempV, source_data::radar, _callsign, _loglevel);
	setZeroes(tempV, radarCsv);
	_fullTrackVector.insert(std::end(_fullTrackVector), std::begin(tempV), std::end(tempV));

	tempV.clear();
	//read OWNSHIP data from three sources and also put it in fullTrackVector
	st|=parseCsvFileAndSave(ownshipCsv, tempV, source_data::ownship, "OWNSHIP", _loglevel);
	setZeroes(tempV, ownshipCsv);
	_fullTrackVector.insert(std::end(_fullTrackVector), std::begin(tempV), std::end(tempV));
	tempV.clear();
	
	if (_fullTrackVector.empty()) {
		logger.error("remixer: no registers read from intruder/ownship csv input files, so no action taken...");
		if (_noConsole)
			std::cerr<<"remixer: no registers read from intruder / ownship csv input files, so no action taken...\n";
		logger.setLevel(loglevel0);
		return -1;
	}
		
	std::ofstream oFile, ownship_oFile;
	//open binary file only if oBinFname != ""
	if (!oBinFname.empty())
	{
		oFile.open("intruder-"+oBinFname, std::ios::out | std::ios::binary);
		ownship_oFile.open("ownship-"+oBinFname, std::ios::out | std::ios::binary);
		logger.debug("Binary output files (ownship)%s created", oBinFname );
		if (_noConsole)
			std::cerr << "Warning: binary mode, no UDP packet will be sent" << std::endl;
	}
	else
		logger.debug("NOT Binary output file created, so only UDP packets data will be sent.");

	auto mytime0 = GetTickCount64();//msecs

	//In order to save ownship/intruder  trajectory
	//	std::vector<ownship_stateICD::aircraft_state> ownshipVector;
	std::vector <trackICD::track> intruderVector, ownshipVector;
	std::vector <trackICD::track> intruderSubstageVector, ownshipSubstageVector;//sub stage vectors

	_hslot = makeMailSlot();//used by tp server
	if (_hslot == INVALID_HANDLE_VALUE) 
	{
		if (_noConsole)
		std::cerr << "makeMAilSlot() Error, aborting..." << std::endl;
		logger.setLevel(loglevel0);
		return -1;
	}
	//two operation modes:
	//stop mode:  frena la simulacion hasta que llega el csv (comando "STOP"), breakFlag->true, genera csv y kml ok
	//pause mode: no frena la simulacion, pero los ownship register generados en ese intervalo son erroneos
	logger.trace("Remixer: Simulation begins...");

//*******************************************************************************
	_breakFlag = false;
	_pauseFlag = false;
	_alertReceived = false;
	_alertPresent = false;
	_avoidancePathReceived = false;
	_ownshipKMLcolor = _ownshipKMLcolor0;
	_breakIndex = 0;
	std::ofstream ownshipKmlFileHandle, intruderKmlFileHandle;
	long ownshipFpos = 0, intruderFpos=0;
	createKmlTrackFile(googleOwnshipTrackFile,
					ownshipFpos,
					ownshipKmlFileHandle,
					ownshipCsv,
					_ownshipKMLopacity + _ownshipKMLcolor,
					_ownshipKMLicon,
					_ownshipKMLdescription);
	createKmlTrackFile(googleIntruderTrackFile,
			intruderFpos,
			intruderKmlFileHandle,
			truthCsv + "," + adsbCsv + "," + radarCsv,
			_ownshipKMLopacity + _intruderKMLcolor,
			_intruderKMLicon,
			"Intruder Live Track");

SIMULATION_BEGINS:
	//sort fullTrackVector index with track_time
	std::sort(_fullTrackVector.begin(), _fullTrackVector.end(),
		[](trackICD::track lhs, trackICD::track rhs) { return lhs.track_time < rhs.track_time; });

	auto t = _fullTrackVector.back().track_time - _fullTrackVector[0].track_time;
	logger.debug("TrackVector size: %ld, total simulation time: %ld secs, bin out file: \"%s\"",
		(size_t)_fullTrackVector.size(), (long)t, oBinFname);
#ifdef _DEBUG
	TRACE("TrackVector size: %ld, total simulation time: %ld secs, bin out file: \"%s\"",
		(size_t)_fullTrackVector.size(), (long)t, oBinFname);
#endif 

	double delay = 0, proratedDelay = 0;//msecs
	auto t0 = GetTickCount64();//msecs

	//huge for loop int/own vector processing
	for (size_t i=0 ; i < _fullTrackVector.size(); ++i) 
	{
		auto &track = _fullTrackVector[i];
		if (i + 1 < _fullTrackVector.size())
			delay = (_fullTrackVector[i + 1].track_time - track.track_time)*1000;//msecs
//READ_MAILSLOT:
		bool simulationBegins = false;
		auto st=readMailSlot(i, track, simulationBegins);
		if (!st && simulationBegins)goto SIMULATION_BEGINS;//restart simulation with new vector values
		if (i < _startIndex)
		{
			std::cerr << "index <" << _startIndex << ", so it is ignored..." << std::endl;
			logger.warning("index < %d, so it is ignored...", _startIndex);
			continue;
		}
		track.aircraft_state.toa += toaOffset.count();
		track.track_time = track.aircraft_state.toa;
		
		auto ownshipFlag = (bool) !strncmp(track.callsign, "OWN", 3);

		googlePoint gp(track.aircraft_state.latitude, track.aircraft_state.longitude, 
							track.aircraft_state.geometric_altitude );
		char *ship="INTRUDER";
		if (ownshipFlag) 
		{	//converts track to aircraft_state
			//ownship_stateICD::aircraft_state ownshipState;
			//ownshipStateReset(ownshipState);
			//mops2track::intruder2ownship(track, ownshipState);
			//no push if there is an ALERT pending
			ship = "OWNSHIP ";
			if (!_pauseFlag) 
				//ownshipVector.push_back(ownshipState);//saves state in vector
				ownshipVector.push_back(track);//saves state in vector

			updateKmlTrackFile(ownshipKmlFileHandle, gp, ownshipFpos);
		}
		else //if (track.source_data.FUSED_TRACK) 
		{	//Intruder kml update position/track
			intruderVector.push_back(track);
			updateKmlTrackFile(intruderKmlFileHandle, gp, intruderFpos);
		}
#ifdef DEBUG
		std::cout << ship<<", lat: " << track.aircraft_state.latitude
			<< ", lon: " << track.aircraft_state.longitude
			<< ", toa: " << track.aircraft_state.toa - toaOffset.count() << std::endl;
		TRACE("%s, lat:%.6f, lon:%.6f, alt:%.2f, north v:%.3f, east v:%.3f", 
			ship, track.aircraft_state.latitude,
			track.aircraft_state.longitude, track.aircraft_state.barometric_altitude,
			track.aircraft_state.ground_speed_north, track.aircraft_state.ground_speed_east);
#endif

		if (_alertPresent) 
		{
			char aux[256];
			snprintf(aux, sizeof(aux), "\t%s, lat: %.4f, lon: %.4f, alt: %.2f",
					(ownshipFlag?"Intruder":"Ownship"), gp._lat, gp._lon, gp._alt);
			_alertVec.push_back(aux);
		}

		if (_savingSubstage) {
			if (ownshipSubstageVector.empty() && intruderSubstageVector.empty()) {
				//first time do a rewind (rewind parameter) in order to rescue last positions of the stage
				int ii = i - _rewindSize;
				if (ii < 0) ii = 0;
				for (ii; ii <= i; ii++) 
					if (!strncmp(_fullTrackVector[ii].callsign, "OWN", 3))
						ownshipSubstageVector.push_back(_fullTrackVector[ii]);
					else 
						intruderSubstageVector.push_back(_fullTrackVector[ii]);
			}
			if ( ownshipFlag && (ownshipSubstageVector.size() < _substageSize))
				ownshipSubstageVector.push_back(track);
			else if (!ownshipFlag && (intruderSubstageVector.size() < _substageSize)) 
				intruderSubstageVector.push_back(track);
			//detects _alertPresent descending flank
			if (!_savingSubstageAlertPresentEnds && _alertPresent == false )
				_savingSubstageAlertPresentEnds = true;

			//end of substage, saves it
			if ( (intruderSubstageVector.size() >= _substageSize ||
				ownshipSubstageVector.size() >= _substageSize) 
				&& _savingSubstageAlertPresentEnds )
			{
				_savingSubstage = false;//ends
				//mops generation
				auto YYYYYMMDD_hhmmss = getYYYYMMDD_hhmmss();
				std::string fnameO = _outdir + "/" + _stage + "_partialOwnship_" + YYYYYMMDD_hhmmss + ".csv";
				generateCsv(ownshipSubstageVector, fnameO, toaOffset);
				std::string fnameI = _outdir + "/" + _stage + "_partialIntruder_" + YYYYYMMDD_hhmmss + ".csv";
				generateCsv(intruderSubstageVector, fnameI, toaOffset);
				TRACE("saving substage mops to %s and %s", fnameI.c_str(), fnameO.c_str());
				logger.trace("saving substage mops to %s and %s", fnameI, fnameO);
				//kml generation
				fnameO = _googleDir + "/" + _stage + "_partialOwnshipT_" + YYYYYMMDD_hhmmss + ".kml";
				generateKml(ownshipSubstageVector, fnameO, generateKmlDate(toaOffset.count()), true);
				fnameI = _googleDir + "/" + _stage + "_partialIntruderT_" + YYYYYMMDD_hhmmss + ".kml";
				generateKml(intruderSubstageVector, fnameI, generateKmlDate(toaOffset.count()), false);
				ownshipSubstageVector.clear();
				intruderSubstageVector.clear();
			}
		}

		if (track.aircraft_state.geometric_altitude < _minAlt) {
			std::cerr << "alt(m) <"<< _minAlt << ", so it is ignored..." << std::endl;
			logger.warning("alt(m) < %f, so it is ignored...", _minAlt);
			continue;
		}
		//sending oBuffer packet to udp server only if oFile is closed
		std::string src="???";
		sendUdpPacket(track, ownshipFlag, oFile, ownship_oFile, src);
		auto mytime=GetTickCount64()-mytime0;
		auto fortime = GetTickCount64() - t0;
		delay -= (fortime+proratedDelay);//emulates simulated time
		proratedDelay = 0;
		if (delay < 0) {
			proratedDelay = -delay;
			delay = 0;
		}
		if (_noSleep == false && !oFile.is_open())
			std::this_thread::sleep_for(std::chrono::milliseconds((unsigned)delay));
		t0 = GetTickCount64();//msecs
		logger.debug("=== %lu: %s(%s), tid: %u, delay= %ld, toa=%ld, for-time= %ld msecs\tSENT.",
				mytime, ownshipFlag ? std::string("OWN") : std::string("INT"),
				src, track.track_id, (long)delay, (long) (track.track_time- toaOffset.count()), (long)fortime);
		delay = 0;//msecs
	}//for()
	
SIMULATION_ENDS:
	logger.trace("simulation ends....");
	//save final ownship/intruder csv tracks (4 Jesus)
	std::string YYYYMMDD_hhmmss = getYYYYMMDD_hhmmss();
	std::string fname = _outdir+"/"+ _stage +"_finalOwnship_" + YYYYMMDD_hhmmss + ".csv";
	generateCsv(ownshipVector, fname, toaOffset);
	fname = _outdir + "/" + _stage + "_finalIntruder_" + YYYYMMDD_hhmmss + ".csv";
	generateCsv(intruderVector, fname, toaOffset);

	//	const std::string &date,//=std::string("2017-01-01T"),
	fname = _googleDir+"/"+_stage+"_finalOwnshipT_" + YYYYMMDD_hhmmss + ".kml";
	generateKml(ownshipVector, fname, generateKmlDate(toaOffset.count()), true);
	fname = _googleDir + "/" + _stage + "_finalIntruderT_" + YYYYMMDD_hhmmss + ".kml";
	generateKml(intruderVector, fname, generateKmlDate(toaOffset.count()), false);
	fname = _googleDir + "/" + _stage +"_finalOwnship_" + YYYYMMDD_hhmmss + ".kml";
	generateKml(ownshipVector, fname, generateKmlDate(toaOffset.count()), true, false);
	fname = _googleDir + "/" + _stage + "_finalIntruder_" + YYYYMMDD_hhmmss + ".kml";
	generateKml(intruderVector, fname, generateKmlDate(toaOffset.count()), false, false);
	
	if (_alertReceived && !_avoidancePathReceived)
	{
		logger.error("WARNING, alert(s) received but not avoidance path");
		_alertVec.push_back("WARNING: alert(s) received but not avoidance path");
	}

	generateAlertRepot(_alertsDir+"/"+ _stage+ "_alertReport_" + getYYYYMMDD_hhmmss() + ".log");

	auto createHidddenFile=[](auto &fname)
	{
		remove(fname);
		std::fstream fs;
		fs.open(fname, std::ios::out);
		fs.close();
		auto attrib = GetFileAttributes(fname);
		SetFileAttributes(fname, attrib + FILE_ATTRIBUTE_HIDDEN);
		return 0;
	};

	//in order to notify tp-server about the loop end or test end
	createHidddenFile("__fin__");

	if (oFile.is_open()) {
		oFile.close();
		ownship_oFile.close();
	}

	closeKmlTrackFile(ownshipKmlFileHandle);
	closeKmlTrackFile(intruderKmlFileHandle);

	logger.setLevel(loglevel0);
	return 0;
}

int mops2track::readMailSlot(unsigned i, trackICD::track const &track, bool &simulationBegins)
{
	DWORD cbBytes; char szBuffer[1024];
	auto &logger = Poco::Logger::get("remixer");
	auto tagtime = GetTickCount64();//msecs
	simulationBegins = false;

READ_MAILSLOT:
	BOOL bResult = ReadFile(_hslot, szBuffer, sizeof(szBuffer), &cbBytes, NULL);
	for (; _breakFlag && !bResult; Sleep(10))
		bResult = ReadFile(_hslot, szBuffer, sizeof(szBuffer), &cbBytes, NULL);

	if (!bResult)
	{
		return -1;//no message
	}

	//diferent messages
	auto stopMsg = false;
	auto pauseMsg = false;
	auto csvMsg = false;
	auto alertMsg = false;
	auto errMsg = false;

	if (!strncmp("STOP", szBuffer, 3)) stopMsg = true;
	else if (!strncmp("PAUSE", szBuffer, 3)) pauseMsg = true;
	else if (!strncmp("CSV", szBuffer, 3)) csvMsg = true;
	else if (!strncmp(szBuffer, "ALERT", 3)) alertMsg = true;
	else if (!strncmp(szBuffer, "ERROR", 3)) alertMsg = true;

	alertPush(track, szBuffer, i);
	logger.notice("\"%s\" message received from TP Server.", std::string(szBuffer));
	if (stopMsg || pauseMsg) 
	{	//STOP or PAUSE mailslot msg received from tp_server
		//save the remaining intruder registers from #i ahead
		_avoidancePathReceived = true;
		//breakIndex = i;//saves break index
		_ownshipKMLcolor = _ownshipAvoidanceColor;

#ifdef DEBUG
		std::cout << "------" << szBuffer << " required at entry #" << i
			<< ", track time(secs):" << track.track_time << std::endl;
#endif
		_breakTime = track.track_time;
		//Saves remaining intruder registers to come (ahead)
		_breakIndex = i;

		//_ownshipKMLcolor = _ownshipKMLalertColor;
		_ownshipKMLdescription = "Avoidance Path processed.";

		if (stopMsg)
		{	//stop simulaiton for wait csv file in "busy loop"
			_breakFlag = true;
			logger.notice("waiting for csv file name message from TP Server...", std::string(szBuffer));
			goto READ_MAILSLOT;//waits for csv trajectory file from TP Server....
		}
		else 
		{
			//simulations continues BUT with original ownship data
			//until csv name trajectory is received from tp_server
			_pauseFlag = true;
		}
	}
	else if ((_breakFlag || _pauseFlag) && csvMsg )
	{	//csv file name received, from #pos 3
		_breakFlag = _pauseFlag = false;
		auto wastedtime = GetTickCount64() - tagtime;//msecs
#ifdef DEBUG
		std::cout << "--------Break Time: " << _breakTime
			//<< ", recalculatedTrackVec[0].track_time:" << recalculatedTrackVec[0].track_time
			//<< ", toa: " << recalculatedTrackVec[0].aircraft_state.toa
			<< ", CSV to read: " << &szBuffer[4]
			<< ", wasted time: " << wastedtime << "msecs " << std::endl;
#endif
		//read new csv ownship path generated by tp_server
		std::vector <trackICD::track> tpTrackVec;

		if (parseCsvFileAndSave(std::string(&szBuffer[4]), tpTrackVec,
								source_data::ownship, "OWNSHIP", "error"))
		{
			logger.error("New ownship trajectory csv file parse error.");
			if (_noConsole)
			std::cerr << "New ownship trajectory csv file parse error." << std::endl;
			alertPush(track, "New ownship trajectory csv file parse error.", i);
			return -1;
		}
		else
		{
			auto tf = tpTrackVec.back().track_time;
			auto tff = tf + _breakTime;
#ifdef DEBUG
			std::cout << "NOTICE: " << tpTrackVec.size() << " registers read from " << &szBuffer[4] << std::endl
				<< "\t\tnew tp route, duration: " << tf << " , duration+_breakTime: " << tff << std::endl
				<< "NOTICE: curent fullTrackVector size: " << _fullTrackVector.size() << " entries" << std::endl;
#endif
			std::vector <trackICD::track> recalculatedTrackVec;
			auto m = 0, mm = 0, n = 0;
			for (int j = _breakIndex; j < _fullTrackVector.size(); j++)
				if (strncmp(_fullTrackVector[j].callsign, "OWN", 3))
				{	//saves intruder registers
					recalculatedTrackVec.push_back(_fullTrackVector[j]);
					mm++;
				}
				else if (_fullTrackVector[j].track_time > tff)
				{	//saves ownship registers after tp route ends
					recalculatedTrackVec.push_back(_fullTrackVector[j]);
					m++;
				}
				else
					n++;
#ifdef DEBUG
			std::cout << "NOTICE: " << mm << " intruder entries, " << m << " ownship entries saved, " << n << " discarted.\n"
				<< "\t\t: " << recalculatedTrackVec.size() << " total registers ahead saved." << std::endl;
#endif
			//shifts track_time and toa
			for (auto &x : tpTrackVec)
			{
				x.track_time += _breakTime;
				x.aircraft_state.toa += _breakTime;
			}

			recalculatedTrackVec.insert(recalculatedTrackVec.end(), tpTrackVec.begin(), tpTrackVec.end());
			_fullTrackVector = std::move(recalculatedTrackVec);//recalculatedTrackVec is not needed anymore

			logger.debug("Resuming Simulation with New ownship trajectory from avoidance...%d entries", _fullTrackVector.size());
			auto wastedtime2 = GetTickCount64() - tagtime;//msecs
#ifdef DEBUG
			std::cout << "NOTICE: new _fullTrackVector size: " << _fullTrackVector.size() << " entries. Reesuming Simulation" << std::endl;
			std::cout << "\t\tFINAL tp consumed time: " << wastedtime2 << "msecs " << std::endl;
#endif
			simulationBegins = true;
			return 0;
		}
	}
	else if (alertMsg)
	{	//ALERT received
		if (!strncmp(szBuffer, "ALERT_ENDS", 8))
		{
			_ownshipKMLdescription = "Alert ends";
			TRACE("Alert ends...");
			logger.notice("Alert ends...");
			_alertPresent = false;	//alertPresent flag reset
			_ownshipKMLcolor = _ownshipKMLcolor0;
			if (_avoidancePathReceived) _ownshipKMLcolor = _ownshipAvoidanceColor;
		}
		else
		{	//some alert begins 
			TRACE("Alert begins...");
			logger.notice("Alert begins...");
			_alertReceived	= true;	//WARNING: this flag is not reset in the future!
			_savingSubstage = true;	//this flag will be reset at the end of substage saving
			_alertPresent	= true;	//this flag is reset at the end of alert
			_ownshipKMLdescription.assign(szBuffer);
			_ownshipKMLcolor = setOwnshipKMLColor(szBuffer[6] - '0');
		}
#ifdef DEBUG
		std::cout << "NEW Ownship KML Alert Color: " << _ownshipKMLcolor << std::endl;
#endif
	}
	else if (errMsg)
	{	//ERROR received from tp
		_breakFlag = _pauseFlag = false;
		logger.error("ERROR message received from TP Server \"%s\".",
			std::string(szBuffer));
		_ownshipKMLdescription.assign("TP Calculation ERROR");
		alertPush(track, "TP Calculation Error", i);
		//break;
		return -1;//error
	}
	else 
	{
		logger.notice("Unknown message received from TP Server \"%s\".",
			std::string(szBuffer));
		if (_noConsole)
		std::cerr << "Unknown message received from TP Server \"" << szBuffer << std::endl;
		return -1;//error
	}
	return 0;//OK
}

int
mops2track::sendUdpPacket(trackICD::track const &track, bool ownshipFlag,
	std::ofstream &intObinFile, std::ofstream &ownshipObinFile, std::string &src)
{
	auto[truthPort, adsbPort, radarPort, ownshipPort] = _portsTuple;
	auto &logger = Poco::Logger::get("remixer");
	trackICD::track_buffer oBuffer;
	trackICD::write_track(track, oBuffer);
	
	//ownship_stateICD::aircraft_state ownshipState;
	//ownshipStateReset(ownshipState);
	auto opt=mops2track::intruder2ownship(track);
	auto ownshipState = opt.value();
	
	ownship_stateICD::aircraft_state_buffer oBufferOwnship;
	write_aircraft_state(ownshipState, oBufferOwnship);

	Poco::Net::SocketAddress truthSA(_udpServerAddress, truthPort);
	Poco::Net::SocketAddress adsbSA(_udpServerAddress, adsbPort);
	Poco::Net::SocketAddress radarSA(_udpServerAddress, radarPort);
	Poco::Net::SocketAddress ownshipSA(_udpServerAddress, ownshipPort);
#ifdef DEBUG
	Poco::Net::SocketAddress ownshipSA_debug(_udpServerAddress, "2101");
#endif
	Poco::Net::DatagramSocket dgs, dgs_debug;
	src = "UNKNOWN";
	int st{ 0 };
	if (ownshipFlag) {
		dgs_debug.connect(ownshipSA_debug);
		dgs.connect(ownshipSA);
	}
	else if (track.source_data.ADSB_TRACK) {
		src = "ADSB_TRK ";
		dgs.connect(adsbSA);
	}
	else if (track.source_data.RADAR_TRACK) {
		src = "RADAR_TRK";
		dgs.connect(radarSA);
	}
	else if (track.source_data.FUSED_TRACK) {
		src = "FUSED_TRK";//truth data
		dgs.connect(truthSA);
	}
	else {
		std::string aux = ownshipFlag ? std::string("OWN") : std::string("INT");
		logger.error("remixer: %s, src: %s, tid: %u will NOT be sent, because src is unknown.",
			aux, src, track.track_id);
		if (_noConsole)
		std::cerr << "remixer: " << aux << ", src:" << src << ", tid:" << track.track_id << std::endl;
		st=-1;
	}
	if (!ownshipFlag)
		//Intruder
		dgs.sendBytes(&oBuffer, sizeof(oBuffer));
	else
	{	//ownship
		dgs.sendBytes(&oBufferOwnship, sizeof(oBufferOwnship));
#ifdef DEBUG
		dgs_debug.sendBytes(&oBufferOwnship, sizeof(oBufferOwnship));
#endif
	}
			
	if (ownshipFlag && ownshipObinFile.is_open() )
		ownshipObinFile.write((char*)&oBufferOwnship, sizeof(oBufferOwnship));
	else if (ownshipFlag && intObinFile.is_open())
		intObinFile.write((char*)&oBuffer, sizeof(oBuffer));

	return st;
}