/***
 * abstrat base class TestFunc declaration
 * 
 * this abstrat class is used to define uniform function interfaces;
 *		getInfo methods for loading informations from a CSimpleIniA* object;
 *		initEnv methods for initializing environment required by the function tested;
 *		testFunc methods for running the function tested;
 *		finalEnv methods for releasing environment built by initEnv;
 *		destructor methods for freeing informations loaded by getInfo;
 * every concrete class derived from this class must implement all five functions exactly;
 * 
 * new TestFunc object will be created by TestFuncFactory. the pointer of new TestFunc object will
 * be introduced to TestFuncProcessor, it will use the pointer to call these interfaces defined in
 * TestFunc. because the polymorphism of virtual member function, suitable implementation will be
 * executed accurately.
 * 
 */

#ifndef TEST_FUNC_H
#define TEST_FUNC_H
#include "SimpleIni.h"
#include "Log.h"

class TestFunc {
public :
	virtual bool getInfo(const CSimpleIniA* pIni) = 0;
	virtual bool initEnv(const char* buildPath, void* template_context) = 0;
	virtual void testFunc() = 0;
	virtual void finalEnv() = 0;
	virtual ~TestFunc() {};
};
#endif

