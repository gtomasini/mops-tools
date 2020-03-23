#include <iostream>

#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Format.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"

#include "daa_datatypes_icd/trackICD.h"
#include "daa_datatypes_icd/ownship_stateICD.h"
#include "daa_datatypes_Icd/IcdAvoidanceOutput.h"
#include "trackLib/trackDisplay.h"

#include "../avoidance_suscriber/avoidance_path.h"
#include "../avoidance_suscriber/IcdPlan.cpp"
#include "../avoidance_suscriber/avoidance_path_2_xml.h"
#include "daa_datatypes_icd/alert_packageICD.h"

using namespace std;

//TODO: put in another place
avoidance_path 
fatpaths2avoidancePath(IcdAvoidanceOutput::FatPathsAndWayPoints const& a_fatpaths)  {
	avoidance_path a_avoidance_path;
	a_avoidance_path._date = Poco::DateTimeFormatter::format(Poco::Timestamp{ Poco::Timestamp::fromEpochTime(a_fatpaths.Time) }, Poco::DateTimeFormat::SORTABLE_FORMAT);
	a_avoidance_path._waypoints.reserve(a_fatpaths.WyPtPath.Path.NumPoints);

	auto i = 0;
	for (auto x : a_fatpaths.WyPtPath.Path.Path) {
		if (i++ == a_fatpaths.WyPtPath.Path.NumPoints) break;
		waypoint a_waypoint;
		a_waypoint._id				= Poco::format("%s", (int)x.ID);
		//a_waypoint._longitude_rad	= x.LonRad;
		a_waypoint._longitude_rad = x.Longitude;
		//a_waypoint._latitude_rad	= x.LatRad;
		a_waypoint._latitude_rad = x.Latitude;
		//a_waypoint._altitude_m		= x.AltMtr;
		a_waypoint._altitude_m = x.Altitude;
		//a_waypoint._ground_speed_mps	= x.GrndSpdMtrSec;
		a_waypoint._ground_speed_mps = x.GrndSpd;
		//a_waypoint._ground_track_rad	= x.GroundTrackRad;
		a_waypoint._ground_track_rad = x.GroundTrack;
		a_avoidance_path._waypoints.push_back(a_waypoint);
	}

	return a_avoidance_path;
}

int main(int argc, char **argv) {
	//auto ipAddr = argc > 1 ? atoi(argv[1]) : "0.0.0.0";
	auto port = argc > 1 ? atoi(argv[1]) : 2200;
	cout << "please write \"" << argv[0] << " [udp_port]\"" << endl;
	auto addr = Poco::Net::IPAddress();
	cout << "\tlistening UDP packets on "<< addr<<":"<< port<< endl;

	//IPAddress() actually "means" "0.0.0.0"
	Poco::Net::SocketAddress sa(addr, port);
	Poco::Net::DatagramSocket dgs(sa);

	static DWORD mytime0 = 0;//msecs
	for (;;)
	{
		Poco::Net::SocketAddress sender;
		char buffer[8000];

		try {
			auto n = dgs.receiveFrom(buffer, sizeof(buffer), sender);

			if (n == 0 || n < 0) {
				std::cerr << "receive From error:" << n << std::endl;
				continue;
			}

		
		if (mytime0 == 0)	mytime0 = GetTickCount();
		DWORD mytime = GetTickCount() - mytime0;
		cout << endl<< mytime<<" msecs:\t" << sender.toString() << ": " << n << " bytes received====================" << endl;;

		if (n == sizeof(alert_packageICD::alert_package_buffer) ) {
			static auto alertCounter = 0;
			alert_packageICD::alert_package alert;
			//alert_packageICD::read_alert_package(alert, (alert_packageICD::alert_package_buffer &) buffer);
			alert_packageICD::read_alert_package_ptr(alert, (const char *)buffer);

			std::cout << "alert packed received #"<<alertCounter++<<"\n"
				<< "\t #num_alerts: " << (unsigned) alert.num_alerts << std::endl;
			for (auto i = 0; i < alert.num_alerts; ++i) {
				std::cout << i << ") type:" << (unsigned)alert.current_alerts[i].type
					<< ", alert_time: " << alert.current_alerts[i].alert_time << std::endl;
			}
			//cout << "alert[0] type: " << alert.current_alerts[0].type << std::endl;
			Poco::Net::SocketAddress avoidanceSA("127.0.0.1", 4000);
			Poco::Net::DatagramSocket dgs;
			dgs.connect(avoidanceSA);
			dgs.sendBytes(buffer, sizeof(alert_packageICD::alert_package_buffer));
		}
		else if (n == sizeof(IcdAvoidanceOutput::FatPathsAndWayPoints_buffer) )
		{
			std::cout << "FatPath received..." << std::endl;
			IcdAvoidanceOutput::FatPathsAndWayPoints a_fatpaths;
			IcdAvoidanceOutput::read_FatPathsAndWayPoints(a_fatpaths, (IcdAvoidanceOutput::FatPathsAndWayPoints_buffer &)buffer);

			if (a_fatpaths.WyPtPath.Path.NumPoints) {
				std::cout << "#waypoints:" << (unsigned)a_fatpaths.WyPtPath.Path.NumPoints << std::endl;
				auto aa_path = fatpaths2avoidancePath(a_fatpaths);
				auto dd = aa_path._date;
				static auto ii = 0;
				std::cout << "(" << ii++ << ") avoidance_path._date: " << dd << std::endl;
				auto i = 0;
				for (auto x: aa_path._waypoints) {
					const auto M_PI = 3.14159265358979323846;
					std::cout.precision(10);
					//std::cout << "\t" << i++ << ") lat:"
					//	<< x._latitude_rad * 180 / M_PI << ", lon:"
					//	<< x._longitude_rad * 180 / M_PI << std::endl;
					std::cout << "\t" << i++ << ") lat: "
						<< x._latitude_rad  << ", lon: "
						<< x._longitude_rad << ", alt: "
						<< x._altitude_m << ", grSpd: "
						<< x._ground_speed_mps << ", grTrk: "
						<< x._ground_track_rad << ", id: "
						<< x._id << std::endl;
				}
			}
		}
		else if (n == sizeof(IcdPlan::MissionPlan_buffer))
		{
			IcdPlan::MissionPlan plan;
			read_MissionPlan(plan, (IcdPlan::MissionPlan_buffer &) buffer);

			cout << "plan received, #points:" << (int)plan.NumPoints 
				<< "id: " << plan.CurrWyPtID << std::endl;
			for (int i = 0; i < (int)plan.NumPoints; ++i) {
				IcdPlan::GuidancePoint	gui_point = plan.Plan[i];
				cout << "\tid=" << gui_point.ID <<", lat: "
					<<gui_point.Latitude << ", lon:" << gui_point.Longitude << std::endl;
			}
		}
		else if (n==sizeof(trackICD::track_buffer) )//intruder
		{
			trackICD::track track;
			trackICD::track_buffer trackBuf;
		
			//void read_track(track& data, const track_buffer& raw);
			memcpy(trackBuf, buffer, sizeof(trackBuf));
			trackICD::read_track(track, trackBuf);
			trackDisplay(track);
		}
		else if ( n== sizeof(ownship_stateICD::aircraft_state_buffer))//ownship
		{
			ownship_stateICD::aircraft_state state;
			ownship_stateICD::aircraft_state_buffer ownshipBuffer;
			
			memcpy(ownshipBuffer, buffer, n);
			ownship_stateICD::read_aircraft_state(state, ownshipBuffer);

			stateDisplay(state);
		}
		else
		{
			std::cerr << "unknow received packet, size: " << n << std::endl;
		}
		}
		catch (const std::exception& ex)
		{
			std::cerr << "catched exception: " << ex.what() << std::endl;
			continue;
		}
		catch (...)
		{
			std::cerr << "catched unknown exception...." << std::endl;
			continue;
		}
	}

	return 0;
}
