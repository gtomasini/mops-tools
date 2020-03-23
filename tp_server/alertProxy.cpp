//udpReceive

#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/SocketAddress.h>
//#include <Poco/DateTimeFormatter.h>
//#include <Poco/DateTimeFormat.h>
//#include <Poco/Format.h>
#include <Poco/Logger.h>
#include <filesystem>
#include <atomic>
#include <inttypes.h>

#include "utility"
#include "remixerLib/mailslot.h"
//#include "daa_datatypes_icd/IcdAvoidanceOutput.h"
//#include "../avoidance_suscriber/avoidance_path.h"
#include "daa_datatypes_icd/alert_packageICD.h"
//#include "../avoidance_suscriber/avoidance_path_2_xml.h"
//#include "../avoidance_suscriber/avoidance_path_2_xml.cpp"

#include "alertProxy.h"
#include "additionalStuff.h"

#define DEBUG

namespace fs = std::filesystem;
extern std::atomic_bool theadEnds;//not used yet

extern Poco::Logger &logger;

//thread, never returns
void alertProxyTask(
	unsigned assessmentPort, 
	std::string avoidanceAddress, 
	unsigned avoidancePort) 
{
	auto &log = Poco::Logger::get("tpserver"); // inherits root channel
#ifdef DEBUG
	std::cout<<"alertProxyTask created, assessment port(in): "<<assessmentPort <<", avo port(out):"<< avoidancePort<<std::endl;
#endif
	log.information("alertProxyTask created, assessment port:%u, avoidance Server: %s:%u", 
		assessmentPort, avoidanceAddress, avoidancePort);

	Poco::Net::SocketAddress sa(Poco::Net::IPAddress(), assessmentPort);
	Poco::Net::DatagramSocket dgs(sa);

	std::cout << __FUNCTION__ << __LINE__ << std::endl;
	auto alertFlag=false;
	auto alertCounter = 0;
	for (;;)
	{
		Poco::Net::SocketAddress sender;
		char buffer[8000];

		try
		{
			auto n = dgs.receiveFrom(buffer, sizeof(buffer), sender);
			if (n == 0 || n < 0) {
				std::cerr << "alertProxy receive ERROR, " << n << " bytes received, continue...." << std::endl;
				log.error("alertProxy receive error: #bytes received: %d, continue....", n);
				continue;
			}
			if (n != sizeof(alert_packageICD::alert_package_buffer))
			{
				log.error("warning: unknow udp received packet, size: %u", n);
				std::cerr << "warning: unknow udp received packet, size:" << n << std::endl;
				continue;
			}
		}
		catch (const std::exception & ex)
		{
			std::cerr << "catched exception: " << ex.what() << std::endl;
			log.error("catched exception: %s", ex.what());
			continue;
		}
		catch (...)
		{
			std::cerr << "catched unknown exception...." << std::endl;
			log.error("catched unknown exception....");
			continue;
		}
		
		alert_packageICD::alert_package alert;
		//alert_packageICD::read_alert_package(alert, (alert_packageICD::alert_package_buffer &) buffer);
		alert_packageICD::read_alert_package_ptr(alert, (const char *)buffer);

#ifdef DEBUG
		std::cout << "alert packet received #" << alertCounter++
				<< ", #num_alerts: " << (unsigned)alert.num_alerts << std::endl;
#endif

		if (alert.num_alerts > 20) continue;//ignore

		//alert level
		//NONE = 0, PREVENTIVE = 1, CORRECTIVE = 2,  WARNING = 3
		bool someAlertFlag = false;
		for (auto i = 0; i < alert.num_alerts; ++i) 
		{
			auto current = alert.current_alerts[i];
#ifdef DEBUG
			std::cout << "\talert #"<< i << ", type:" << (unsigned)current.type
				<< ", alert_time: " << current.alert_time 
				<< ", modified_tau(secs): "<< current.modified_tau 
				<< ", hmd(m):"<< current.hmd<< std::endl;
#endif
			log.debug("alert #%u, type:%u, alert time: %lu, modified_tau(secs) %lu, hmd(m): %lu", 
					i, (unsigned)current.type, current.alert_time, current.modified_tau, 
					current.hmd );
			char buffer[32];
			sprintf(buffer, "ALERT_%01d_%" PRId64 "_%0.2fsecs_%0.2fm", 
				(unsigned)current.type, current.alert_time, current.modified_tau, current.hmd);
			log.notice("sending \"%s\"...", std::string(buffer));
				sendMessageToMailSlotServer(hMailslotRemixer, buffer);
				someAlertFlag = current.type > 0;
		}

		if (!someAlertFlag && alertFlag) 
		{
			//alert(s) Ends
			char buffer[32];
			sprintf(buffer, "ALERT_ENDS_%" PRId64 "", alert.current_alerts[0].alert_time);
			log.notice("sending \"%s\"...", std::string(buffer));
			sendMessageToMailSlotServer(hMailslotRemixer, buffer);
			alertFlag = false;
		}
		else if (someAlertFlag)
			alertFlag = true;

		//resend udp package to avoidance
		Poco::Net::SocketAddress avoidanceSA(avoidanceAddress, avoidancePort);
		Poco::Net::DatagramSocket dgs;
		dgs.connect(avoidanceSA);
		dgs.sendBytes(buffer, sizeof(alert_packageICD::alert_package_buffer));
	}
}

HANDLE connect2mailSlotServer(const LPCWSTR &lpname) {
	HANDLE hMailslot;
	for (;;) {
		hMailslot = CreateFileW(
			lpname,				// mailslot name 
			GENERIC_WRITE,         // mailslot write only 
			FILE_SHARE_READ,       // required for mailslots
			NULL,                  // default security attributes
			OPEN_EXISTING,         // opens existing mailslot 
			FILE_ATTRIBUTE_NORMAL, // normal attributes 
			NULL);                 // no template file 

		if (INVALID_HANDLE_VALUE == hMailslot) {
			std::cerr << "Error occurred while connecting"
				" to the (REMIXER) mailslot server: " << GetLastError() << std::endl;
		}
		else
			return hMailslot;
		Sleep(5000);
	}
}



