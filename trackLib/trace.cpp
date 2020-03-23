#include "trace.h"
#include <stdio.h>

#ifdef _DEBUG
bool _trace(const CHAR *format, ...)
{
	CHAR buffer[1000];

	va_list argptr;
	va_start(argptr, format);
	wvsprintf(buffer, format, argptr);
	va_end(argptr);

	OutputDebugString(buffer);

	return true;
}

bool __trace(const CHAR *format, ...)
{
	CHAR buffer[1000];

	va_list argptr;
	va_start(argptr, format);
	vsprintf(buffer, format, argptr);
	va_end(argptr);

	OutputDebugString(buffer);

	return true;
}


#endif