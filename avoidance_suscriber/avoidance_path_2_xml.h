#pragma once
#include <string>
#include <fileapi.h>
#include "avoidance_suscriber/avoidance_path.h"

struct xmlParameters {
	std::string	_APM_id = "C172";
	unsigned _m = 980;//kg
	unsigned _tas = 28;//mps
	std::string _stage = "H2";
};

class avoidance_path_2_xml {
	avoidance_path	_a_path;
	std::string _inputXML;
	std::string _weatherXML;
	std::string _outFolder;
	std::string _timeZone;
	std::string _date;
	std::string _time;
	xmlParameters _xmlPars;

	int write_weather_model(const std::string &date, 
		const std::string &time, 
		const std::string &fnameXML);

	//format "yyyy-mm-dd hh-mm-ss" to date:"yyyymmdd" and time:"hh-mm-ss"
	//i.e. "2017-01-01 00:03:48" to date:"20170101" and time"00:03:48"
	//return 0 if ok, -1 if nok
	int grabDateAndTime(const char *date_and_time, std::string &date, std::string &time);
	const std::string getCurrentDateTime();
	const std::string getTime() { return _time; };
	const std::string getDate() { return _date; };
	template <typename T>
	std::string to_string_with_precision(const T a_value, const int n = 16)
	{
		std::ostringstream out;	out.precision(n);
		out << std::fixed << a_value;
		return out.str();
	}

public: 
	//generates tp input/weather xml files in order to feed tp.exe
	avoidance_path_2_xml(const avoidance_path &aa_path,
		const xmlParameters &pars, 
		const std::string &ofile="input_",
		const std::string &weatherXML="weatherModel_",
		const std::string &outFolder="tp",
		const std::string &timeZone = "Europe/Madrid")
		:_a_path(aa_path), _xmlPars(pars), _outFolder(outFolder),
		_timeZone(timeZone){
		CreateDirectoryA(_outFolder.c_str(), NULL);
		_inputXML = _outFolder + "\\" + ofile;
		_weatherXML = _outFolder + "\\" + weatherXML;
	}

	//return weatherFileName and inputFileName
	int
	write_input(std::string &weatherFileName, std::string &inputFileName, 
		std::string const &stage="XX", const float tas = 40);

	//color default=orange
	auto generateKml(const std::string & oKmlfname, const std::string &color = "ff0080ff") noexcept -> int;

};