/*
WARNING:


Start Visual Studio. Go to Tools->Options and expand Projects and solutions. Select VC++ Directories from the tree and choose Include Files from the combo on the right.

You should see:

	$(WindowsSdkDir)\include

*/
#include <iostream>
#include <windows.h>	//WARNING, this has conflicts with poco headeers
#include <string>
#include <exception>
#include <filesystem>
#include <thread>
#include <Poco/Logger.h>
#include <Poco/Util/IniFileConfiguration.h>
#include <Poco/WindowsConsoleChannel.h>
#include <Poco/SimpleFileChannel.h>


#include "../avoidance_suscriber/avoidance_path_2_xml.h"
#include "alertProxy.h"
#include "avoidanceProxy.h"
#include "trackLib/trace.h"
#include "additionalStuff.h"
#include "remixerLib/mailslot.h"
#include "mailslotServer.h"

using Poco::AutoPtr;
using Poco::Util::IniFileConfiguration;

#define DEBUG

int main(int argc, char* argv[])
{
	std::string inifile("tp_server.ini");
	std::string test("true");

	if (argc == 1) {
		std::cout << "you can write: \"" << argv[0] << " stage-nake k [true|false] configurationFile\"" << std::endl;
		std::cout << "\twhere k is #apath to take in account, default is 1";
		std::cout << "\tdefault stage: XX";
		std::cout << "\ttest mode default: true" << std::endl;
		std::cout << "\tdefault configuration file (.ini) is " << inifile << std::endl;
		std::cout << "\ttest mode is "<<test << std::endl;
	}

//	if (IsRunAsAdministrator() == FALSE )
//		ElevateNow();
	auto k_instance = 0;//number of received avoid paths in order to consider it
	auto testMode = true;
	std::string stage = "XX";
	stage = argc < 2 ? stage : argv[1];
	k_instance = argc < 3 ? k_instance : atoi(argv[2]);
	test = argc < 4 ? test : argv[3];
	if (test[0] == 'f' || test[0] == 'F') testMode = false;

	if (testMode) strcpy(breakMsg, "STOP");//text message to remixer

	inifile = argc < 5 ? inifile : argv[4];//config
	if (!std::filesystem::exists(inifile))
	{
		std::cerr << inifile << " doesn't exist" << std::endl;
		return -1;
	}

	AutoPtr<IniFileConfiguration> pConf(new IniFileConfiguration(inifile));
	auto loglevel = pConf->getString("logger.level", "trace");
	auto logfile = pConf->getString("logger.file", argv[0]);
	const auto tp_retries = pConf->getUInt("tp.retries", 3);

	std::cout << "TPserver, .ini :"<<inifile<<", stage: " << stage << ", #k-instances: "<< k_instance << std::endl;
	TRACE("TPserver, .ini:%s, stage:%s, #k-instances:%d", inifile.c_str(), stage.c_str(), k_instance);
	Sleep(500);

	char args[256];
	for (int i = 1; i < argc; ++i)
	{
		strcat(args, argv[i]);
		strcat(args, " ");
	}

	if (IsRunAsAdministrator() == FALSE &&
		MessageBox(0, "Need To Elevate", "Filesystem access permition error", 
			MB_ICONQUESTION |MB_SYSTEMMODAL | MB_ICONERROR | MB_YESNO) == IDYES)
	{
		if (ElevateNow(args) == false) return 0;
	}

	//if (std::filesystem::exists("__fin__")) std::cout << "hidden file exists" << std::endl;
	remove("__fin__");
	AutoPtr<Poco::SimpleFileChannel> pChannel(new Poco::SimpleFileChannel);
	pChannel->setProperty("path", "out/"+stage+"_"+logfile);
	pChannel->setProperty("rotation", "100 K");
	Poco::Logger::root().setChannel(pChannel);
	auto &logger = Poco::Logger::get("TPserver");;
	logger.setLevel(loglevel);

	try {
		auto trap_exec = pConf->getString("tp.trap_executable", "trap_exec.exe");
		//in udp mode avoidance data is received directly from daa-avoidance "box" by udp port 10000
		//in mailslot mode (deprecated) avoidance data is received from avoidance_suscriber(dds) app by mailslot 
		auto mode		= pConf->getString("assessment.mode", "udp");
		auto avoOutPort = pConf->getUInt("avoidance.outPort", 10000);
		auto avoInPort	= pConf->getUInt("avoidance.inPort", 4000);
		auto assOutPort = pConf->getUInt("assessment.outPort", 4001);
		auto avoAddress = pConf->getString("avoidance.address", "127.0.0.1");
		auto assAddress = pConf->getString("assessment.address", "0.0.0.0");
		auto googleDir	= pConf->getString("common.googledir", "google");

		auto tpDir = pConf->getString("common.tpdir", "tp");
		auto outDir = pConf->getString("common.outdir", "out");

		//TODO: put this stuff in some class
		xmlParameters xmlPar;
		xmlPar._APM_id = pConf->getString("tp_xml.APM_id", "C208");
		xmlPar._m = pConf->getUInt("tp_xml.m", 2150);
		xmlPar._tas = pConf->getUInt("tp_xml.tas", 35);
		xmlPar._stage = pConf->getString("`tp_xml.stage", "Xx");

		logger.notice("Stage: %s, Test Mode: %b, Operation Mode: %s, breakMsg: %s, "\
				"avoidance out port: %u, assesment out port: %u, k(instance): %u"\
				"tas: %u", 
				stage, testMode, mode, std::string(breakMsg), avoOutPort, assOutPort, k_instance, xmlPar._tas );

		std::cout << "NOTICE, from .ini: tas: " << xmlPar._tas << ", weight: " << xmlPar._m << std::endl;

		std::filesystem::create_directory(tpDir);
		std::filesystem::create_directory(outDir);
		std::filesystem::create_directory(googleDir);

		//here connects to (remixer) mailSlot Server
		hMailslotRemixer = connect2mailSlotServer(lpRemixerSlotName);

		logger.notice("Waiting for mailslot client connection...");

		//TODO, pass address & port as parameters (grab from conf .ini)
		std::thread alertTask(alertProxyTask, assOutPort, avoAddress, avoInPort);
		Sleep(500);

		HANDLE hMailslot(INVALID_HANDLE_VALUE);//WARNING: this value would be at the beginning
		ULONGLONG t0(0);
		avoidanceProxy avoProxy(avoOutPort, stage);
		avoProxy.setGoogleDir(googleDir);

		std::string kmlFname = stage +"_avoidancePaths_"+getYYYYMMDD_hhmmss()+".kml";
		avoProxy.createKmlFile("google/" + kmlFname);
		const auto avoLink = "google/avoidancePaths.kml";
		std::filesystem::remove(avoLink);
		//would throw exception if not in admin mode
		std::filesystem::create_symlink(kmlFname, avoLink);
		
		for (auto ii = 0;; )
		{
			std::string weatherFileName("weatherFileNameERROR");
			std::string	inputXMLfileName("inputFileNameERROR");
			std::string kmlFileName("kmlFileNameERROR");

			//indirect conexion, in order of testing is soo complicated*******************************
			if (mode.compare("udp")) 
			{	//deprecated mode trough dds bridge
				//receives data from mailSlotServer (avoidance_suscriber)(dds data)
				std::cerr << "WARNING: deprecated DDS mode." << std::endl;
				logger.warning(" deprecated DDS mode");
				if (receiveTextFromMailSlot(hMailslot, weatherFileName, inputXMLfileName, t0))
					continue;
			}
			else//************************************************************************************
			{	//UDP Mode, direct conexion to avoidance "daa-box"
				auto[weatherFilename, inputXMLfilename, KMLfilename] = avoProxy.receiveApath(xmlPar);
				if (weatherFilename == "LAT_ERROR" || inputXMLfilename == "LON_ERROR") 
				{
					logger.error("lat/lon avoidance values error (0) aborting");
					sendMessageToMailSlotServer(hMailslotRemixer, "ERROR_AVOIDANCE_LAT/LON_ZEROED");
					continue;
				}
				else if (weatherFilename == "UNEXPECTED_ERROR")
				{
					logger.error("unexpected error ignoring it");
					sendMessageToMailSlotServer(hMailslotRemixer, "ERROR_UNEXPECTED");
					continue;
				}
				else if (KMLfilename == "IGNORED")
				{
					logger.notice(weatherFilename+" "+inputXMLfilename);
					if (weatherFilename=="LOW_ALT")
						sendMessageToMailSlotServer(hMailslotRemixer, "NOTICE_LOW_ALTITUDE_AVOIDANCEPATH");
					continue;
				}
				
				weatherFileName = weatherFilename;
				inputXMLfileName = inputXMLfilename;
				kmlFileName = KMLfilename;
				++ii;
			}

			processAA_path(weatherFileName,
				inputXMLfileName,
				kmlFileName,
				trap_exec,
				stage,
				tpDir,
				googleDir,
				ii != k_instance,
				tp_retries);

			Sleep(10000);//ignore apaths in within 10 secs
			continue;
		}//for
		//theadEnds = true;
		for (; !std::filesystem::exists("__fin__");)
			Sleep(100);

		std::cout << "bye..." << std::endl;
		CloseHandle(hMailslot);//own mail slot
		exit(0);
				
		return 0;
	}
	catch (const std::exception& e) {
		logger.error("exception: %s", e.what());
	}
	catch (const std::string& e) {
		logger.error("exception: %s", e);
	}
	catch (...) {
		logger.error("unknown exception...");
	}
	return -1;
}

