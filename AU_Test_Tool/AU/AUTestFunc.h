/***
 * abstract AUTestFunc class derived from TestFunc declarartion
 *
 * AUTestFunc class is derived from TestFunc, and implements initEnv and finalEnv function.
 * every concrete class to test function from AU should be derived from AUTestFunc.
 * TmuContext_t context attribute used to build AU environment is added into AUTestFunc.
 *
 */

#ifndef AU_TEST_FUNC_H
#define AU_TEST_FUNC_H
#include "TestFunc.h"
#include "TmLoader.h"

class AUTestFunc :public TestFunc {
public :
	virtual bool getInfo(const CSimpleIniA* pIni) = 0;
	virtual bool initEnv(const char* buildPath, void* template_context);
	virtual void testFunc() = 0;
	virtual void finalEnv();
	virtual ~AUTestFunc() {};
protected :
	TmuContext_t context;
public :
	void setContext(TmuContext_t& ctx) {
		context = ctx;
	}
	TmuContext_t& getContext() {
		return context;
	}
};

#endif

