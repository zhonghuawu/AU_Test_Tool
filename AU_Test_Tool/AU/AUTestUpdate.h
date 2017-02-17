#ifndef AU_TEST_UPDATE_H
#define AU_TEST_UPDATE_H
#include "AUTestFunc.h"

class AUTestSet;

class AUTestUpdate: public AUTestFunc {
private :
	TmuItemInfo_t *pItems;
	int nItemCount;
	SourceInfo sinfo;
	long nOption;
private :
	long loopValue;
	double retryspan;
//	BOOL(*TmuCallback)(uint32_t, uint32_t, void*);
//	void *pVoid;
public :
	AUTestUpdate();
	virtual bool getInfo(const CSimpleIniA* pIni);
	virtual bool initEnv(const char* buildPath, void* template_context);
	virtual void testFunc();
	virtual ~AUTestUpdate();
private :
	AUTestSet *testSet;
};
#endif