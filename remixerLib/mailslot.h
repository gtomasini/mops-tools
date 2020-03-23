//Mailslot name format - \\.\mailslot\mailslotname
//This mailslot is for server on the same computer, 
//however, mailslots can be used to connect to a remote server
//WARNING: this inline works only in cpp 17!!!!
//this mail slot should be created by TPServer
#pragma once
#include <Windows.h>

const LPCWSTR lpRemixerSlotName = L"\\\\.\\Mailslot\\REMIXER_slot";
