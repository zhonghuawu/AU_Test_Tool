#include "AUTestUpdate.h"
#include "AUTestSet.h"
#include "AULoadIni.h"

#include "util.h"

static BOOL callback(int32_t cmd,TmuCBParam_t lparam,void * pVoid) {
	// add your code here, for 'cmd' definition, refer to "AU.h"
	return TRUE;
}

AUTestUpdate::AUTestUpdate()
: pItems(NULL)
, nItemCount(0)
, nOption(0)
, loopValue(1)
, retryspan(0.0f)
, testSet(new AUTestSet()) {
	memset(&sinfo, 0, sizeof(sinfo));
}

bool AUTestUpdate::getInfo(const CSimpleIniA* pIni) {
	if(!testSet->getInfo(pIni)) return false;
	const char* pSection = "ProcessType";
	nOption = pIni->GetLongValue(pSection, "ProcessOption", 0x0);
	loopValue = pIni->GetLongValue(pSection, "LoopValue", 1);
	retryspan = pIni->GetDoubleValue(pSection, "retryspan", 0.0f);
	char logBuf[256] = {0};
	pSection = "AUConfiguration";
	if(!GetSourceInfo(sinfo, *pIni, pSection)) {
		sprintf(logBuf, "Failed to load %s Section.", pSection);
		LogResult(logBuf);
		return false;
	} else {
		sprintf(logBuf, "Get informations from server: %s", this->sinfo.source);
		LogResult(logBuf);
	}
	pSection = "ItemsInfo";
	if(!GetItemsInfo(this->pItems, this->nItemCount, *pIni, pSection)) {
		sprintf(logBuf, "Failed to load %s Section.", pSection);
		LogResult(logBuf);
		return false;
	}
	return true;
}
bool AUTestUpdate::initEnv(const char* buildPath, void* template_context) {
	bool rtn = this->AUTestFunc::initEnv(buildPath, template_context);
	if(!rtn) {
		LogResult("init Environment failed!");
		return false;
	}
	testSet->setContext(this->getContext());
	testSet->testFunc();
	return true;
}

void AUTestUpdate::testFunc() {
	LogResult("The progress is running now, start to Update. please wait...");
	char logBuf[256] = {0};
	char buf[256];
	int rtn, loopTimes = 0;
	while(true) {
		sprintf(logBuf, "%d times to Update...", ++loopTimes);
		LogResult(logBuf);
		rtn = TmuUpdateEx(pItems, nItemCount, &sinfo, nOption, callback, NULL, context);
		sprintf(logBuf, "TmuUpdateEx returns: %d", rtn);
		LogResult(logBuf);
		
		if (rtn == TmuStatus_SUCCESS) {
			LogResult("Update success.");
			for(int i=0; i<nItemCount; i++) {
				if (pItems[i].klass == CLS_PATTERN) 
					sprintf(buf, "%d",pItems[i].origin.version );
				else
					TmuMakeVersionBuildString (pItems[i].origin, buf, sizeof(buf));
				sprintf(logBuf, "\tFrom version: %s", buf);
				LogResult(logBuf);
				if (pItems[i].klass == CLS_PATTERN)
					sprintf(buf, "%d",pItems[i].result.version );
				else
					TmuMakeVersionBuildString (pItems[i].result, buf, sizeof(buf));
				sprintf(logBuf, "\tTo version: %s", buf);
				LogResult(logBuf);
				/***********/
				if(pItems[i].result.version != 0) {
					pItems[i].origin.version = pItems[i].result.version;
					pItems[i].origin.build = pItems[i].result.build;
					pItems[i].origin.reserved = pItems[i].result.reserved;
				}
				/***********/
			}
		} else if (rtn == TmuStatus_NO_UPDATE_NEEDED) {
			LogResult("No Update needed.");
		} else {
			TmuGetErrorString (rtn, buf, sizeof(buf));
			sprintf(logBuf, "Update failed: %s\n", buf);
			LogResult(logBuf);
		}
		if(loopTimes == this->loopValue)
			break;

		//waiting retryspan minutes
		mySleep(retryspan);
	}
}

AUTestUpdate::~AUTestUpdate() {
	if(pItems != NULL) {
		for(int i=0; i<nItemCount; i++)
			delete pItems[i].dest;
		delete [] pItems;
	}
	delete testSet;
}
