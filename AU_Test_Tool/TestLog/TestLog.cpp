#include "TestLog.h"
#include <time.h>

const char* TestLog::pLogFile = "Test.log";

void TestLog::print(FILE* fp) const {
	fputs(m_Info.c_str(), fp);
	fflush(fp);
}
void TestLog::printTime(FILE* fp) const {
	fputs(m_TimeInfo.c_str(), fp);
	fflush(fp);
}

void TestLog::appendInfo(const char *pInfo) {
	if(pInfo!=NULL) {
		this->m_Info.append("\t");
		this->m_Info.append(pInfo);
		this->m_Info.append("\n");
	}
}

void TestLog::appendTimeInfo() {
	char *buf = new char[100];
	time_t curClock;
	time(&curClock);
	tm* pCurTime = localtime(&curClock);
	strftime(buf, 100, "%Y/%m/%d %H:%M:%S", pCurTime);
	m_TimeInfo.append("[");
	m_TimeInfo.append(buf);
	m_TimeInfo.append("]");
	delete []buf;
}

void TestLog::clear() {
	this->m_Info.clear();
}
void TestLog::clearTime() {
	this->m_TimeInfo.clear();
}