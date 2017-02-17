#ifndef IAU_TEST_UPDATE_H
#define IAU_TEST_UPDATE_H
#include "IAUTestFunc.h"
#include "iau_library.h"
class IAUTestUpdate : public IAUTestFunc {
private :
	Iau::Library *pSdk;
	IAU_ContextHandle context;
private :
	std::wstring* wproductsInformation;
	std::wstring* wsourceInformation;
	int productsNumber;	
private :
	long loopValue;
	double retryspan;
public :
	IAUTestUpdate();
	virtual bool getInfo(const CSimpleIniA* pIni);
	virtual bool initEnv(const char* buildPath, void* template_context);
	virtual void testFunc();
	virtual void finalEnv();
	virtual ~IAUTestUpdate();
};
#endif
