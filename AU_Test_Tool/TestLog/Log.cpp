#include "Log.h"
#include "tinyxml.h"

static TestLog log;
static FILE* fp = NULL;

void LogInit(const char* logFilename) {
	std::string sLogFile;
	if(logFilename != NULL && logFilename[0] != '\0')
		sLogFile += logFilename;
	else
		sLogFile += TestLog::pLogFile;
	if((fp = fopen(sLogFile.c_str(), "a")) == NULL) {
		printf("open log file failed!\n");
		printf("log path: %s\n", sLogFile.c_str());
		exit(-1);
	}
	log.clear();
	log.clearTime();
}
void LogTime(TestLog::Device device) {
	log.appendTimeInfo();
	if(device & TestLog::CONSOLE)
		log.printTime(stdout);
	if(device & TestLog::LOGFILE)
		log.printTime(fp);
	log.clearTime();
}
void LogResult(const char* pInfo, TestLog::Device device) {
	log.appendInfo(pInfo);
	if(device & TestLog::CONSOLE)
		log.print(stdout);
	if(device & TestLog::LOGFILE)
		log.print(fp);
	log.clear();
}
void LogResult(TiXmlDocument* xml, TestLog::Device device) {
	if(xml == NULL)
		return;
	if(device & TestLog::CONSOLE) 
		xml->Print(stdout, 1);
	if(device & TestLog::LOGFILE)
		xml->Print(fp, 1);
}

void LogFinal() {
	log.clear();
	log.clearTime();
	fclose(fp);
}
