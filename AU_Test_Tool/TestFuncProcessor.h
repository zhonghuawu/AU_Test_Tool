/***
 * class TestFuncProcessor
 * 
 * aim to encapsulate process procedure for using TestFunc. main function will create a 
 * TestFuncProcessor object and use this object to invoke TestFunc method. main function
 * must maintain the order of TestFuncProcessor member method invoked in order to ensure
 * the steps of initializing env, running testing and finalling env could be executed properly
 * the order invoked follow:
 *		new processor object
 *		load ini file
 *		get test func
 *		initial env
 *		run testing
 *		final env
 *		delete processor object 
 *
 */

#ifndef TEST_FUNC_CUSTERMOR_H
#define TEST_FUNC_CUSTERMOR_H
#include "TestFunc.h"
class TestFuncProcessor {
private :
	TestFunc* testFunc;
	CSimpleIniA* pIni;
	const char* buildPath;
	const char* processName;
public :
	TestFuncProcessor();
	explicit TestFuncProcessor(const char* filename);
	bool LoadIniFile(const char* filename, bool bIsUtf8=false, bool bUseMultiKey=false, bool bUseMultiLine=false);
	bool getTestFunc();
	bool initEnv();
	void runTest();
	void finalEnv();
	void clear();
	~TestFuncProcessor();
private :
	inline void start(const char* pLogFile=NULL);
	inline void finish();
};
#endif