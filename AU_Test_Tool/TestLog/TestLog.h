#ifndef TEST_LOG_H
#define TEST_LOG_H
#include <stdio.h>
#include <string>
class TestLog {
private :
	std::string m_Info;
	std::string m_TimeInfo;
public :
	static const char* pLogFile;

	typedef enum { ALL=~0, CONSOLE=1, LOGFILE=2} Device;
	void print(FILE* fp) const;
	void printTime(FILE* fp) const;
	void appendInfo(const char* pInfo);
	void appendTimeInfo();
	void clear();
	void clearTime();
};
#endif

