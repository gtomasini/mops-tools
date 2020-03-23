//for using with debugview
#include <windows.h>
#ifdef _DEBUG
bool _trace(const CHAR *format, ...);
bool __trace(const CHAR *format, ...);
#define TRACE __trace
#else
#define TRACE false && _trace
#endif
