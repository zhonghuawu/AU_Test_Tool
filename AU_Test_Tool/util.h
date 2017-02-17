#ifndef __UTIL_H__
#define __UTIL_H__
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#define LPCWSTR const wchar_t*
#define LPCSTR const char*
#endif
//process sleep function
void mySleep(double minutes);

#endif
