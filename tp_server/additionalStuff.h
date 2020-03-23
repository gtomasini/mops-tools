#include <windows.h>	//WARNING, this has conflicts with poco headeers
#include <string>
#include <atomic>

inline char breakMsg[] = "ALERT";//WARNING, could be set to "STOP"

static inline std::atomic<HANDLE> hMailslotRemixer = INVALID_HANDLE_VALUE;
//only in mailslot mode we need this function
int
receiveTextFromMailSlot(HANDLE &hMailslot, std::string &str1, std::string &str2, ULONGLONG &t0);
BOOL IsRunAsAdministrator();
bool ElevateNow(const char *args);
HANDLE connect2mailSlotServer(const LPCWSTR &lpname);
int
sendMessageToMailSlotServer(std::atomic<HANDLE> &hMailSlotServer, std::string const &msg);
int processAA_path(
	std::string const &weatherFileName,
	std::string const &inputXMLfileName,
	std::string const &kmlFileName,
	std::string const &trapExec,
	std::string const &stage,
	std::string const &tpDir = "tp",
	std::string const &googleDir = "google",
	bool disregard = false,
	unsigned retries = 3
);

//returns string yyyymmdd_hhmmss format
std::string getYYYYMMDD_hhmmss();
