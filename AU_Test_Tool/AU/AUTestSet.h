#ifndef AU_TEST_SET_H
#define AU_TEST_SET_H
#include "AUTestFunc.h"
#include <map>
class AUTestSet :public AUTestFunc {
private :
	std::map<const char*, const char*> properties;
public :
	AUTestSet();
	virtual bool getInfo(const CSimpleIniA* pIni);
	virtual void testFunc();
	virtual ~AUTestSet();
};
#endif

