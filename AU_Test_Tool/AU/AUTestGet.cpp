#include "AUTestGet.h"
#include "AULoadIni.h"

AUTestGet::AUTestGet() {
	this->properties.clear();
}

bool AUTestGet::getInfo(const CSimpleIniA* pIni) {
	const char* pSection = "Property";
	if(!GetPropertiesKey(properties, *pIni, pSection)) {
		char logBuf[256] = {0};
		sprintf(logBuf, "Failed to load %s Section.", pSection);
		LogResult(logBuf);
		return false;
	}
	return true;
}

void AUTestGet::testFunc() {
	LogResult("The progress is running now, start to Get property. please wait...");
	bool rtn = true;
	char logBuf[256] = {0};
	char attr[1024];
	memset(attr, '\0', 1024);
	std::vector<const char*>::const_iterator i = properties.begin();
	for(; i!=properties.end(); i++) {
		if(TmuGetPropertyEx(*i, attr, 1024, context)) {
			sprintf(logBuf, "Get property %s = %s", *i, attr);
			LogResult(logBuf);
		} else {
			sprintf(logBuf, "Get property %s failed!", *i);
			LogResult(logBuf);
			rtn = false;
		}
	}
	if(!rtn){
		sprintf(logBuf, "At least one Property failed to get.");
		LogResult(logBuf);
	} else {
		sprintf(logBuf, "All Properties get successfully.\n");
		LogResult(logBuf);
	}
}

AUTestGet::~AUTestGet() {
	this->properties.clear();
}