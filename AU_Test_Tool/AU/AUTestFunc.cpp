#include "AUTestFunc.h"

bool AUTestFunc::initEnv(const char* buildPath, void* templat)  {
	char logBuf[256] = {0};
	if(!TmuLoaderInit(buildPath)) {
		LogResult("Load AU failed.");
		sprintf(logBuf, "AU path: %s\n", buildPath);
		LogResult(logBuf);
		return false;
	}
	sprintf(logBuf, "Load AU from %s successfully!", buildPath);
	LogResult(logBuf);
	context = TmuCreateContext((TmuContext_t)templat);
	return true;
}

void AUTestFunc::finalEnv() {
	TmuReleaseContext(context);
	TmuLoaderFinal ();
	LogResult("Done!");
}
