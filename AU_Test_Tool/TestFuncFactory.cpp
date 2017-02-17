#include "TestFuncFactory.h"

#include "AUTestSet.h"
#include "AUTestGet.h"
#include "AUTestUpdate.h"
#include "AUTestDuplicate.h"
#include "AUTestRollback.h"
#include "AUTestNull.h"

#include "IAUTestUpdate.h"
#include "IAUTestDuplicate.h"

#if defined _WIN32 || defined _WIN64
#include <io.h>
#else
#include <unistd.h>
#endif

AUTestFunc* TestFuncFactory::createAUTestFunc(const char *pn) {
	AUTestFunc* testFunc = NULL;
	if(!pn || *pn == '\0') return testFunc;
	char processName[64] = {0};
	char* p = processName;
	while(*pn) *p++ = tolower(*pn++);
	if(!strcmp(processName, "null")) {// NULL operator
		testFunc = new AUTestNull();
	} else if(!strcmp(processName, "set")) {//SetPropertyEx
		testFunc = new AUTestSet();
	} else if(!strcmp(processName, "get")) {//GetPropertyEx
		testFunc = new AUTestGet();
	} else if(!strcmp(processName,"update")) {//Update
		testFunc = new AUTestUpdate();
	} else if(!strcmp(processName, "duplicate")){ //Duplicate
		testFunc = new AUTestDuplicate();
	} else if(!strcmp(processName, "rollback")) {//rollback
		testFunc = new AUTestRollback();
	} else {//unrecognizable operation
		char logBuf[256] = {0};
		sprintf(logBuf, "%s operation is unrecognizable.\n", processName);
		LogResult(logBuf, TestLog::CONSOLE);		
		LogResult("Nothing to do!\n", TestLog::CONSOLE);
	}
	return testFunc;
}
IAUTestFunc* TestFuncFactory::createIAUTestFunc(const char *pn) {
	IAUTestFunc* testFunc = NULL;
	if(!pn || *pn == '\0') return testFunc;
	char processName[64] = {0};
	char* p = processName;
	while(*pn) *p++ = tolower(*pn++);
	if(!strcmp(processName, "null")) {// NULL operator
		printf("Nothing to do!\n");
	} else if(!strcmp(processName,"update")) {//Update
		testFunc = new IAUTestUpdate();
	} else if(!strcmp(processName, "duplicate")){ //Duplicate
		testFunc = new IAUTestDuplicate();
	} else {//unrecognizable operation
		char logBuf[256] = {0};
		sprintf(logBuf, "%s operation is unrecognizable.\n", processName);
		LogResult(logBuf, TestLog::CONSOLE);	
		LogResult("Nothing to do!\n", TestLog::CONSOLE);
	}
	return testFunc;
}
TestFunc* TestFuncFactory::createTestFunc(const char *buildPath, const char *processName) {
	TestFunc* testFunc = NULL;
	char bufUpdate[256] = {0};
	char bufDuplicate[256] = {0};
#ifdef _WIN32
	sprintf(bufUpdate, "%s\\iau.dll", buildPath);
	sprintf(bufDuplicate, "%s\\iaurelay.dll", buildPath);
#else
	sprintf(bufUpdate, "%s/libiau.so", buildPath);
	sprintf(bufDuplicate, "%s/libiaurelay.so", buildPath);
#endif
	if((access(bufUpdate, 0) != -1) || (access(bufDuplicate, 0) != -1)) {
		testFunc = createIAUTestFunc(processName);
	} else {
		testFunc = createAUTestFunc(processName);
	}
	return testFunc;
}
