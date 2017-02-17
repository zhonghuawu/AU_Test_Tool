#ifndef AU_LOAD_INI_H
#define AU_LOAD_INI_H
#include "SimpleIni.h"
#include "TmLoader.h"

#include <map>
#include <vector>

bool GetSourceInfo(SourceInfo& sinfo, const CSimpleIniA& ini, const char *pSection);
bool GetProperties(std::map<const char*, const char*>& properties, const CSimpleIniA& ini, const char* pSection);
bool GetPropertiesKey(std::vector<const char*>& vproperties, const CSimpleIniA& ini, const char* pSection);
bool GetItemsInfo(TmuItemInfo_t *& items, int& itemsNumber, const CSimpleIniA& ini, const char* pSection);
#endif

