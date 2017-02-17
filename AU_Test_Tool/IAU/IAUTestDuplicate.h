#ifndef IAU_TEST_DUPLICATE_H
#define IAU_TEST_DUPLICATE_H
#include "IAUTestFunc.h"
class IAUTestDuplicate : public IAUTestFunc {
public :
	IAUTestDuplicate();
	virtual bool getInfo(const CSimpleIniA* pIni);
	virtual bool initEnv(const char* buildPath, void* template_context);
	virtual void testFunc();
	virtual void finalEnv();
	virtual ~IAUTestDuplicate();
};
#endif