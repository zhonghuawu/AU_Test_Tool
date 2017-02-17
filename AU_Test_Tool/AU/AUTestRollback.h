#ifndef AU_TEST_ROLLBACK_H
#define AU_TEST_ROLLBACK_H
#include "AUTestFunc.h"
class AUTestRollback : public AUTestFunc {
private :
	TmuItemInfo_t *pItems;
	int nItemCount;
	long nOption;
private :
	long loopValue;
	double retryspan;
//	BOOL (*TmuCallback)(int32_t, int32_t, void*);
//	void *pVoid;
public :
	AUTestRollback();
	virtual bool getInfo(const CSimpleIniA* pIni);
	virtual void testFunc();
	virtual ~AUTestRollback();
};
#endif