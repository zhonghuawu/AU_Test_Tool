#include "TestFuncProcessor.h"
#include "TestFuncFactory.h"
#include "Log.h"

void TestFuncProcessor::start(const char* pLogFile) {
	LogInit(pLogFile);
	LogTime();
	LogResult("start Test Function Processor.");
}
void TestFuncProcessor::finish() {
	LogTime();
	LogResult("finish Test Function Processor.\n");
	LogFinal();
}

TestFuncProcessor::TestFuncProcessor()
: testFunc(NULL)
, pIni(NULL)
, buildPath(NULL)
, processName(NULL) {
	this->start();
}

TestFuncProcessor::TestFuncProcessor(const char *filename)
: testFunc(NULL)
, pIni(NULL)
, buildPath(NULL)
, processName(NULL) {
	this->start();
	this->LoadIniFile(filename);
}

TestFuncProcessor::~TestFuncProcessor() {
	this->clear();
	this->finish();
}
void TestFuncProcessor::clear() {
	if(pIni!=NULL)
		delete pIni;
	if(testFunc!=NULL)
		delete testFunc;
}

bool TestFuncProcessor::LoadIniFile(const char *pFilename, bool bIsUtf8, bool bUseMultiKey, bool bUseMultiLine) {
	if(pIni != NULL)
		pIni->Reset();
	else
		pIni = new CSimpleIniA(bIsUtf8, bUseMultiKey, bUseMultiLine);
	if(pIni && pIni->LoadFile(pFilename)<0) {
		LogResult("Failed to open ini file.");
		return false;
	}
	processName = pIni->GetValue("ProcessType", "ProcessName");
	if(!processName || processName == "") {
		LogResult("Get ProcessName from section ProcessType failed.");
		return false;
	}
	buildPath = pIni->GetValue("BuildPath", "buildPath");
	if(!buildPath || buildPath == "") {
		LogResult("Get BuildPath from section BuildPath failed.");
		return false;
	}
	return true;
}

bool TestFuncProcessor::getTestFunc() {
	testFunc = TestFuncFactory::createTestFunc(buildPath, processName);
	if(testFunc == NULL) {
		LogResult("get test function failed! please check buildPath and processName in configure ini.");
		return false;
	}
	return true;
}

bool TestFuncProcessor::initEnv() {
//	TmuContext_t context = NULL;
//	IAU_ContextHandle context = NULL;
	void* template_context = NULL;
	if(testFunc!=NULL) {
		if(!testFunc->getInfo(pIni)) return false;
		if(!testFunc->initEnv(buildPath, template_context)) return false;
	} else {
		LogResult("init environment failed! not yet get test function.", TestLog::CONSOLE);
		return false;
	}
	return true;
}

void TestFuncProcessor::runTest() {
	if(testFunc!=NULL) {
		testFunc->testFunc();
	} else {
		LogResult("run testing function failed! not yet get test function.", TestLog::CONSOLE);
	}
}

void TestFuncProcessor::finalEnv() {
	if(testFunc!=NULL) {
		testFunc->finalEnv();
	} else {
		LogResult("init environment failed! not yet get test function.", TestLog::CONSOLE);
	}
}
