#ifndef AU_TEST_NULL_H
#define AU_TEST_NULL_H
#include "AUTestFunc.h"
class AUTestNull: public AUTestFunc {
public :
	virtual bool getInfo(const CSimpleIniA* pIni);
	virtual void testFunc();
};
#endif