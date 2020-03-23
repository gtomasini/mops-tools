//Mailslot name format - \\.\mailslot\mailslotname
//This mailslot is for server on the same computer, 
//however, mailslots can be used to connect to a remote server
//WARNING: this inline works only in cpp 17!!!!
//this mail slot should be created by TPServer
#pragma once
#include <Windows.h>

inline const auto BUFFER_SIZE = 1024; //1k

const LPCWSTR lpTPserverSlotName = L"\\\\.\\Mailslot\\TPserver_slot";


