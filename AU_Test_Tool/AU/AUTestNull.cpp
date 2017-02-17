#include "AUTestNull.h"
#include "Log.h"

bool AUTestNull::getInfo(const CSimpleIniA *pIni) {
	LogResult("Needn't to get information!");
	return true;
}

void AUTestNull::testFunc() {
	LogResult("Needn't to run any function!");
	LogResult("Null operator just load and release AU.");
}