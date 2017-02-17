#ifndef IAU_LOAD_INI_H
#define IAU_LOAD_INI_H
#include "SimpleIni.h"
#include <string>

bool GetSourceInfo(std::wstring*& wsourceInformation, const CSimpleIniA& ini, const char* pSection);
bool GetProductsInfo(std::wstring*& wproductsInformation, int& productsNumber, const CSimpleIniA& ini, const char* pSection);

#endif
