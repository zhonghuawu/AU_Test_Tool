#include "IAUTestUpdate.h"
#include "IAULoadIni.h"
#include "IAU_Util.h"
#include "util.h"

IAUTestUpdate::IAUTestUpdate()
: pSdk(NULL)
, wproductsInformation(NULL)
, wsourceInformation(NULL)
, productsNumber(0)
, loopValue(1)
, retryspan(0.0f) { }

bool IAUTestUpdate::getInfo(const CSimpleIniA *pIni) {
	const char* pSection = "IAUConfiguration";
	bool rtn = GetSourceInfo(this->wsourceInformation, *pIni, pSection);
	pSection = "ProductsInfo";
	if(rtn)
		rtn = GetProductsInfo(this->wproductsInformation, this->productsNumber, *pIni, pSection);
	pSection = "ProcessType";
	if(rtn)
		loopValue = pIni->GetLongValue(pSection, "LoopValue", 1);
	if(rtn)
		retryspan = pIni->GetDoubleValue(pSection, "retryspan", 0);
	return rtn;
}

bool IAUTestUpdate::initEnv(const char* buildPath, void* template_context) {
	std::string sbuildPath(buildPath);
	std::wstring wbuildPath = Ansi2WChar(sbuildPath.c_str(), sbuildPath.size());;
#ifdef _WIN32
	wbuildPath+= L"\\iau.dll";
#else 
	wbuildPath+= L"/libiau.so";
#endif
	char logBuf[256] = {0};
	pSdk = new Iau::Library(wbuildPath);
	if(pSdk == NULL || !pSdk->Load()) {
		sprintf(logBuf, "Load iau from %s failed!", WChar2Ansi(wbuildPath.c_str()).c_str());
		LogResult(logBuf);
		return false;
	} else {
		sprintf(logBuf, "Load iau from %s successfully!", WChar2Ansi(wbuildPath.c_str()).c_str());
		LogResult(logBuf);
	}	
	IAU_Status status = this->pSdk->Startup(NULL);//pSdk->Startup(NULL);
	if (IAU_FAILED(status)) {
		LogResult("Start up iau failed!");
		return false;
	}
	status = pSdk->CreateContext(context, *wsourceInformation);
	if (IAU_FAILED(status)) {
		LogResult("Create context faild!");
		return false;
	}///CreateContext
	return true;
}

void IAUTestUpdate::testFunc() {
	LogResult("The progress is running now, start to Update. please wait...");
	char logBuf[256] = {0};
	int loopTimes = 0;
	TiXmlDocument* xml = new TiXmlDocument();
	while(true) {
		sprintf(logBuf, "%d times to update...", ++loopTimes);
		LogResult(logBuf);
		bool bAllSucceeded = true;
		std::wstring resultHandle;
		std::string resultInfo;
		std::string productInfo;
		IAU_Status status;
		for(int i=0; i<this->productsNumber; i++) {
			resultHandle.clear();
			resultInfo.clear();
			productInfo.clear();
			xml->Clear();
			status = pSdk->Update(context, wproductsInformation[i], resultHandle);
			
			if(IAU_FAILED(status)) {
				bAllSucceeded = false;
				sprintf(logBuf, "update product %d failed!", i+1);
				LogResult(logBuf);				
			}
			if(!resultHandle.empty() && resultHandle[0] != L'\0') {
				resultInfo = WChar2Ansi(resultHandle.c_str());
				productInfo = WChar2Ansi((wproductsInformation+i)->c_str());
				/*******/
				xml->Parse(resultInfo.c_str());
				LogResult(xml);
				xml->Clear();
				/*******/
				renewal(productInfo, resultInfo);
				(wproductsInformation+i)->clear();
				*(wproductsInformation+i) = Ansi2WChar(productInfo.c_str(), productInfo.size());
			}
		}
		if(bAllSucceeded) {
			LogResult("All pruducts are succeeded to update using iau!");
		} else {
			LogResult("At least one product is failed to update using iau!");
		}
		
		if(loopTimes == this->loopValue) 
			break;

		//wait retryspan minutes
		mySleep(retryspan);
	}
	delete xml;
}

void IAUTestUpdate::finalEnv() {
	pSdk->DestroyContext(context);
	pSdk->Shutdown();
	LogResult("Done!");
}

IAUTestUpdate::~IAUTestUpdate() {
	this->wsourceInformation->clear();
	delete wsourceInformation;
	if(this->wproductsInformation!=NULL) {
		for(int i=0; i<this->productsNumber; i++)
			this->wproductsInformation[i].clear();
		delete [] this->wproductsInformation;
	}
	delete pSdk;
}
