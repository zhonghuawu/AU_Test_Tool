/***
 * abstract IAUTestFunc class derived from TestFunc declarartion
 *
 * IAUTestFunc class is derived from TestFunc.
 * every concrete class to test function from AU should be derived from IAUTestFunc.
 * IAU_ContextHandle context attribute used to build iAU environment is added into IAUTestFunc.
 *
 */

#ifndef IAU_TEST_FUNC_H
#define IAU_TEST_FUNC_H
#include "TestFunc.h"

class IAUTestFunc : public TestFunc {
public :
	virtual bool getInfo(const CSimpleIniA* pIni) = 0;
	virtual bool initEnv(const char* buildPath, void* template_context) = 0;
	virtual void testFunc() = 0;
	virtual void finalEnv() = 0;
	virtual ~IAUTestFunc() {};
};
#endif
