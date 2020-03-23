#include <POCO/XML/XMLWriter.h>
#include <POCO/SAX/AttributesImpl.h>
#include <Poco/UTF8Encoding.h>
#define _USE_MATH_DEFINES 
#include <math.h>
#include <fstream>
#include <sstream>
#include <iomanip>
//#include "avoidance_suscriber/avoidance_path.h"
#include "avoidance_path_2_xml.h"

using namespace Poco::XML;

//date_and_time format "yyyy-mm-dd hh-mm-ss" to date:"yyyymmdd" and time:"hh-mm-ss"
//i.e. "2017-01-01 00:03:48" to date:"20170101" and time"00:03:48"
//return 0 if ok, -1 if nok
int avoidance_path_2_xml::grabDateAndTime(
	const char *date_and_time, //"yyyy-mm-dd hh-mm-ss" 
	std::string &date,	//"yyyymmdd" 
	std::string &time	/* "hh-mm-ss" */) {
	const char *p = date_and_time;
	if (strlen(p) < 19) return -1;

	char d[12];
	d[0] = p[0];
	d[1] = p[1];
	d[2] = p[2];
	d[3] = p[3];
	d[4] = p[5];
	d[5] = p[6];
	d[6] = p[8];
	d[7] = p[9];
	d[8] = 0;

	char t[12];
	memcpy(t, p + 11, 8);
	t[8] = 0;

	date = std::string(d);
	time = std::string(t);

	return 0;
}

const std::string avoidance_path_2_xml::getCurrentDateTime() {
	SYSTEMTIME st;
	GetSystemTime(&st);
	char aux[84];
	sprintf(aux, "%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
	_date.assign(aux);

	sprintf(aux, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
	_time.assign(aux);

	sprintf( aux, "%s_%02d%02d%02d", _date.c_str(), st.wHour, st.wMinute, st.wSecond);
	return std::string(aux);
}

/*
< ? xml version = "1.0" ? >
<em:weather_isa xmlns : em = "http://www.boeing.com/XSD/EM/v6.0" xmlns : gc = "http://www.boeing.com/XSD/GC/v6.0">
<gc:UTC_time date = "20170101" time = "00:01:51" timezone = "Europe/Madrid" / >
<em:wind_lls_1 type = "f_constant" output_unit = "[mps]" f0 = "0" / >
<em:wind_lls_2 type = "f_constant" output_unit = "[mps]" f0 = "0" / >
<em:wind_lls_3 type = "f_constant" output_unit = "[mps]" f0 = "0" / >
<em:DeltaT type = "f_null" / >
<em:Deltap type = "f_null" / >
< / em:weather_isa>

*/

int avoidance_path_2_xml::write_weather_model(
	const std::string & date, 
	const std::string & time, 
	const std::string & fnameXML)
{
	std::ofstream oFstream(fnameXML);
	//Poco::UTF8Encoding utf8encoding;
	Poco::XML::XMLWriter writer(oFstream, XMLWriter::WRITE_XML_DECLARATION |
		XMLWriter::PRETTY_PRINT);

	writer.setNewLine("\n");
	writer.startDocument();
	AttributesImpl attrs;
	//attrs.addAttribute("trio", "", "xmlns:trio", "", "http://www.boeing.com/XSD/TRIO/v6.0");
	//writer.startPrefixMapping("trio", "");
	//writer.startElement("", "trajectory_related_information", "", attrs);

	writer.startPrefixMapping("em", "em");
	attrs.addAttribute("em", "", "xmlns:em", "", "http://www.boeing.com/XSD/EM/v6.0");
	attrs.addAttribute("em", "", "xmlns:gc", "", "http://www.boeing.com/XSD/GCPT/v6.0");
		
	writer.startElement("em", "weather_isa", "", attrs);
	attrs.clear();

	writer.startPrefixMapping("gc", "gc");
	attrs.addAttribute("gc", "", "date", "", date);
	attrs.addAttribute("gc", "", "time", "", time);
	attrs.addAttribute("gc", "", "timezone", "", _timeZone);
	writer.startElement("gc", "UTC_time", "", attrs);
	writer.endElement("gc", "UTC_time", "");
	attrs.clear();

	writer.endPrefixMapping("gc");

	//<em:wind_lls_1 type = "f_constant" output_unit = "[mps]" f0 = "0" / >
	attrs.addAttribute("em", "", "type", "", "f_constant");
	attrs.addAttribute("em", "", "output_unit", "", "[mps]");
	attrs.addAttribute("em", "", "f0", "", "0");
	writer.startElement("em", "wind_lls_1", "", attrs);
	writer.endElement("em", "wind_lls_1", "");

	writer.startElement("em", "wind_lls_2", "", attrs);
	writer.endElement("em", "wind_lls_2", "");

	writer.startElement("em", "wind_lls_3", "", attrs);
	writer.endElement("em", "wind_lls_3", "");
	attrs.clear();

	//<em:DeltaT type = "f_null" / >
	attrs.addAttribute("em", "", "type", "", "f_null");
	writer.startElement("em", "DeltaT", "", attrs);
	writer.endElement("em", "DeltaT", "");

	//<em:Deltap type = "f_null" / >
	writer.startElement("em", "Deltap", "", attrs);
	writer.endElement("em", "Deltap", "");
	attrs.clear();

	writer.endElement("em", "weather_isa", "");
	writer.endPrefixMapping("em");
	writer.endDocument();
	oFstream.close();

	return 0;
}


//generates input.xml and weather.xml in order to feed tp.exe
int avoidance_path_2_xml::write_input(std::string &weatherFileName,
	std::string &inputFileName, const std::string &stage, const float tas)
{
	if (_a_path._waypoints.size() == 0) return -1;

	//Create weather model XML with same date/time----------------------------------
	std::string auxDate, auxTime;
	int st = grabDateAndTime(_a_path._date.c_str(), auxDate, auxTime);

	std::string curDate(getCurrentDateTime());
	weatherFileName =  _weatherXML + stage + "_"+  curDate + ".xml";
	inputFileName =  _inputXML + stage +"_" + curDate + ".xml";

	if (!st) {
		write_weather_model(auxDate, auxTime, weatherFileName);
	}
	else
		std::cerr<<"date/time parsing error, no weather model generated!"<<std::endl;
	//----------------------------------------------------------------------------

	std::ofstream oFstream(inputFileName);

	//Poco::UTF8Encoding utf8encoding;
	Poco::XML::XMLWriter writer(oFstream, XMLWriter::WRITE_XML_DECLARATION |
		XMLWriter::PRETTY_PRINT);

	writer.setNewLine("\n");
	writer.startDocument();

	AttributesImpl attrs;
	attrs.addAttribute("trio", "", "xmlns:trio", "", "http://www.boeing.com/XSD/TRIO/v6.0");
	attrs.addAttribute("trio", "", "xmlns:fido", "", "http://www.boeing.com/XSD/FIDO/v6.0");
	attrs.addAttribute("trio", "", "xmlns:codo", "", "http://www.boeing.com/XSD/CODO/v6.1");
	attrs.addAttribute("trio", "", "xmlns:gc", "", "http://www.boeing.com/XSD/GCPT/v6.0");
	attrs.addAttribute("trio", "", "xmlns:em", "", "http://www.boeing.com/XSD/EM/v6.0");
	attrs.addAttribute("trio", "", "xmlns:spi", "", "http://www.boeing.com/XSD/SPIGITA/v6.0");
	attrs.addAttribute("trio", "", "xmlns:aidm", "", "http://www.boeing.com/XSD/AIDM/v6.0");
	attrs.addAttribute("trio", "", "xmlns:op_solver", "", "http://www.boeing.com/XSD/OP_SOLVER/v6.0");

	writer.startPrefixMapping("trio", "");
	writer.startElement("", "trajectory_related_information", "", attrs);
	attrs.clear();
	attrs.addAttribute("", "", "gufi", "", "UNKNOWN");
	attrs.addAttribute("", "", "APM_id", "", _xmlPars._APM_id);// "C172"
	attrs.addAttribute("", "", "beacon_code", "", "UNKNOWN");
	attrs.addAttribute("", "", "flight_number", "", "UNKNOWN");
	attrs.addAttribute("", "", "aircraft_id", "", "UNKNOWN");
	writer.startElement("", "aircraft_identifier_information", "", attrs);
	attrs.clear();
	writer.endElement("", "aircraft_identifier_information", "");
	//writer.endPrefixMapping("trio");
	writer.startPrefixMapping("op_solver", "op_solver");
	writer.startElement("op_solver", "initial_conditions", "");

	//**********************************initial conditions***********************
	auto waypoint0 = _a_path._waypoints[0];

	//<gc:UTC_time date="20170101" time="00:01:51" timezone="Europe/Madrid"/>
	//writer.endPrefixMapping("op_solver");
	writer.startPrefixMapping("gc", "gc");
	attrs.addAttribute("gc", "", "date", "", auxDate );
	attrs.addAttribute("gc", "", "time", "", auxTime );
	attrs.addAttribute("gc", "", "timezone", "", _timeZone);
	writer.startElement("gc", "UTC_time", "", attrs);
	attrs.clear();
	writer.endElement("gc", "UTC_time", "");

	//<gc:t units="[sec]" value="0"/>
	//TODO: replace this value
	writer.startPrefixMapping("gc", "gc");
	attrs.addAttribute("gc", "", "units", "", "[sec]");
	attrs.addAttribute("gc", "", "value", "", "0000");
	writer.startElement("gc", "t", "", attrs);
	attrs.clear();
	writer.endElement("gc", "t", "");

	//<gc:Hp units="[m]" value="2472.84"/>
	//ALTITUDE=================================================
	writer.startPrefixMapping("gc", "gc");
	attrs.addAttribute("gc", "", "units", "", "[m]");
	attrs.addAttribute("gc", "", "value", "", 
				/*std::to_string(waypoint0._altitude_m)*/
			to_string_with_precision(waypoint0._altitude_m));
	writer.startElement("gc", "Hp", "", attrs);
	attrs.clear();
	writer.endElement("gc", "Hp", "");

	//<gc:phi units = "[deg]" value = "33.984" / >
	//LATITUDE(phi)============================================
	auto auxf = waypoint0._latitude_rad * 180 / M_PI;
	auto auxs = /*std::to_string(auxf)*/ to_string_with_precision(auxf);
	writer.startPrefixMapping("gc", "gc");
	attrs.addAttribute("gc", "", "units", "", "[deg]");
	attrs.addAttribute("gc", "", "value", "", auxs);
	writer.startElement("gc", "phi", "", attrs);
	attrs.clear();
	writer.endElement("gc", "phi", "");

	//<gc:lambda units = "[deg]" value = "-117" / >
	//LONGITUDE(lambda)=========================================
	auxf = waypoint0._longitude_rad * 180 / M_PI;
	auxs = to_string_with_precision(auxf);
	writer.startPrefixMapping("gc", "gc");
	attrs.addAttribute("gc", "", "units", "", "[deg]");
	attrs.addAttribute("gc", "", "value", "", auxs);
	writer.startElement("gc", "lambda", "", attrs);
	attrs.clear();
	writer.endElement("gc", "lambda", "");

	//<gc:d units = "[nm]" value = "0" / >
	//DISTANCE(0)===============================================
	writer.startPrefixMapping("gc", "gc");
	attrs.addAttribute("gc", "", "units", "", "[nm]");
	attrs.addAttribute("gc", "", "value", "", "0");
	writer.startElement("gc", "d", "", attrs);
	attrs.clear();
	writer.endElement("gc", "d", "");

	//<gc:m units = "[kg]" value = "980" / >
	//MASA======================================================
	writer.startPrefixMapping("gc", "gc");
	attrs.addAttribute("gc", "", "units", "", "[kg]");
	attrs.addAttribute("gc", "", "value", "", std::to_string(_xmlPars._m));//would be 980 or near
	writer.startElement("gc", "m", "", attrs);
	attrs.clear();
	writer.endElement("gc", "m", "");

	//<gc:tas units = "[mps]" value = "28" / >
	//TAS true air speed =======================================
	writer.startPrefixMapping("gc", "gc");
	attrs.addAttribute("gc", "", "units", "", "[mps]");
	//waypoint0._ground_speed_mps is zero, so we must use another thing
	//TODO, put actuall speed (TAS)
	std::string tasStr = std::to_string(tas);
	if (_xmlPars._tas > 0)
		tasStr = std::to_string(_xmlPars._tas);

	attrs.addAttribute("gc", "", "value", "", tasStr);
	writer.startElement("gc", "tas", "", attrs);
	attrs.clear();
	writer.endElement("gc", "tas", "");

	//<gc:chi units = "[deg]" value = "0" / >
	writer.startPrefixMapping("gc", "gc");
	attrs.addAttribute("gc", "", "units", "", "[deg]");
	attrs.addAttribute("gc", "", "value", "", "0");
	writer.startElement("gc", "chi", "", attrs);
	attrs.clear();
	writer.endElement("gc", "chi", "");

	//<gc:deltaHL units = "[adim]" value = "1" / >
	writer.startPrefixMapping("gc", "gc");
	attrs.addAttribute("gc", "", "units", "", "[adim]");
	attrs.addAttribute("gc", "", "value", "", "1");
	writer.startElement("gc", "deltaHL", "", attrs);
	attrs.clear();
	writer.endElement("gc", "deltaHL", "");

	//<gc:deltaLG units = "[adim]" value = "0" / >
	writer.startPrefixMapping("gc", "gc");
	attrs.addAttribute("gc", "", "units", "", "[adim]");
	attrs.addAttribute("gc", "", "value", "", "0");
	writer.startElement("gc", "deltaLG", "", attrs);
	attrs.clear();
	writer.endElement("gc", "deltaLG", "");

	//<gc:deltaSB units = "[adim]" value = "0" / >
	writer.startPrefixMapping("gc", "gc");
	attrs.addAttribute("gc", "", "units", "", "[adim]");
	attrs.addAttribute("gc", "", "value", "", "0");
	writer.startElement("gc", "deltaSB", "", attrs);
	attrs.clear();
	writer.endElement("gc", "deltaSB", "");

	writer.endPrefixMapping("gc");
	//"initial conditions" close
	writer.endElement("op_solver", "initial_conditions", "");
	writer.endPrefixMapping("op_solver");

	//****************flight intent***************************************

	writer.startPrefixMapping("fido", "fido");
	//<fido:flight_intent>
	writer.startElement("fido", "flight_intent", "");
	//<fido:flight_segments>
	writer.startElement("fido", "flight_segments", "");

	//**************** segment *******************************
	for(auto i=1; i < _a_path._waypoints.size(); ++i)
	{
		auto waypoint=_a_path._waypoints[i];
		//<fido:flight_segment id="0" FS_Code="11111">
		//TODO replace 0
		auto auxs = std::to_string(i - 1);
		writer.startPrefixMapping("fido", "fido");
		attrs.addAttribute("fido", "", "FS_Code", "", "11111");
		attrs.addAttribute("fido", "", "id", "", auxs);
		writer.startElement("fido", "flight_segment", "", attrs);
		attrs.clear();

		//<codo:composite description = "Null" code = "000000">
		writer.startPrefixMapping("codo", "codo");
		attrs.addAttribute("codo", "", "description", "", "Null");
		attrs.addAttribute("codo", "", "code", "", "000000");
		writer.startElement("codo", "composite", "", attrs);
		attrs.clear();

		//<codo:user_defined code="000000">
		writer.startPrefixMapping("codo", "codo");
		attrs.addAttribute("codo", "", "code", "", "000000");
		writer.startElement("codo", "user_defined", "", attrs);
		attrs.clear();

		//<codo:parameter name = "template_name" value = "Null" / >
		writer.startPrefixMapping("codo", "codo");
		attrs.addAttribute("codo", "", "name", "", "template_name");
		attrs.addAttribute("codo", "", "value", "", "Null");
		writer.startElement("codo", "parameter", "", attrs);
		attrs.clear();
		writer.endElement("codo", "parameter", "");

		//<codo:twaypoint id="1">
		auxs = std::to_string(i);
		writer.startPrefixMapping("codo", "codo");
		attrs.addAttribute("codo", "", "id", "", auxs);
		writer.startElement("codo", "twaypoint", "", attrs);
		attrs.clear();

		//<spi:point name = "WPT1">
		//TODO: WPTi
		auto wptis = "WPT" + std::to_string(i);//i.e. WPT1, WPT2...
		writer.startPrefixMapping("spi", "spi");
		attrs.addAttribute("spi", "", "name", "", wptis);
		writer.startElement("spi", "point", "", attrs);
		attrs.clear();

		//<em:point name = "WPT1" ellipsoid = "wgs84" type = "longitude_and_latitude">
		writer.startPrefixMapping("em", "em");
		attrs.addAttribute("em", "", "name", "", wptis);//i.e. WPT1, WPT2...
		attrs.addAttribute("em", "", "ellipsoid", "", "wgs84");
		attrs.addAttribute("em", "", "type", "", "longitude_and_latitude");
		writer.startElement("em", "point", "", attrs);
		attrs.clear();

		//<gc:longitude units = "[deg]" value = "-116.999" / >
		//LONGITUDE===================================================
		auxf = waypoint._longitude_rad * 180 / M_PI;
		auxs = to_string_with_precision(auxf);
		writer.startPrefixMapping("gc", "gc");
		attrs.addAttribute("gc", "", "units", "", "[deg]");
		attrs.addAttribute("gc", "", "value", "", auxs);
		writer.startElement("gc", "longitude", "", attrs);
		writer.endElement("gc", "longitude", "");
		attrs.clear();

		//<gc:latitude units = "[deg]" value = "33.9938" / >
		//LATITUDE====================================================
		auxf = waypoint._latitude_rad * 180 / M_PI;
		auxs = to_string_with_precision(auxf);
		writer.startPrefixMapping("gc", "gc");
		attrs.addAttribute("gc", "", "units", "", "[deg]");
		attrs.addAttribute("gc", "", "value", "", auxs);
		writer.startElement("gc", "latitude", "", attrs);
		writer.endElement("gc", "latitude", "");
		attrs.clear();
		writer.endPrefixMapping("gc");

		writer.endElement("em", "point", "");
		writer.endPrefixMapping("em");
		writer.endElement("spi", "point", "");
		writer.endPrefixMapping("spi");
		writer.endElement("codo", "twaypoint", "");

		writer.endElement("codo", "user_defined", "");
		writer.endElement("codo", "composite", "");
		writer.endPrefixMapping("codo");
		writer.endElement("fido", "flight_segment", "");
	}
	writer.endElement("fido", "flight_segments", "");
	//"flight intent" close
	writer.startElement("fido", "constraints", "");
	writer.endElement("fido", "constraints", "");
	writer.startElement("fido", "objectives", "");
	writer.endElement("fido", "objectives", "");


	writer.endElement("fido", "flight_intent", "");
	writer.endPrefixMapping("fido");
	writer.endElement("", "trajectory_related_information", "");
	writer.endPrefixMapping("trio");

	writer.endDocument();
	
	oFstream.close();
	return 0;
}

auto avoidance_path_2_xml::generateKml(
	const std::string &oKmlfname,
	const std::string &color)  noexcept -> int
{
	std::ofstream oFile;	//output kml file

	if (!oKmlfname.empty())
		oFile.open(oKmlfname, std::ios::out);
	else return -1;

	oFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		<< "<kml xmlns = \"http://www.opengis.net/kml/2.2\""
		<< " xmlns:gx = \"http://www.google.com/kml/ext/2.2\">\n"
		<< "<Folder>\n"
		<< "\t<Placemark>\n"
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
		<< "\t<name>"<<oKmlfname<<"</name>\n"
		<< "\t<description>tp generated avoidance path(7 waypoints)</description>\n"
		<< "\t\t<gx:Track>\n"
			<< "\t\t\t<altitudeMode>absolute</altitudeMode>\n";

	for (auto i = 0; i < _a_path._waypoints.size(); ++i){
		auto waypoint = _a_path._waypoints[i];
		auto auxf = waypoint._latitude_rad * 180 / M_PI;
		auto lat = to_string_with_precision(auxf);
		auxf = waypoint._longitude_rad * 180 / M_PI;
		auto lon = to_string_with_precision(auxf);
		auto alt = to_string_with_precision(waypoint._altitude_m);

		oFile << std::setprecision(12) << "\t\t\t<gx:coord>" << lon << " " << lat
				<< " " << alt << "</gx:coord>" << std::endl;
	}

	oFile << "\t\t</gx:Track>\n"
		<<"\t</Placemark>\n"
	<<"</Folder>\n</kml>\n";

	oFile.close();
	return 0;
}

