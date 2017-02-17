#include "AUTestSet.h"
#include "AULoadIni.h"

AUTestSet::AUTestSet() {
	this->properties.clear();
}

bool AUTestSet::getInfo(const CSimpleIniA* pIni) {
	char logBuf[256] = {0};
	const char* pSection = "AUConfiguration";
	if(!GetProperties(properties, *pIni, pSection)) {
		sprintf(logBuf, "Failed to load %s Section.", pSection);
		LogResult(logBuf);
		return false;
	}
	return true;
}

void AUTestSet::testFunc() {
	LogResult("The progress is running now, start to Set property. please wait...");
	char logBuf[256] = {0};
	bool rtn = true;
	for(std::map<const char*, const char*>::const_iterator i=properties.begin(); i!=properties.end(); i++) {
		if(TmuSetPropertyEx(i->first, i->second, context)) {
			sprintf(logBuf, "Set Property: %s = %s succeed.", i->first, i->second);
			LogResult(logBuf);
		}
		else {
			rtn = false;
			sprintf(logBuf, "Set Property: %s = %s failed!", i->first, i->second);
			LogResult(logBuf);
		}
	}
	if(!rtn) {
		LogResult("At least one Property failed to set.");
	} else {
		LogResult("All Properties set successfully.");
	}
}
AUTestSet::~AUTestSet() {
	this->properties.clear();
}