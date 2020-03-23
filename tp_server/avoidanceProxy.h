#pragma once
#include <string>
#include <tuple>
#include <fstream>
#include <Poco/Logger.h>


#include "../avoidance_suscriber/avoidance_path_2_xml.h"
#include "daa_datatypes_icd/IcdAvoidanceOutput.h"

# define M_PI           3.14159265358979323846 

class avoidanceProxy {
	unsigned _port=10000;//
	std::string _stage = "XX";
	double _prevLat0 = 0;
	double _prevLon0 = 0;
	double _prevAlt0 = 0;
	const double altThreshold = 1200;//meters
	std::ofstream _oKmlFile;	//output kml file
	long	_pos=0;
	std::string _googleDir = "google";

	//returns when avoidance path is received
	avoidance_path
	fatpaths2avoidancePath(IcdAvoidanceOutput::FatPathsAndWayPoints const& a_fatpaths);

public:
	avoidanceProxy(unsigned port, const std::string &stage);
	void setGoogleDir(const std::string &dir) {
		_googleDir = dir;
	}
	std::tuple<std::string, std::string, std::string>
		receiveApath(xmlParameters const &xmlPar);
	
	auto createKmlFile(const std::string & oKmlfname) noexcept -> int;
	auto append2KmlFile(const avoidance_path &aa_path, const std::string &color,
		const std::string &name, const std::string &desc) noexcept -> int;
	auto closeKmlFile() noexcept -> int;

	virtual ~avoidanceProxy() {
		closeKmlFile();
	}
	//static Poco::Logger &_logger;
};