//main routine remixer file
/*
WARNING:


Start Visual Studio. Go to Tools->Options and expand Projects and solutions. Select VC++ Directories from the tree and choose Include Files from the combo on the right.

You should see:

	$(WindowsSdkDir)\include

*/
#include <Poco/Util/Application.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/AbstractConfiguration.h>
#include <Poco/Util/IniFileConfiguration.h>
#include <Poco/AutoPtr.h>

#include <iostream>
#include <sstream>
#include <filesystem>
#include <exception>

#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Ws2_32.lib")

#include "remixerLib\parseCsv.h"
#include "remixerLib\mops2track.h"

using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::Util::OptionCallback;
using Poco::AutoPtr;
using Poco::Util::IniFileConfiguration;

using namespace remixerLib;

class RemixerMain : public Application {

public:
	RemixerMain() : _helpRequested(false) 
	{
	}

protected:
	void initialize(Application& self)
	{
		loadConfiguration(); // load default configuration files, if present
		Application::initialize(self);
		// add your own initialization code here
	}

	void getConf()
	{
		auto inifile = findProperties("inifile", "remixer.ini");
		if (!std::filesystem::exists(inifile))
			std::cerr << inifile << "ERROR: doesn't exist" << std::endl;
		AutoPtr<IniFileConfiguration> pConf(new IniFileConfiguration(inifile));

		//Remixer mode, begins-------------------------------------------------------
		_config.loglevel = findProperties("level", pConf->getString("logger.level", "notice"));
		_config.udpServer = findProperties("server", pConf->getString("sensors.ip_address", "127.0.0.1"));

		_config.adsbPort = stoi(findProperties("adsbport", pConf->getString("sensors.adsb_track_port", "2200")));
		_config.radarPort = stoi(findProperties("radarport", pConf->getString("sensors.radar_track_port", "2201")));
		_config.ownshipPort = stoi(findProperties("ownshipport", pConf->getString("sensors.ownship_port", "2100")));
		_config.truthPort = stoi(findProperties("truthport", pConf->getString("sensors.clean_track_port", "2300")));

		_config.adsbCsv = findProperties("adsbCSV", pConf->getString("sensors.adsb_csv_int_file", ""));

		_config.radarCsv = findProperties("radarCSV", pConf->getString("sensors.radar_csv_int_file", ""));
		_config.truthCsv = findProperties("truthCSV", pConf->getString("sensors.truth_csv_int_file", ""));
		_config.ownshipCsv = findProperties("ownshipCSV", pConf->getString("sensors.truth_csv_int_file", ""));
		_config.callsign = findProperties("callsign", pConf->getString("common.ownship_id", "XX_INTR"));
		_config.alertsDir = pConf->getString("common.alertsDir", "alerts");
		_config.googleDir = pConf->getString("common.googleDir", "google");

		auto stage = [](auto &callsign) {
			auto pos = callsign.find_first_of('_');
			std::string stage = "XX";
			if (pos != std::string::npos)
				stage = callsign.substr(0, pos);
			return stage;
		};

		_config.stage = findProperties("stage", stage(_config.callsign));
		//kml
		_config.KMLintruderColor = pConf->getString("kml.intruderColor", KML_GREEN_COLOR);
		_config.KMLownshipColor = pConf->getString("kml.ownshipColor", KML_BLACK_COLOR);
		_config.KMLownshipAvoidanceColor = pConf->getString("kml.ownshipAvoidanceColor", KML_WHITE_COLOR);
		_config.KMLownshipAlert1Color = pConf->getString("kml.ownshipAlert1Color", KML_YELLOW_COLOR);//yellow
		_config.KMLownshipAlert2Color = pConf->getString("kml.ownshipAlert2Color", KML_ORANGE_COLOR);//orange
		_config.KMLownshipAlert3Color = pConf->getString("kml.ownshipAlert3Color", KML_RED_COLOR);//red
		_config.KMLopacity = pConf->getString("kml.opacity", KML_OPACITY0);
		_config.KMLownshipIcon = pConf->getString("kml.ownshipIcon", KML_ARROW_ICON);
		_config.KMLintruderIcon = pConf->getString("kml.intruderIcon", KML_ARROW_ICON);
		_config.KMLlineWidth = pConf->getString("kml.linewidth", "1");
		//bin output file, not default value
		_config.obinfname = findProperties("binfile", pConf->getString("common.out_bin_file", ""));
		_config.logFile = pConf->getString("logger.file", "");
		_config.outdir = pConf->getString("common.out", "out");
		_config.minAlt = pConf->getUInt("common.minAltitude", 100);
		_config.startIndex = pConf->getUInt("common.startIndex", 0);

		auto auxs = findProperties("toa_offset", "1483228800");//2017-01-01

		auto string2val = [](const auto &str, auto &value2set)
		{
			value2set = 0;
			std::stringstream ss(str);
			ss >> value2set;
		};

		string2val(pConf->getString("common.toa_offset", auxs), _config.toaOffset);
		_config.noSleep = pConf->getBool("simtime.nosleep", false);

		//only 4 noremixer mode
		string2val(findProperties("sleep", "1000"), _config.delay);

		_config.rewindSize = pConf->getUInt("common.rewindSize", 130);
		_config.substageSize = pConf->getUInt("common.substageSize", 190);
	}

	//Main Routine**********************************************************************
	int main(const ArgVec& args) {
		if (_helpRequested) return Application::EXIT_OK;

		getConf();

		//only 4 noremixer mode
		auto ifname = findProperties("csvFile");//input file name, not default value

		char timebuf[128];
		if (struct tm gmt; _gmtime64_s(&gmt, &_config.toaOffset) ||
			asctime_s(timebuf, sizeof(timebuf), &gmt))
		{
			std::cerr << "_gmtime64_s/asctime_s failed due to an invalid argument.\n";
			return -1;
		}

		std::filesystem::create_directory(_config.googleDir);
		std::filesystem::create_directory(_config.outdir);
		std::filesystem::create_directory(_config.alertsDir);

		if (!_config.adsbCsv.empty() || !_config.radarCsv.empty() ||
			!_config.truthCsv.empty() || !_config.ownshipCsv.empty()) {
			//Remixer ("batidora" ;-) MODE 

			_config.csvFilesTuple = { _config.truthCsv, _config.adsbCsv, _config.radarCsv, _config.ownshipCsv };
			_config.portsTuple = { _config.truthPort, _config.adsbPort, _config.radarPort, _config.ownshipPort };

			std::cout << "Remixer Mode( " << _config.adsbCsv << "," << _config.radarCsv
				<< "," << _config.truthCsv << "," << _config.ownshipCsv << ")\n"
				<< "TOA offset: " << _config.toaOffset << "msecs, " << timebuf
				<< "Callsign: " << _config.callsign << ", truth port: " << _config.truthPort
				<< ", Stage: " << _config.stage
				<< ", noSleep: " << _config.noSleep
				<< std::endl;

			Sleep(1000);//In order to see printouts

			mops2track::configure(_config);
			mops2track::remixer(_config.csvFilesTuple, std::chrono::seconds(_config.toaOffset),
				_config.logFile, _config.obinfname);
		}
		else if (!ifname.empty()) 
		{	//STANDALONE MODE (deprecated)
			std::cerr << "WARNNG: Deprecated mode" << std::endl;
			std::string source = findProperties("src", "unknown");
			auto port = _config.truthPort;
			auto  src = source_data::unknown;
			if (source == "adsb") {
				src = source_data::adsb;
				port = _config.adsbPort;
			}
			else if (source == "camera") {
				src = source_data::camera;
				port = _config.radarPort;
			}
			else if (source == "radar") {
				src = source_data::radar;
				port = _config.radarPort;
			}
			std::cout << "Standalone Mode(" << ifname << "), src: " << source << " port:"
				<< port << ", outdir:" << _config.outdir << std::endl;
#ifdef DEBUG
			Sleep(900);
#endif
			mops2track::processIntruderCsvFile(ifname, _config.obinfname, _config.delay,
				_config.udpServer, port, src, _config.callsign, _config.loglevel);
		}
		else 
		{
			std::cout << "NOTICE: Help way..." << std::endl;
			displayHelp();
			return Application::EXIT_NOINPUT;
		}

		return Application::EXIT_OK;
	}

	void uninitialize() {
		// add your own uninitialization code here
		Application::uninitialize();
	}

	void reinitialize(Application& self) {
		Application::reinitialize(self);
		// add your own reinitialization code here
	}

	void defineOptions(OptionSet& options) {
		Application::defineOptions(options);

		options.addOption(
			Option("help", "h", "display help information on command line arguments")
			.required(false)
			.repeatable(false)
			.callback(OptionCallback<RemixerMain>(this, &RemixerMain::handleHelp)));

		options.addOption(
			Option("define", "D", "define a configuration property")
			.required(false)
			.repeatable(true)
			.argument("name=value")
			.callback(OptionCallback<RemixerMain>(this, &RemixerMain::handleDefine)));

		options.addOption(
			Option("inifile", "f", "load configuration data from a .ini file")
			.required(false)
			.repeatable(false)
			.argument("inifile")
			.callback(OptionCallback<RemixerMain>(this, &RemixerMain::handleConfig)));

		options.addOption(
			Option("csv", "a", "cvs name, input file to parse and process, no default value."
				" Only in stand-alone mode,")
			.required(false)
			.repeatable(false)
			.argument("value")
			.binding("csvFile"));

		options.addOption(
			Option("server", "a", "udp server destination address, default is 127.0.0.1 ."
				" Could be a hostname.")
			.required(false)
			.repeatable(false)
			.argument("value")
			.binding("server"));

		options.addOption(
			Option("adsbport", "a", "adsb udp server destination port, default is 2200")
			.required(false)
			.repeatable(false)
			.argument("value")
			.binding("adsbport"));

		options.addOption(
			Option("radarport", "a", "radar udp server destination port, default is 2200")
			.required(false)
			.repeatable(false)
			.argument("value")
			.binding("radarport"));

		options.addOption(
			Option("truthport", "a", "truth udp server destination port, default is 2300")
			.required(false)
			.repeatable(false)
			.argument("value")
			.binding("truthport"));

		options.addOption(
			Option("ownshipport", "a", "ownship udp server destination port, default is 2100")
			.required(false)
			.repeatable(false)
			.argument("value")
			.binding("ownshipport"));

		options.addOption(
			Option("sleep", "a", "in noremixer mode is the time in msecs between two consecutive rows, default 1000msec")
			.required(false)
			.repeatable(false)
			.argument("value")
			.binding("sleep"));

		options.addOption(
			Option("binfile", "a", "output binary file name, no default value."
				" If it not set or set null, the output file will not be created."
				" In this case data will be sent to udp server")
			.required(false)
			.repeatable(false)
			.argument("value")
			.binding("binfile"));

		options.addOption(
			Option("src", "a", "intruder source data: adsb, radar, camera, fused, unknow, default is unknow."
				" Only in stand-alone mode.")
			.required(false)
			.repeatable(false)
			.argument("value")
			.binding("src"));

		options.addOption(
			Option("callsign", "a", "intruder callsign: trackid=hash(callsign), default is H2_INTR.")
			.required(false)
			.repeatable(false)
			.argument("value")
			.binding("callsign"));

		options.addOption(
			Option("adsbCSV", "a", "intruder adsb csv file name: to feed Remixer")
			.required(false)
			.repeatable(false)
			.argument("value")
			.binding("adsbCSV"));

		options.addOption(
			Option("radarCSV", "a", "intruder radar csv file name: to feed Remixer")
			.required(false)
			.repeatable(false)
			.argument("value")
			.binding("radarCSV"));

		options.addOption(
			Option("truthCSV", "a", "intruder truth csv file name: to feed Remixer")
			.required(false)
			.repeatable(false)
			.argument("value")
			.binding("truthCSV"));

		options.addOption(
			Option("ownshipCSV", "a", "ownship csv file name: to feed Remixer")
			.required(false)
			.repeatable(false)
			.argument("value")
			.binding("ownshipCSV"));

		options.addOption(
			Option("toa_offset", "a", "Toa Offset in seconds")
			.required(false)
			.repeatable(false)
			.argument("value")
			.binding("toa_offset"));

		options.addOption(
			Option("stage", "a", "scenary: H1, C1, etc. Default is XX")
			.required(false)
			.repeatable(false)
			.argument("value")
			.binding("stage"));

		options.addOption(
			Option("level", "a", "log level: none, fatal, critical, error, warning, "
				"notice, information, debug, trace. Default value: notice")
			.required(false)
			.repeatable(false)
			.argument("value")
			.binding("level"));
	}

	void handleHelp(const std::string& name, const std::string& value) {
		_helpRequested = true;
		displayHelp();
		stopOptionsProcessing();
	}

	void handleDefine(const std::string& name, const std::string& value) {
		defineProperty(value);
	}

	void handleConfig(const std::string& name, const std::string& value) {
		loadConfiguration(value);
	}

	void displayHelp() {
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS\n"
			"should be: /csv=filename (standalone mode) /src=adsb /sleep=1000\n"
			"or: /adsbcsv=file1 /radarcsv=file2 /truthcsv=file3 /ownshipCsv=file4 (Remixer mode)\n"
		);
		helpFormatter.setHeader("options:");
		helpFormatter.format(std::cout);
	}

	void defineProperty(const std::string& def) {
		std::string name, value;
		
		if (auto pos = def.find('='); pos != std::string::npos) {
			name.assign(def, 0, pos);
			value.assign(def, pos + 1, def.length() - pos);
		}
		else name = def;
		config().setString(name, value);
	}
	
	std::string
	findProperties(const std::string& base, const std::string &retVal="") {
		AbstractConfiguration::Keys keys;
		config().keys(base, keys);
		if (keys.empty()) {
			if (config().hasProperty(base)) {
				return config().getString(base);
			}
		}
		else {
			for (AbstractConfiguration::Keys::const_iterator it = keys.begin(); it != keys.end(); ++it) {
				std::string fullKey = base;
				if (!fullKey.empty()) fullKey += '.';
				fullKey.append(*it);
				return findProperties(fullKey);
			}
		}
		return retVal;
	}

	bool _helpRequested;
	remixerLib::Configuration _config;
};//Remixer class

POCO_APP_MAIN(RemixerMain)
