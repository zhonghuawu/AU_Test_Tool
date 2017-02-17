/***
 * class TestFuncFactory declaration
 * 
 * be factory class for creating new product called TestFunc.
 * be designed with class singleton pattern.
 * provide only one public static interface createTestFunc
 * createTestFunc for creating TestFunc according build path of library and name of process.
 * contain two private static method for creating AUTestFunc and IAUTestFunc
 *
 */

#ifndef TEST_FUNC_FACTORY_H
#define TEST_FUNC_FACTORY_H
#include "TestFunc.h"
#include "AUTestFunc.h"
#include "IAUTestFunc.h"

class TestFuncFactory {
private :
	TestFuncFactory();
	static AUTestFunc* createAUTestFunc(const char* processName);
	static IAUTestFunc* createIAUTestFunc(const char* processName);
public :
	static TestFunc* createTestFunc(const char* buildPath, const char* processName);
};
#endif