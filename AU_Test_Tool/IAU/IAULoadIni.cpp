#include "IAULoadIni.h"
#include "IAU_Util.h"
#include "Log.h"

#include <vector>
#include <map>
#include <algorithm>

static inline TiXmlElement* insertElement(TiXmlElement* pNode, const char* psNodeName, const char* psContent=NULL) {
	TiXmlElement* pElem = new TiXmlElement(psNodeName);
	pNode->LinkEndChild(pElem);
	if(psContent != NULL) {
		TiXmlText* pText = new TiXmlText(psContent);
		pElem->LinkEndChild(pText);
	}
	return pElem;
}

static inline void insertNode(TiXmlElement* pNode, const CSimpleIniA::TKeyVal* pKeyVal
									   , const char* pIniKey, const char* pXmlNodeName) {
	CSimpleIniA::TKeyVal::const_iterator ikv = pKeyVal->find(pIniKey);
	if(ikv != pKeyVal->end())
		insertElement(pNode, pXmlNodeName, ikv->second);
}

bool GetSourceInfo(std::wstring*& wsourceInformation, const CSimpleIniA& ini, const char* pSection) {
	const CSimpleIniA::TKeyVal *pIAUConfiguration = ini.GetSection(pSection);
	if(pIAUConfiguration == NULL) return false;
	TiXmlDocument* sourceInformationXml = new TiXmlDocument();
	TiXmlElement* iau_configuration_element = new TiXmlElement("iau_configuration");
	sourceInformationXml->LinkEndChild(iau_configuration_element);
	iau_configuration_element->SetAttribute("version", "1.0");

	//insert index node and server information
	TiXmlElement* index_element = insertElement(iau_configuration_element, "index");
	insertNode(index_element, pIAUConfiguration, "ServerUrl", "server");

	//insert network information
	TiXmlElement* network_element = insertElement(iau_configuration_element, "network");
	insertNode(network_element, pIAUConfiguration, "username", "username");
	insertNode(network_element, pIAUConfiguration, "password", "password");
	insertNode(network_element, pIAUConfiguration, "retryTimes", "retry_times");
	insertNode(network_element, pIAUConfiguration, "bandwidthlimit", "bandwidth_limit");

	//insert proxy information, if useproxy=false, every information will not be inserted.
	CSimpleIniA::TKeyVal::const_iterator ikv = pIAUConfiguration->find("useproxy");	
	if(ikv != pIAUConfiguration->end() && ikv->second[0] != 'f' && ikv->second[0] != 'F') {
		TiXmlElement* proxy_element = insertElement(iau_configuration_element, "proxy");
		insertElement(proxy_element, "enabled", ikv->second);
		insertNode(proxy_element, pIAUConfiguration, "proxytype", "type");
		insertNode(proxy_element, pIAUConfiguration, "proxyaddr", "server");
		insertNode(proxy_element, pIAUConfiguration, "proxyport", "port");
		insertNode(proxy_element, pIAUConfiguration, "proxyusername", "username");
		insertNode(proxy_element, pIAUConfiguration, "proxypassword", "password");
	}

	//insert data folder information
	ikv = pIAUConfiguration->find("datafolder");
	if(ikv != pIAUConfiguration->end()) {
		TiXmlElement* data_element = insertElement(iau_configuration_element, "data");
		insertElement(data_element, "directory", ikv->second);
		insertNode(data_element, pIAUConfiguration, "datareserved", "reserved");
	}

	//sourceInformationXml->SaveFile("sourceInfomation.xml");
	LogResult("get source information:");
	LogResult(sourceInformationXml);
	LogResult();

	//convert xml format information to string
	std::string information;
	GetInfoNode(information, sourceInformationXml->RootElement(), 0);
	wsourceInformation = new std::wstring(Ansi2WChar(information.c_str(), information.size()));
	delete sourceInformationXml;
	return true;
}

bool GetProductsInfo(std::wstring*& wproductsInformation, int& productsNumber, const CSimpleIniA& ini, const char* pSection) {
	const CSimpleIniA::TKeyVal* pProductsInfo = ini.GetSection(pSection);
	if(pProductsInfo == NULL)
		return false;
	char logBuf[256] = {0};
	std::vector<const char* > vProducts;
	std::vector<const char* > vProductsSubPros;
	std::vector<const char* > vProductsSubProsComponents;
	const char* sKey = NULL;
	const char* pos = NULL;
	CSimpleIniA::TKeyVal::const_iterator ikv = pProductsInfo->begin();
	for(; ikv != pProductsInfo->end(); ikv++) {
		sKey = ikv->first.pItem;
		pos = strchr(sKey, '.');
		if(pos == NULL) {
			LogResult("products configuration error!");
			sprintf(logBuf, "%24s = %s", ikv->first.pItem, ikv->second);
			LogResult(logBuf);
			return false;
		}
		if(++pos && (*pos == 'M' || *pos == 'm')) {
			if(++pos && (*pos == 'A' || *pos == 'a')) {
				vProducts.push_back(sKey);
			}
			--pos;
		}
		if(*pos == 'S' || *pos == 's') {
			if((pos = strchr(pos, '.')) == NULL) {
				vProductsSubPros.push_back(sKey);
			} else if(++pos && !strchr(pos, '.')) {
				vProductsSubProsComponents.push_back(sKey);
			}
		}
	}

	productsNumber = vProducts.size();
	wproductsInformation = new std::wstring[productsNumber];
	std::string* productsInformation = new std::string[productsNumber];
	char sProductSubProIndex[100] = {0};
	char sProductIndex[100] = {0};
	char sPattern[100] = {0};
	char *pStr = NULL;
	const char* pSubProIndex = 0;
	TiXmlDocument* product_document = new TiXmlDocument();
	const char* digits = "0123456789";
	sprintf(logBuf, "%d product(s) need to handle...", productsNumber);
	LogResult(logBuf);
	for(int i=0; i<productsNumber; i++) {
		TiXmlElement* product_information_element = new TiXmlElement("product_information");
		product_document->LinkEndChild(product_information_element);
		product_information_element->SetAttribute("version", "1.0.0");
		//get main product
		TiXmlElement* products_element = insertElement(product_information_element, "products");
		ikv = pProductsInfo->find(vProducts[i]);
		if(ikv != pProductsInfo->end()) {
			products_element->SetAttribute("id", ikv->second);
		}
		memset(sProductIndex, '\0', 100);
		strcpy(sProductIndex, vProducts[i]);
		*strchr(sProductIndex, '.') = '\0'; //sProductIndex = "Product[i]";
		//get sub product
		std::vector<const char*>::const_iterator iSubPro = vProductsSubPros.begin();
		for(; iSubPro != vProductsSubPros.end(); iSubPro++) {
			if(strstr(*iSubPro, sProductIndex) == NULL) 
				continue;
			//get product[i].SubProN
			strcpy(sProductSubProIndex, *iSubPro);
			ikv = pProductsInfo->find(sProductSubProIndex);
			if(ikv == pProductsInfo->end()) 
				continue;
			TiXmlElement* product_element = insertElement(products_element, "product");
			product_element->SetAttribute("id", ikv->second);
			pSubProIndex = strpbrk(strchr(sProductSubProIndex, '.'), digits);
			//get Product[i].MiscN
			sprintf(sPattern, "%s.Misc%s", sProductIndex, pSubProIndex);
			insertNode(product_element, pProductsInfo, sPattern, "misc");

			//get Product[i].ActivePathN
			sprintf(sPattern, "%s.ActivePath%s", sProductIndex, pSubProIndex);
			insertNode(product_element, pProductsInfo, sPattern, "path");

			//get Product[i].InstallPathN
			sprintf(sPattern, "%s.InstallPath%s", sProductIndex, pSubProIndex);
			insertNode(product_element, pProductsInfo, sPattern, "install");

			//get Product[i].SubProN.Component[k]
			TiXmlElement* components_element = insertElement(product_element, "components");
			std::vector<const char*>::const_iterator iComponent = vProductsSubProsComponents.begin();
			for(; iComponent != vProductsSubProsComponents.end(); iComponent++) {
				if(strstr(*iComponent, sProductSubProIndex) == NULL)
					continue;
				sprintf(sPattern, "%s", *iComponent);
				ikv = pProductsInfo->find(sPattern);
				if(ikv == pProductsInfo->end()) 
					continue;
				TiXmlElement* component_element = insertElement(components_element, "component");
				//set properties for component
				sprintf(sPattern, "%s", ikv->second);
				const std::string componentAttr[7] = {
					 "class", "type", "version", "language", "platform", "region", "oem"};
				const char* componentDelimiters = "ctvlpro";
				const int componentOrder[7] = {2, 0, 1, 4, 3, 5, 6};
				std::map<std::string, std::string> componentAttrMap;
				const std::string* pString = componentAttr;
				pStr = strtok(sPattern, componentDelimiters);
				while(pStr != NULL && pString < componentAttr+7) {
					componentAttrMap[*pString++] = std::string(pStr);
					pStr = strtok(NULL, componentDelimiters);
				}
				for(int i=0; i<7; i++) {
					component_element->SetAttribute(componentAttr[componentOrder[i]].c_str()
						, componentAttrMap[componentAttr[componentOrder[i]]].c_str());
				}
				//get Product[i].SubProN.Component[k].path
				sprintf(sPattern, "%s.path", *iComponent);
				insertNode(component_element, pProductsInfo, sPattern, "path");

				//get Product[i].SubProN.Component[k].install
				sprintf(sPattern, "%s.install", *iComponent);
				insertNode(component_element, pProductsInfo, sPattern, "install");
			}
		}

		LogResult(product_document);
		GetInfoNode(*(productsInformation+i),product_document->RootElement(), 0);
		*(wproductsInformation+i) = Ansi2WChar((productsInformation+i)->c_str(), (productsInformation+i)->size());
		//strcat(sProductIndex, ".xml");
		//product_document->SaveFile(sProductIndex);
		product_document->Clear();
	}
	LogResult();
	delete product_document;

	return true;
}
