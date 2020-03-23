#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/Format.h>
#include <Poco/SimpleFileChannel.h>
#include <Poco/AutoPtr.h>
#include <filesystem>
#include <string>

#include "avoidanceProxy.h"
#include "utility"
#include "daa_datatypes_icd/IcdAvoidanceOutput.h"
#include "../avoidance_suscriber/avoidance_path.h"
#include "daa_datatypes_icd/alert_packageICD.h"
//#include "../avoidance_suscriber/avoidance_path_2_xml.h"
#include "../avoidance_suscriber/avoidance_path_2_xml.cpp"
#include "trackLib/trace.h"

//Poco::Logger &avoidanceProxy::_logger = Poco::Logger::get("tpserver");

#define DEBUG

using Poco::AutoPtr;

avoidanceProxy::avoidanceProxy(unsigned port, const std::string &stage):_port(port), _stage(stage)
{
	//AutoPtr<Poco::SimpleFileChannel> pChannel(new Poco::SimpleFileChannel);
	//pChannel->setProperty("path", "avoidanceproxy.log");
	//pChannel->setProperty("rotation", "100 K");
	//Poco::Logger::root().setChannel(pChannel);
}

avoidance_path 
avoidanceProxy::fatpaths2avoidancePath(IcdAvoidanceOutput::FatPathsAndWayPoints const& a_fatpaths) {
	avoidance_path a_avoidance_path;

	a_avoidance_path._date = Poco::DateTimeFormatter::format(Poco::Timestamp{ Poco::Timestamp::fromEpochTime(a_fatpaths.Time) }, Poco::DateTimeFormat::SORTABLE_FORMAT);
	a_avoidance_path._waypoints.reserve(a_fatpaths.WyPtPath.Path.NumPoints);

	auto i = 0;
	for (auto x : a_fatpaths.WyPtPath.Path.Path) {
		if (i++ == a_fatpaths.WyPtPath.Path.NumPoints) break;
		waypoint a_waypoint;
		a_waypoint._id = Poco::format("%s", (int)x.ID);
		a_waypoint._longitude_rad = x.Longitude;
		a_waypoint._latitude_rad = x.Latitude;
		a_waypoint._altitude_m = x.Altitude;
		a_waypoint._ground_speed_mps = x.GrndSpd;
		a_waypoint._ground_track_rad = x.GroundTrack;
		a_avoidance_path._waypoints.push_back(a_waypoint);
	}

	return a_avoidance_path;
}

//returns when avoidance path is received
std::tuple<std::string, std::string, std::string>
avoidanceProxy::receiveApath(xmlParameters const &xmlPar) 
{
#ifdef DEBUG
	std::cout << __FUNCTION__ << ", port:" << _port<< std::endl;
	TRACE("receivedApath, port %d", _port);
#endif
	auto &log = Poco::Logger::get("TPserver");// inherits root channel
	log.debug("receiveApath routine, port: %u", _port);
	//IPAddress() actually "means" "0.0.0.0"
	std::cout << __FUNCTION__ << ":" << __LINE__ << std::endl;
	Poco::Net::SocketAddress sa(Poco::Net::IPAddress(), _port);
	std::cout << __FUNCTION__ << ":" << __LINE__ << std::endl;
	Poco::Net::DatagramSocket dgs(sa);
	std::cout << __FUNCTION__ << ":" << __LINE__ << std::endl;
	Poco::Net::SocketAddress sender;
	//char buffer[sizeof(IcdPlan::MissionPlan_buffer)];
	char buffer[sizeof(IcdAvoidanceOutput::FatPathsAndWayPoints_buffer)];

	for (auto i=0;!std::filesystem::exists("__fin__");++i) 
	{
		try
		{
			auto n = dgs.receiveFrom(buffer, sizeof(buffer), sender);
			if (n == 0 || n < 0 || n != sizeof(buffer)) {
				std::cerr << "avoidanceProxy::receiveApath ERROR, " << n << " bytes received, continue...." << std::endl;
				log.error("avoidanceProxy: #bytes received: %d, continue....", n);
				continue;
			}
		}
		catch (const std::exception & ex) 
		{
			std::cerr << "catched exception: "<<ex.what() << std::endl;
			log.error("catched exception: %s", ex.what());
			continue;
		}
		catch (...) 
		{
			std::cerr << "catched unknown exception...." << std::endl;
			log.error("catched unknown exception....");
			continue;
		}

		IcdAvoidanceOutput::FatPathsAndWayPoints a_fatpaths;
		IcdAvoidanceOutput::read_FatPathsAndWayPoints(a_fatpaths, buffer);

#ifdef DEBUG
		std::cout << "fatpath packet #"<<i<<" received (" 
			<< (unsigned)a_fatpaths.WyPtPath.Path.NumPoints << " waypoints)"<<std::endl;
		TRACE("fatpath packet #%d received, %d waypoints", 
				i, (unsigned)a_fatpaths.WyPtPath.Path.NumPoints);
#endif
		log.trace("fatpath packet #%d received, %d waypoints",
					i, (unsigned)a_fatpaths.WyPtPath.Path.NumPoints);

		if (a_fatpaths.WyPtPath.Path.NumPoints) {
			auto aa_path = fatpaths2avoidancePath(a_fatpaths);

			auto dd = aa_path._date;
			static int ii = 0;
#ifdef DEBUG
			std::cout << "(" << ii++ << ") avoidance_path._date: " << dd << std::endl;
#endif
			log.notice("(%d) avoidance path, date: %s", ii, dd);
			int i = 0;

			for (auto &x : aa_path._waypoints) {
				//std::cout.precision(20);
				if (x._latitude_rad < 0.000001 && x._longitude_rad < 0.000001 )
					return std::make_tuple("LAT_ERROR", "LON_RRROR", "ERROR_LOW_VALUE");

				//no viene mas en radianes
				//auto lat = x._latitude_rad * 180 / M_PI;
				//auto lon = x._longitude_rad * 180 / M_PI;

				auto lat = x._latitude_rad;
				auto lon = x._longitude_rad;
				x._latitude_rad *= (M_PI / 180);
				x._longitude_rad *= (M_PI / 180);

				auto alt = x._altitude_m;
				auto grSpd = x._ground_speed_mps;
								
				if (i++ == 0)
				{
#ifdef DEBUG
					std::cout << "\tWaypoint 0, lat(deg): "
						<< lat << ", lon(deg): " << lon << ", alt(m): " 
						<< alt << ", grSpd:" << grSpd << std::endl;
#endif
				}
				log.notice("lat(deg): %f, lon(deg): %f, alt(m): %f, grSpd: %f", lat, lon, alt, grSpd);
				if (x._altitude_m < altThreshold) 
				{
					std::cerr << "\taa_path IGNORED because Low Altitude (" 
						<< aa_path._waypoints[0]._altitude_m << "m)" << std::endl;
					log.notice("aa_path IGNORED because Low Altitude (%d)",  
							aa_path._waypoints[0]._altitude_m );
					return std::make_tuple("LOW_ALT", std::to_string(aa_path._waypoints[0]._altitude_m) + "m", "IGNORED");
				}
			}

			//this is because avoidance has a bug, it sends duplicated aa_paths
			if (aa_path._waypoints[0]._latitude_rad == _prevLat0 &&
				aa_path._waypoints[0]._longitude_rad == _prevLon0 &&
				aa_path._waypoints[0]._altitude_m == _prevAlt0)
			{
				std::cerr << "\taa_path IGNORED because is DUPLICATED." << std::endl;
				log.notice("duplicated aa_path, ignoring it...");
				return std::make_tuple("DUPLICATED_AA_PATH", 
					"lat:"+std::to_string(_prevLat0)+",lon:"+std::to_string(_prevLon0), "IGNORED");
			}
			
			std::cerr << "\taa_path processed OK...." << std::endl;
			log.notice("aa_path processed OK....");
			_prevLat0 = aa_path._waypoints[0]._latitude_rad;
			_prevLon0 = aa_path._waypoints[0]._longitude_rad;
			_prevAlt0 = aa_path._waypoints[0]._altitude_m;
			
			avoidance_path_2_xml apath2xml(aa_path, xmlPar);
			std::string weatherFile, inputFile;
			apath2xml.write_input(weatherFile, inputFile, _stage);
			//KML output
			std::string fname = "avoidancePath.kml";
			apath2xml.generateKml(_googleDir+"/" + fname);
			append2KmlFile(aa_path, "ff00ffff", inputFile, inputFile);
			dgs.close();
			//const auto avoLink = "google/AvoidanceTrack.kml";
			//std::filesystem::remove(avoLink);
			//std::filesystem::create_symlink(fname, avoLink);

			//CreateSymbolicLink(avoLink, fname.c_str(), 0);
			return std::make_tuple(weatherFile, inputFile, fname);
		}
	}

	return std::make_tuple("UNEXPECTED_ERROR", "UNEXPECTED_ERROR", "ERROR");
}

auto avoidanceProxy::createKmlFile(
	const std::string &oKmlfname)  noexcept -> int
{
	if (!oKmlfname.empty())
		_oKmlFile.open(oKmlfname, std::ios::out);
	else return -1;

	_oKmlFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		<< "<kml xmlns = \"http://www.opengis.net/kml/2.2\""
		<< " xmlns:gx = \"http://www.google.com/kml/ext/2.2\">\n"
		<< "<Folder>\n";

	_pos = _oKmlFile.tellp();
	_oKmlFile << "</Folder>\n"
		<< "</kml>\n";
	_oKmlFile.flush();
	return 0;
}

auto avoidanceProxy::append2KmlFile(const avoidance_path &aa_path,
	const std::string &color,
	const std::string &name, const std::string &desc)  noexcept -> int
{
	if (_oKmlFile.is_open() == false ) return -1;
	if (_pos != 0) 	_oKmlFile.seekp(_pos);
	_oKmlFile << "<Placemark>\n"
		<< "\t<Style>\n"
		<< "\t\t<IconStyle>\n"
		<< "\t\t\t<color>6f00ffff</color>\n"
		<< "\t\t\t<Icon><href>http://earth.google.com/images/kml-icons/track-directional/track-0.png</href></Icon>\n"
		<< "\t\t</IconStyle>\n"
		<< "\t\t<LineStyle>\n"
		<< "\t\t<color>"<<color<<"</color>\n"
		<< "\t\t<gx:labelVisibility>1 </gx:labelVisibility>\n"
		<< "\t\t</LineStyle>\n"
		<< "\t\t<LabelStyle>\n"
		<< "\t\t\t<Color>6f00ffff</Color>\n"
		<< "\t\t\t<colorMode>normal</colorMode>\n"
		<< "\t\t\t<scale>0.5</scale>\n"
		<< "\t\t</LabelStyle>\n"
		<< "\t</Style>\n"
		<< "\t<name>" << name << "</name>\n"
		<< "\t<description>" << desc << "</description>\n"
		<< "\t<gx:Track>\n"
		<< "\t\t<altitudeMode>absolute</altitudeMode>\n";

	for (auto i = 0; i < aa_path._waypoints.size(); ++i) {
		auto waypoint = aa_path._waypoints[i];
		auto auxf = waypoint._latitude_rad * 180 / M_PI;
		auto lat = std::to_string(auxf);
		auxf = waypoint._longitude_rad * 180 / M_PI;
		auto lon = std::to_string(auxf);
		auto alt = std::to_string(waypoint._altitude_m);

		_oKmlFile << std::setprecision(6) << "\t\t\t<gx:coord>" << lon << " " << lat
			<< " " << alt << "</gx:coord>" << std::endl;
	}
	_oKmlFile << "\t</gx:Track>\n"
		<< "</Placemark>\n";
	_pos=_oKmlFile.tellp();
	_oKmlFile << "</Folder>\n"
		<< "</kml>\n";

	_oKmlFile.flush();

	return 0;
}

auto avoidanceProxy::closeKmlFile()  noexcept -> int
{
	if (_oKmlFile.is_open() == false) return -1;
	if (_pos == 0)
		_oKmlFile << "</Folder>\n"
		<< "</kml>\n";

	_oKmlFile.close();
	return 0;
}