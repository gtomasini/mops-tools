#include <thread>
#include <Poco/DateTimeParser.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <pococonf/configurator.h>
#include <pocogger/logger_file.h>
#include <pocogger/poco_logger.h>
#define _USE_MATH_DEFINES 

#include "daa_datatypes_icd/IcdFlightPlan.h"

#include "avoidance_subscriber.h"
#include "avoidance_path_2_xml.h"
#include "tp_server/mailslotServer.h"
#include <windows.h>

#define DEBUG

int avoidance_subscriber::initialize(configurator_t const& a_conf,
	poco::logger_file &log) {
	using namespace swim::opendds;
	_logger = log;
    auto& the_entity = entity::instance();
    the_entity.set_config(settings{ a_conf.get_section("dds_config").getFilePath("config") });
    auto domain_id = a_conf.get_section("dds_config").getInt("avoidance_fatpaths_domain");
    auto topic_id = a_conf.get_section("dds_config").getString("avoidance_fatpaths_topic");
	_udp_server = a_conf.get_section("destination").getString("ip");
	_udp_port = a_conf.get_section("destination").getInt("port");
	_xmlPar._m = a_conf.get_section("tp_xml").getUInt("m");
	_xmlPar._tas = a_conf.get_section("tp_xml").getUInt("tas");
	_xmlPar._stage = a_conf.get_section("tp_xml").getString("stage");

	//Warning if _stage exists the _xmlPar will be overwritte
	if (!_xmlPar._stage.empty() && a_conf.has_section(_xmlPar._stage)) {
		_xmlPar._APM_id = a_conf.get_section(_xmlPar._stage).getString("APM_id");
		_xmlPar._m = a_conf.get_section(_xmlPar._stage).getUInt("m");
		_xmlPar._tas = a_conf.get_section(_xmlPar._stage).getUInt("tas");
	}

    _subscriber.set_participant(the_entity.get_participant(domain{ domain_id }));
    _subscriber.create_listener(topic{ topic_id });
    _subscriber.listener().subscribe(*this);

	logger_debug(Poco::format("Stage: %s, APM_id: %s, tas: %d", 
		_xmlPar._stage.c_str(), _xmlPar._APM_id.c_str(), _xmlPar._tas ));
	return createTPserverMailSlot();
}

int avoidance_subscriber::createTPserverMailSlot()
{
	//Connect to the server mailslot using CreateFile()
	_hTPserverMailslot = CreateFileW(
		lpTPserverSlotName,        		// mailslot name 
		GENERIC_WRITE,         // mailslot write only 
		FILE_SHARE_READ,       // required for mailslots
		NULL,                  // default security attributes
		OPEN_EXISTING,         // opens existing mailslot 
		FILE_ATTRIBUTE_NORMAL, // normal attributes 
		NULL);                 // no template file 

	if (INVALID_HANDLE_VALUE == _hTPserverMailslot)
	{
		logger_error(Poco::format("ERROR connecting to TPServer MailSlot: %d",
				GetLastError()));
		return 1;  //Error
	}
	return 0;
}


int avoidance_subscriber::send2TPserver(const std::string & msg)
{
	for (int i = 0; i < 3; ++i) {
		if (INVALID_HANDLE_VALUE == _hTPserverMailslot)
				createTPserverMailSlot();
		DWORD cbBytes;
		BOOL bResult = WriteFile(
			_hTPserverMailslot,     // handle to mailslot 
			msg.c_str(),           // buffer to write from 
			msg.size() + 1,   // number of bytes to write, include the NULL
			&cbBytes,             // number of bytes written 
			NULL);                // not overlapped I/O 

		if (!bResult || msg.size() + 1 != cbBytes){
			CloseHandle(_hTPserverMailslot);
			_hTPserverMailslot = INVALID_HANDLE_VALUE;
		}
		else {
			return 0;
		}
	}
	logger_error(Poco::format("Error occurred while writing "
			" to TPServer mailslot: %d", GetLastError() ));
	return 1;
}

avoidance_subscriber::~avoidance_subscriber() {
		CloseHandle(_hTPserverMailslot);
}


void avoidance_subscriber::run() {
    for(;;) 
	{
         _subscriber.wait_for_publisher_silent();
         using namespace std::chrono_literals;
         std::this_thread::sleep_for(1s);
    }
}

auto avoidance_subscriber::to_avoidance_path(IcdAvoidanceOutput::FatPathsAndWayPoints const& a_fatpaths) const -> avoidance_path    {
    avoidance_path a_avoidance_path;
    a_avoidance_path._date = Poco::DateTimeFormatter::format(Poco::Timestamp{ Poco::Timestamp::fromEpochTime(a_fatpaths.Time) }, Poco::DateTimeFormat::SORTABLE_FORMAT);
    a_avoidance_path._waypoints.reserve(a_fatpaths.WyPtPath.Path.NumPoints);

    for (auto index = 0; index < a_fatpaths.WyPtPath.Path.NumPoints; ++index)
    {
        waypoint a_waypoint;
        a_waypoint._id = Poco::format("%s",(int)a_fatpaths.WyPtPath.Path.Path[index].ID);
        a_waypoint._longitude_rad = a_fatpaths.WyPtPath.Path.Path[index].LonRad;
        a_waypoint._latitude_rad = a_fatpaths.WyPtPath.Path.Path[index].LatRad;
        a_waypoint._altitude_m = a_fatpaths.WyPtPath.Path.Path[index].AltMtr;
        a_waypoint._ground_speed_mps = a_fatpaths.WyPtPath.Path.Path[index].GrndSpdMtrSec;
        a_waypoint._ground_track_rad = a_fatpaths.WyPtPath.Path.Path[index].GroundTrackRad;
		a_avoidance_path._waypoints.push_back(a_waypoint);
    }
    return a_avoidance_path;
 }


void avoidance_subscriber::on_data_available(daa_dds::avoidance_fatpaths_buffer const& a_data)
{
    auto a_fatpaths_dds_buffer = static_cast<const char*>(a_data.data.get_buffer());
    IcdAvoidanceOutput::FatPathsAndWayPoints a_fatpaths;
    IcdAvoidanceOutput::read_FatPathsAndWayPoints_ptr(a_fatpaths, a_fatpaths_dds_buffer);

    if (a_fatpaths.WyPtPath.Path.NumPoints){
		//here I must signal Mixer in order to stop it.
		//TODO: avisar a la batidora con un mailslot

		//OFP
		//MissionPlanProcessing planProcessing(_udp_server, _udp_port);
		auto aa_path = to_avoidance_path(a_fatpaths);
		//planProcessing.processAvoidancePath(aa_path);
		
		/* onother way
		IcdPlan::MissionPlan my_plan;
		for (auto i=0; i < aa_path._waypoints.size(); i++ ) 
			planProcessing.waypoint2guipoint(aa_path._waypoints[i], my_plan.Plan[i], i);
		
		my_plan.CurrWyPtID = my_plan.Plan[0].ID;
		my_plan.NumPoints = aa_path._waypoints.size();

		// output each package though UDP
		planProcessing.sendMissionPlan(my_plan);
		*/
		//fix TAS in 28mps
		avoidance_path_2_xml apath2xml(aa_path, _xmlPar);

#ifdef DEBUG
		auto dd = aa_path._date;
		static int ii = 0;
		std::cout << "(" << ii++ << ") avoidance_path._date: " << dd << std::endl;
		std::cout << "ground speed[0]: " << aa_path._waypoints[0]._ground_speed_mps << std::endl;
		auto i = 0;
		for (auto x : aa_path._waypoints) {
			std::cout.precision(20);
			std::cout << "\t"<< i++ << ") lat:"
				<< x._latitude_rad * 180 / M_PI << ", lon:"
				<< x._longitude_rad * 180 / M_PI << std::endl;
		}
#endif

		try {
			//generate input and weather xmls
			std::string weatherFile, inputFile;
			auto st=apath2xml.write_input(weatherFile, inputFile);
			//std::cout << "file names:" << fileNames << std::endl;
			//here I must send xml fnames to TP server using mailslot
			if (send2TPserver(weatherFile+":"+inputFile)) {
				std::cerr << "writing to TPServer mailslot Error" << std::endl;
			}
			std::string fname = "avoidancePathOwnship_" + getYYYYMMDD_hhmmss() + ".kml";
			apath2xml.generateKml(fname);
			Sleep(1000);
		}
		catch (const std::exception& ex) {
			std::cerr << ex.what() << std::endl;
			throw;
		}
		catch (...) {
			std::exception_ptr p = std::current_exception();
			std::cerr << "unknown exception" << std::endl;
			throw;
		}
	}
}	