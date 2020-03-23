#include "additionalStuff.h"
#include <iostream>
#include <atomic>
#include <fstream>
#include <Poco/Logger.h>
#include <Poco/WindowsConsoleChannel.h>
#include <Poco/SimpleFileChannel.h>
#include "remixerLib/mailslot.h"
#include "mailslotServer.h"

//tp command (system call)
#define TRAP_CMD "%s --tri-xml %s --wm-xml %s "\
	"--oc-xml conf/operational_context.xml --up-xml conf/user_preferences.xml "\
	"--settings conf/settings.xml --output-log %s "\
	"--output-tr-kml %s --output-tr-csv %s"

std::string getYYYYMMDD_hhmmss()
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	char auxs[120];
	sprintf(auxs, "%04d%02d%02d_%02d%02d%02d",
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond);
	return std::string(auxs);
}

int processAA_path(
	std::string const &weatherFileName,
	std::string const &inputXMLfileName,
	std::string const &kmlFileName,
	std::string const &trapExec,
	std::string const &stage,
	std::string const &tpDir,
	std::string const &googleDir,
	bool disregard,
	unsigned retries
)
{
	auto &logger = Poco::Logger::get("TPserver");

#ifdef DEBUG
	std::cout << "AA_PATH OK******************************************************" << std::endl;
	std::cout << "***" << weatherFileName << ", " << inputXMLfileName << std::endl;
	TRACE("Weather xml file name : \"%s\", TP-Input xml file name : \"%s\"",
		weatherFileName, inputXMLfileName);

#endif
	logger.notice("Weather xml file name: \"%s\", Input xml file name: \"%s\"", weatherFileName, inputXMLfileName);

	//hereforth we must call tp app in order to calculate new ownship avoidance path
	auto YYYYMMDD_hhmmss(getYYYYMMDD_hhmmss());
	char tp_cmd[1024], ocsv[80], logf[80], kml[80];

	snprintf(logf, sizeof(logf), "%s/%s_tp_exec-%s.log",
		tpDir.c_str(), stage.c_str(), YYYYMMDD_hhmmss.c_str());
	snprintf(kml, sizeof(kml), "%s/%s_tp_exec_trj-%s.kml",
		googleDir.c_str(), stage.c_str(), YYYYMMDD_hhmmss.c_str());
	snprintf(ocsv, sizeof(ocsv), "%s/%s_tp_exec_trj-%s.csv",
		tpDir.c_str(), stage.c_str(), YYYYMMDD_hhmmss.c_str());
	snprintf(tp_cmd, sizeof(tp_cmd), TRAP_CMD,
		trapExec.c_str(), inputXMLfileName.c_str(), weatherFileName.c_str(), logf, kml, ocsv);

	auto mytime0 = GetTickCount();//msecs

	//log command to a text file
	auto saveCmd = [](auto tpCommand, auto Time, auto Stage) {
		std::ofstream cmdFile;
		auto tp_cmd_fname = "tp/" + Stage + "_tpCmd-" + Time + ".cmd";
		cmdFile.open(tp_cmd_fname);
		cmdFile << tpCommand;
		cmdFile.close();
	};

	saveCmd(tp_cmd, YYYYMMDD_hhmmss, stage);
	if (disregard)
	{
		logger.warning("IGNORING received avoidance path.");
#ifdef DEBUG
		std::cerr << "NOTICE: Ignoring received avoidance path " << std::endl;
#endif
		char aux[128];
		snprintf(aux, sizeof(aux), "IGNORED a-path, %s, %s",
			inputXMLfileName.c_str(), kmlFileName.c_str());
		sendMessageToMailSlotServer(hMailslotRemixer, aux);
	}
	else 	//try to Stop Remixer ("batidora")
		sendMessageToMailSlotServer(hMailslotRemixer, breakMsg);

	logger.notice("TP system call: \"%s\"", std::string(tp_cmd));

	for (auto i = 0; i < retries; ++i) //todo parametizer number 3
	{	//3 calculation tries
		auto t0 = GetTickCount();
		auto st = system(tp_cmd);//horrible system call!!!
		//auto st = 0;//just for test
		logger.notice("tp cmd status (#%d): %d", i, st);
		if (!st)
		{	//K: send generated csv file name to remixer server in order to process it
			std::cout << "TP.exe System Call Wasted Time: " << GetTickCount() - t0 << " msecs";
			char aux[256] = { "CSV_" };
			strcat_s(aux, ocsv);
			//Remixer notify, only in k instance
			if (!disregard)
			{
				std::cout << "sending \"" << aux << "\"to remixer..." << std::endl;
				logger.trace("sending \"%s\" to remixer", std::string(aux));
				sendMessageToMailSlotServer(hMailslotRemixer, aux);
			}
			break;
		}
		else if (i == (retries - 1) && !disregard)
		{	//NOK
			std::cout << "WARNING: TP.exe status < 0 (ERROR)" << std::endl;
			logger.notice("Sending ERROR message to remixer...");
			sendMessageToMailSlotServer(hMailslotRemixer, "ERROR");
		}
		else
		{
			std::cerr << "WARNING: TP.exe status <0 (ERROR), try: #" << i << std::endl;
			logger.warning("TP.exe status <0 (ERROR), try: #%d", i);
			return -1;
		}
	}
	return 0;
}

int sendMessageToMailSlotServer(std::atomic<HANDLE> &hMailSlotServer, std::string const &msg)
{
	for (int i = 0; i < 5; ++i) {
		DWORD cbBytes;
		auto bResult = WriteFile(hMailSlotServer, msg.c_str(), msg.size() + 1, &cbBytes, NULL);
		if (!bResult || (msg.size() + 1) != cbBytes) {
			std::cerr << "\nError occurred while texting " << msg
				<< " to the (REMIXER) mailslot server: " << GetLastError() << std::endl;
			hMailSlotServer = connect2mailSlotServer(lpRemixerSlotName);
			Sleep(1000);
		}
		else
			return 0;
	}
	return -1;
}

bool ElevateNow(const char *args)
{
	if (!IsRunAsAdministrator())
	{
		CHAR szPath[MAX_PATH];
		if (GetModuleFileNameA(NULL, szPath, ARRAYSIZE(szPath)))
		{
			SHELLEXECUTEINFO sei = { sizeof(sei) };

			sei.lpVerb = "runas";
			sei.lpFile = szPath;
			sei.hwnd = NULL;
			sei.nShow = SW_NORMAL;
			sei.lpParameters = args;

			if (!ShellExecuteEx(&sei))
			{
				DWORD dwError = GetLastError();
				if (dwError == ERROR_CANCELLED)
					//Annoys you to Elevate it LOL
					CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ElevateNow, 0, 0, 0);
			}
		}
		return false;
	}
	else
		std::cerr << "NOTICE: already running as admin..." << std::endl;
	return true;
}

BOOL IsRunAsAdministrator()
{
	DWORD dwError = ERROR_SUCCESS;
	PSID pAdministratorsGroup = NULL;
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	BOOL fIsRunAsAdmin = FALSE;

	if (!AllocateAndInitializeSid(
		&NtAuthority,
		2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&pAdministratorsGroup))
	{
		dwError = GetLastError();
		goto Cleanup;
	}

	if (!CheckTokenMembership(NULL, pAdministratorsGroup, &fIsRunAsAdmin))
	{
		dwError = GetLastError();
		goto Cleanup;
	}

Cleanup:

	if (pAdministratorsGroup)
	{
		FreeSid(pAdministratorsGroup);
		pAdministratorsGroup = NULL;
	}

	if (ERROR_SUCCESS != dwError)
	{
		throw dwError;
	}

	return fIsRunAsAdmin;
}

//only in mailslot mode we need this function (deprecated)
int
receiveTextFromMailSlot(HANDLE &hMailslot, std::string &str1, std::string &str2, ULONGLONG &t0) {
	auto &logger = Poco::Logger::get("TPserver");

	if (hMailslot == INVALID_HANDLE_VALUE) {
		hMailslot = CreateMailslotW(lpTPserverSlotName, // mailslot name
			BUFFER_SIZE,              // input buffer size 
			MAILSLOT_WAIT_FOREVER,    // no timeout
			NULL);                    // default security attribute 

		if (INVALID_HANDLE_VALUE == hMailslot) {
			std::cerr << "\nError occurred while"
				" creating own mailslot: " << GetLastError() << std::endl;
			logger.error("mail slot creatiorn error: %u", GetLastError());
			return -1;
		}
		else
			return 0;
	}

	char szBuffer[BUFFER_SIZE];
	DWORD cbBytes;
	auto bResult = ReadFile(hMailslot, szBuffer, sizeof(szBuffer), &cbBytes, NULL);

	if (!bResult || 0 == cbBytes) {
		std::cerr << "\nError occurred while reading input"
			" from mailSlot Client: " << GetLastError() << std::endl;
		logger.error("input read error: %u", GetLastError());
		return -1;
	}

	ULONGLONG t = GetTickCount64(), deltat = t - t0;	t0 = t;
	std::cout << "Message received from mailSlot Client: \"" << szBuffer << "\"";
	logger.trace("Message received from mailSlot Client : %s", std::string(szBuffer));
	if (deltat < 30e3 /*msecs*/) {
		std::cout << " ignoring..." << std::endl;
		logger.warning("ignoring message...");
		return -1;
	}
	else
		std::cout << std::endl;

	char *p1(szBuffer), *p2(strchr(szBuffer, ':'));
	if (p2 == NULL) {
		std::cerr << "msg format error, ignoring it..." << std::endl;
		logger.error("msg format error, ignoring it...");
		return -1;
	}
	*p2++ = 0;
	//now p1 points to first string and p2 to second!!
	str1.assign(p1);
	str2.assign(p2);
	return 0;
}


