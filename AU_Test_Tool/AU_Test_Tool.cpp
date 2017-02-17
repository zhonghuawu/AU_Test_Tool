#include "TestFuncProcessor.h"

bool ParseCommandLine(int argc, char* argv[], const char*& a_pszFile,
					  bool& a_bIsUtf8, bool& a_bUseMultiKey, bool& a_bUseMultiLine) {
	//a_pszFile = "toolcfg.ini";// Default uses toolcfg.ini
    a_bIsUtf8 = false;
    a_bUseMultiKey = false;
    a_bUseMultiLine = false;
	bool isHelp = false;
    for (--argc; argc > 0; --argc) {
        if (argv[argc][0] == '-') {
            switch (argv[argc][1]) {
            case char('u'): a_bIsUtf8 = true; break;
            case char('m'): a_bUseMultiKey = true; break;
            case char('l'): a_bUseMultiLine = true; break;
			case char('h'): isHelp = true; break;
            }
        } else {
            a_pszFile = argv[argc];
        }
    }
    if (isHelp) {
        printf(
			"Usage: AUSEGTool [-u] [-m] [-l] iniFile(Default toolcfg.ini)\n"
			"    or AUSEGTool [-h] get help information\n"
			"   -u  Load file as UTF-8 (Default is to use system locale)\n"
			"   -m  Enable multiple keys\n"
			"   -l  Enable multiple line values\n"
			"   -h  Help information\n"
            );
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
	const char* pFile = "toolcfg.ini";// Default uses toolcfg.ini
	bool bIsUtf8, bUseMultiKey, bUseMultiLine;
	if(!ParseCommandLine(argc, argv, pFile, bIsUtf8, bUseMultiKey, bUseMultiLine)) {
		return 0;
	}
	TestFuncProcessor* pTFProcessor = new TestFuncProcessor(pFile);
	//if(!pTFProcessor->LoadIniFile(pFile)) return -1;
	if(!pTFProcessor->getTestFunc()) 
		return -1;
	if(!pTFProcessor->initEnv()) 
		return -1;
	pTFProcessor->runTest();
	pTFProcessor->finalEnv();
	delete pTFProcessor;
	
	return 0;
}