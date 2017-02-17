#ifndef AU_TEST_GET_H
#define AU_TEST_GET_H
#include "AUTestFunc.h"
#include <vector>
class AUTestGet : public AUTestFunc {
private :
	std::vector<const char*> properties;
public :
	AUTestGet();
	virtual bool getInfo(const CSimpleIniA* pIni);
	virtual void testFunc();
	virtual ~AUTestGet();
};
#endif