#ifndef __LOG_H__
#define __LOG_H__

#include "TestLog.h"
class TiXmlDocument;

void LogInit(const char* logFilename=NULL);
void LogTime(TestLog::Device device=TestLog::ALL);
void LogResult(const char* pInfo=NULL, TestLog::Device device=TestLog::ALL);
void LogResult(TiXmlDocument* xml, TestLog::Device device=TestLog::ALL);
void LogFinal();

#endif

