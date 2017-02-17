#include "AUTestRollback.h"
#include "AULoadIni.h"

#include "util.h"

static BOOL callback(int32_t cmd,TmuCBParam_t lparam,void * pVoid) {
	// add your code here, for 'cmd' definition, refer to "AU.h"
	return TRUE;
}

AUTestRollback::AUTestRollback()
: pItems(0)
, nItemCount(0)
, nOption(0x0)
, loopValue(1)
, retryspan(0.0f) { }

bool AUTestRollback::getInfo(const CSimpleIniA* pIni) {
	const char* pSection = "ProcessType";
	nOption = pIni->GetLongValue(pSection, "ProcessOption", 0x0);
	loopValue = pIni->GetLongValue(pSection, "LoopValue", 1);
	retryspan = pIni->GetDoubleValue(pSection, "retryspan", 0.0f);
	pSection = "ItemsInfo";
	if(!GetItemsInfo(pItems, nItemCount, *pIni, pSection)) {
		char logBuf[256] = {0};
		sprintf(logBuf, "Failed to load %s Section.", pSection);
		LogResult(logBuf);
		return false;
	}
	return true;
}

void AUTestRollback::testFunc() {
	LogResult("The progress is running now, start to Rollback. please wait...");
	char logBuf[256] = {0};
	char buf[1024]="";
	int rtn, loopTimes=0;
	while(true) {
		sprintf(logBuf, "%d times to Rollback...", ++loopTimes);
		LogResult(logBuf);
		rtn = TmuRollbackEx (pItems, nItemCount, nOption, callback, NULL, context);
		sprintf(logBuf, "TmuRollbackEx returns: %d", rtn);
		LogResult(logBuf);
		if (rtn == TmuStatus_SUCCESS) {
			LogResult("Rollback success.");
		} else if (rtn == TmuStatus_ROLLBACK_NOT_AVAILABLE) {
			LogResult("At least one of the items is not rollbackable.");
		} else {
			TmuGetErrorString (rtn, buf, sizeof(buf));
			sprintf(logBuf, "TmuRollbackEx failed: %s", buf);
			LogResult(logBuf);
		}
		if (rtn == TmuStatus_SUCCESS || rtn == TmuStatus_ROLLBACK_NOT_AVAILABLE) {
			for (int i = 0; i < nItemCount; i++) {
				sprintf(logBuf, "item[%d]: class 0x%x type 0x%x", i, pItems[i].klass, pItems[i].type);
				LogResult(logBuf);
				if (pItems[i].origin.version == 0) {
					LogResult("\tnot rollbackable");
				} else {
					if (pItems[i].klass == CLS_PATTERN) 
						sprintf(buf, "%d",pItems[i].origin.version );
					else
						TmuMakeVersionBuildString (pItems[i].origin, buf, sizeof(buf));
					sprintf(logBuf, "\tFrom version: %s", buf);
					LogResult(logBuf);
					if (pItems[i].klass == CLS_PATTERN)
						sprintf(buf, "%d", pItems[i].result.version);
					else
						TmuMakeVersionBuildString(pItems[i].result, buf, sizeof(buf));
					sprintf(logBuf, "\tTo version: %s", buf);
					LogResult(logBuf);
					/******/
					if(pItems[i].result.version != 0) {
						pItems[i].origin.version = pItems[i].result.version;
						pItems[i].origin.build = pItems[i].result.build;
						pItems[i].origin.reserved = pItems[i].result.reserved;
					}
					/*******/
				}
			}
		}
		if(loopTimes == this->loopValue)
			break;

		//waiting some minutes
		mySleep(retryspan);
	}
}

AUTestRollback::~AUTestRollback() {
	if(pItems != 0) {
		for(int i=0; i<nItemCount; i++)
			delete pItems[i].dest;
		delete [] pItems;
	}
}

