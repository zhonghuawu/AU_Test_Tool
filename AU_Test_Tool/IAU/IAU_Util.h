#ifndef __IAU_UTIL_H__
#define __IAU_UTIL_H__
#include "tinystr.h"
#include "tinyxml.h"
#if defined _WIN32 || defined _WIN64
#include "Windows.h"
#else
#define LPCWSTR const wchar_t*
#define LPCSTR const char*
#endif
//Converting a WChar string to a Ansi string
std::string WChar2Ansi(LPCWSTR pwszSrc);

//Converting a Ansi string to WChar string
std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen);

//Converting a xml to a string
void GetInfoNode(std::string &information,TiXmlElement* Element, int depth);

//renewal product information of iAU using result information generated by iAU.
//used in iAU function tested.
bool renewal(std::string& productInfo, const std::string resultInfo);

#endif
