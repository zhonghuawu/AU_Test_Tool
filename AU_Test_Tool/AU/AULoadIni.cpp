#include "AULoadIni.h"
#include "Log.h"
#include <stdio.h>

bool GetSourceInfo(SourceInfo& sinfo, const CSimpleIniA& ini, const char *pSection) {
	const CSimpleIniA::TKeyVal *pAUConfiguration = ini.GetSection(pSection);	
	if(pAUConfiguration == NULL) {
		return false;
	}
	CSimpleIniA::TKeyVal::const_iterator ikv = pAUConfiguration->find("serverUrl");//sinfo.source
	if(ikv != pAUConfiguration->end()){
		sinfo.source = const_cast<char*>(ikv->second); 
	}
	ikv = pAUConfiguration->find("userName");//sinfo.username
	if(ikv != pAUConfiguration->end()){
		sinfo.username = const_cast<char*>(ikv->second);
	}
	ikv = pAUConfiguration->find("passWord");//sinfo.password
	if(ikv != pAUConfiguration->end()){
		sinfo.password = const_cast<char*>(ikv->second);
	}
	sinfo.useProxy = ini.GetBoolValue(pSection, "useProxy");//sinfo.useProxy
	ikv = pAUConfiguration->find("proxyUrl");//sinfo.proxy
	if(ikv != pAUConfiguration->end()){
		sinfo.proxy = const_cast<char*>(ikv->second);
	}
	sinfo.proxy_port = ini.GetLongValue(pSection, "proxyPort");//sinfo.proxyPort
	ikv = pAUConfiguration->find("proxyUserName");//sinfo.proxy_username
	if(ikv != pAUConfiguration->end()){
		sinfo.proxy_username = const_cast<char*>(ikv->second);
	}
	ikv = pAUConfiguration->find("proxyPassword");//sinfo.proxy_password
	if(ikv != pAUConfiguration->end()){
		sinfo.proxy_password = const_cast<char*>(ikv->second); 
	}
	return true;
}

bool GetProperties(std::map<const char*, const char*>& preperties, const CSimpleIniA& ini, const char* pSection) {
	const CSimpleIniA::TKeyVal *pProperties = ini.GetSection(pSection);	
	if(pProperties == 0) {
		return false;
	}
	const char* allProperties[] = { 	/* the 'key' for TmuSetProperty */
		"SocketTimeout"	, "SocketTimeoutConnect", "SocketTimeoutInterval",	"RetryCount", "IpPriority", 
		"UserAgent", "ResumeDownload", "CachePath", "CacheTTL",	"CacheMaxSize", "ProductName", "ProductInfo", 
		"ActiveUpdatePath",	"OSCodePage", "MsgCodePage", "InfoHttpsLevel", "FileHttpsLevel", "KeptPatternMaxCount",
		"StoragePath", "MaxDiffFileSizePercentage", "BandWidth", "UseOldBuildFormat", "CheckFileSignature", "Use_TLS1_2_Only"
	};
	int len = sizeof(allProperties)/sizeof(char*);
	CSimpleIniA::TKeyVal::const_iterator ikv = pProperties->begin();
	for(int i=0; i<len; i++) {
		ikv = pProperties->find(allProperties[i]);
		if(ikv!=pProperties->end()) {
			preperties[ikv->first.pItem] = ikv->second;
		}
	}
	return true;
}

bool GetPropertiesKey(std::vector<const char*>& vpro, const CSimpleIniA& ini, const char* pSection) {
	const CSimpleIniA::TKeyVal *pProperties = ini.GetSection(pSection);	
	if(pProperties == 0) {
		return false;
	}
	CSimpleIniA::TKeyVal::const_iterator ikv = pProperties->begin();
	for(; ikv!=pProperties->end(); ikv++) {
		vpro.push_back(ikv->second);
	}
	return true;
}

bool GetItemsInfo(TmuItemInfo_t*& items, int& itemsNumber, const CSimpleIniA& ini, const char* pSection) {
	const CSimpleIniA::TKeyVal *pItemsInfo = ini.GetSection(pSection);	
	if(pItemsInfo == NULL) {
		return false;
	}
	char logBuf[256] = {0};
	std::vector<const char*> vItems;
	const char* sKey = NULL;
	const char* cpStr = NULL;
	const char* dformat = "%-24s = %d";
	const char* sformat = "%-24s = %s";
	const char* xformat = "%-24s = 0x%x";
	CSimpleIniA::TKeyVal::const_iterator ikv = pItemsInfo->begin();
	for(; ikv!=pItemsInfo->end(); ikv++) {
		sKey = ikv->first.pItem;		
		cpStr = strchr(sKey, '.');
		if(cpStr == NULL) {
			LogResult("Items conguration error!");
			sprintf(logBuf, sformat, ikv->first.pItem, ikv->second);
			LogResult(logBuf);
			return false;
		} else if(++cpStr && (*cpStr == 'c' || *cpStr == 'C')) {
			vItems.push_back(sKey);
		}
	}
	itemsNumber = vItems.size();//the numbers of available Items	
	items = new TmuItemInfo_t[itemsNumber];
	memset(items, 0, sizeof(TmuItemInfo_t)*itemsNumber);
	sprintf(logBuf, "%d item(s) need to handle...", itemsNumber);
	LogResult(logBuf);
	char sPattern[32] = "";
	char* pStr = NULL;
	const char *postfix = NULL;
	for(int i=0; i<itemsNumber; i++) {
		memset(sPattern, '\0', 32);
		memcpy(sPattern, vItems[i], strlen(vItems[i]));
		pStr = strchr(sPattern, '.') + 1;
		
		//get items[i].klass
		postfix = "class";
		strcpy(pStr, postfix);
		items[i].klass = ini.GetLongValue(pSection, sPattern);
		sprintf(logBuf, dformat, sPattern, items[i].klass);
		LogResult(logBuf);

		//get items[i].type
		postfix = "type";
		strcpy(pStr, postfix);
		items[i].type = ini.GetLongValue(pSection, sPattern);
		sprintf(logBuf, xformat, sPattern, items[i].type);
		LogResult(logBuf);
		

		//get items[i].action
		postfix = "action";
		strcpy(pStr, postfix);
		items[i].action = ini.GetLongValue(pSection, sPattern);
		sprintf(logBuf, xformat, sPattern, items[i].action);
		LogResult(logBuf);

		//get items[i].originVersion
		postfix = "originVersion";
		strcpy(pStr, postfix);
		items[i].origin.version = ini.GetLongValue(pSection, sPattern);
		sprintf(logBuf, dformat, sPattern, items[i].origin.version);
		LogResult(logBuf);

		//get items[i].langauge
		postfix = "language";
		strcpy(pStr, postfix);
		items[i].language = ini.GetLongValue(pSection, sPattern);
		sprintf(logBuf, dformat, sPattern, items[i].language);
		LogResult(logBuf);
		
		//get items[i].platform
		postfix = "platform";
		strcpy(pStr, postfix);
		items[i].platform = ini.GetLongValue(pSection, sPattern);
		sprintf(logBuf, dformat, sPattern, items[i].platform);
		LogResult(logBuf);
		
		//get items[i].dest
		DestInfo *dest = new DestInfo();
		memset(dest, 0, sizeof(dest));
		postfix = "DestFolder";
		strcpy(pStr, postfix);//items[i].dest.destination
		ikv = pItemsInfo->find(sPattern);
		if(ikv != pItemsInfo->end()) {
			dest->destination = const_cast<char*>(ikv->second);
			sprintf(logBuf, sformat, sPattern, dest->destination);
			LogResult(logBuf);
		}
		postfix = "UserName";
		strcpy(pStr, postfix);//items[i].dest.username
		ikv = pItemsInfo->find(sPattern);
		if(ikv!=pItemsInfo->end()) {
			dest->username = const_cast<char*>(ikv->second);
			sprintf(logBuf, sformat, sPattern, dest->username);
			LogResult(logBuf);
		}
		postfix = "PassWord";
		strcpy(pStr, postfix);//items[i].dest.passward
		ikv = pItemsInfo->find(sPattern);
		if(ikv!=pItemsInfo->end()) {
			dest->password = const_cast<char*>(ikv->second);
			sprintf(logBuf, sformat, sPattern, dest->password);
			LogResult(logBuf);
		}
		postfix = "bAddConnection";
		strcpy(pStr, postfix);//items[i].dest.AddConnection
		ikv = pItemsInfo->find(sPattern);
		if(ikv!=pItemsInfo->end()) {
			dest->bAddConnection = ini.GetBoolValue(pSection, sPattern);
			sprintf(logBuf, sformat, sPattern, dest->bAddConnection);
			LogResult(logBuf);
		}
		postfix = "bDelConnection";
		strcpy(pStr, postfix);//items[i].dest.bDelConnection
		ikv = pItemsInfo->find(sPattern);
		if(ikv!=pItemsInfo->end()) {
			dest->bDelConnection = ini.GetBoolValue(pSection, sPattern);
			sprintf(logBuf, dformat, sPattern, dest->bDelConnection);
			LogResult(logBuf);
		}
		items[i].dest = dest;

		LogResult();
	}
	return true;
};
		