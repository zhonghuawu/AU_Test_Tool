#include "IAU_Util.h"
#if defined _WIN32 || defined _WIN64
#else
#include <cstdio>
#endif


//Converting a WChar string to a Ansi string
std::string WChar2Ansi(LPCWSTR pwszSrc) {
#if defined _WIN32 || defined __WIN64
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
#else
	int nLen = wcslen(pwszSrc);
#endif
	if (nLen<= 0) return std::string("");
	char* pszDst = new char[nLen+2];
	memset(pszDst, 0, (nLen+2)*sizeof(char));
	if (NULL == pszDst) return std::string("");

#if defined _WIN32 || defined __WIN64
	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
#else
	wcstombs(pszDst, pwszSrc, nLen);
#endif
	std::string strTemp(pszDst);
	delete [] pszDst;
	return strTemp;
}

//Converting a Ansi string to WChar string
std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen) {
#if defined _WIN32 || defined __WIN64
	int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, 0, 0);
#else
	int nSize = strlen(pszSrc);
#endif
	if(nSize <= 0) return NULL;
#if defined _WIN32 || defined __WIN64
	WCHAR *pwszDst = new WCHAR[nSize+1];
#else
	wchar_t* pwszDst = new wchar_t[nSize+1];
#endif
	memset(pwszDst, 0, (nSize+1)*sizeof(wchar_t));
	if( NULL == pwszDst) return NULL;
#if defined _WIN32 || defined __WIN64
	MultiByteToWideChar(CP_ACP, 0,(LPCSTR)pszSrc, nLen, pwszDst, nSize);
#else
	mbstowcs(pwszDst, pszSrc, nSize);
#endif
	if( pwszDst[0] == 0xFEFF)                    // skip Oxfeff
		for(int i = 0; i < nSize; i ++)
			pwszDst[i] = pwszDst[i+1];
	std::wstring wcharString(pwszDst);
	delete [] pwszDst;
	return wcharString;
}

void GetInfoNode(std::string &information,TiXmlElement* Element, int depth) {
	int count=0;
	bool flag=0;   // judge have out the end tage or not
	information+="<";
	information+=Element->ValueStr();
	TiXmlAttribute* attributeOfProduct = Element->FirstAttribute();
	if(attributeOfProduct) information+=" ";
	while(attributeOfProduct) {
		information+=attributeOfProduct->Name() ;
		information+="=";
		information+="\"";
		information+=attributeOfProduct->ValueStr();
		information+="\"";
		attributeOfProduct=attributeOfProduct->Next();
	}
	information+=">";

	if(Element->GetText()) information+=Element->GetText();
    
	if(Element->FirstChildElement()) {
		++depth;
	   // for( count=0; count<depth;count++){	information+=" ";}
	    GetInfoNode(information,Element->FirstChildElement(),depth);
	}
	//else{              //end tage case 1,no child element,then output end tage
		information+="</";
		information+=Element->ValueStr();
		information+=">";
		flag = 1;
	//}

	if(Element->NextSiblingElement()) {
		  for( count=0; count<depth;count++){	information+=" ";}
          GetInfoNode(information,Element->NextSiblingElement(),depth);
	}
	else if(!flag) {            ////end tage case 2,no brother element, and have not  output in case 1,then output end tage
		information+="</";
		information+=Element->ValueStr();
		information+=">";	
	}	
}


static inline bool isEqualAttr(TiXmlElement* comp1, TiXmlElement* comp2, const char* strAttr) {
	return !strcmp(comp1->Attribute(strAttr), comp2->Attribute(strAttr));
}

static bool isSameComponent(TiXmlElement* comp1, TiXmlElement* comp2) {
	const char* sAttr[] = {"class", "type", "platform", "language", "region", "oem"};
	int lenAttr = sizeof(sAttr)/sizeof(const char*);
	for(int i=0; i < lenAttr; i++) {
		if(!isEqualAttr(comp1, comp2, sAttr[i]))
			return false;
	}
	return true;
}
static bool isSameProduct(TiXmlElement* leftPro, TiXmlElement* rightPro) {
	char leftid[100] = {0};
	char rightid[100] = {0};
	strcpy(leftid, leftPro->Attribute("id"));
	strcpy(rightid, rightPro->Attribute("id"));
	char* pleft = strchr(leftid, 'v');
	char* pright = strchr(rightid, 'v');
	*pleft++ = '\0';
	*pright++ = '\0';
	bool rtn = true;
	if(strcmp(leftid, rightid))
		rtn = false;
	if(rtn) {
		pleft = strchr(pleft, 'l');
		pright = strchr(pright, 'l');
		if(pleft && pright && strcmp(pleft, pright))
			rtn = false;
	}
	return rtn;
}

static TiXmlElement* findNodeFromResult(TiXmlElement* originNode, TiXmlElement* parentResultNode, bool (*comp)(TiXmlElement*, TiXmlElement*)) {
	TiXmlElement* resultNode = parentResultNode->FirstChildElement();
	while(resultNode && !comp(resultNode, originNode))
		resultNode = resultNode->NextSiblingElement();
	return resultNode;
}

static TiXmlElement* GetNodePointerByName(TiXmlElement* pRootElement, const char* strNodeName) {
	if(!strcmp(strNodeName, pRootElement->Value()))
		return pRootElement;
	TiXmlElement* pElem = pRootElement->FirstChildElement();
	for(; pElem; pElem = pElem->NextSiblingElement()) {
		if(GetNodePointerByName(pElem, strNodeName))
			return pElem;
	}
	return NULL;
}

bool renewal(std::string& productInfo, const std::string resultInfo) {
	TiXmlDocument* pro = new TiXmlDocument();
	TiXmlDocument* res = new TiXmlDocument();
	pro->Parse(productInfo.c_str());
	res->Parse(resultInfo.c_str());
	TiXmlElement* product_information_element = pro->RootElement();
	TiXmlElement* update_result_element = res->RootElement();
	TiXmlElement* products_element = product_information_element->FirstChildElement();
	TiXmlElement* rproducts_element = findNodeFromResult(products_element, update_result_element, isSameProduct);
	if(rproducts_element != NULL) {
		products_element->SetAttribute("id", rproducts_element->Attribute("id"));
	}else {
		//printf("cannot find products node!\n");
		return false;
	}
	TiXmlElement* product_element = products_element->FirstChildElement();
	for(; product_element!=NULL; product_element = product_element->NextSiblingElement()) {
		TiXmlElement* rproduct_element = findNodeFromResult(product_element, rproducts_element, isSameProduct);
		if(rproduct_element == NULL)
			continue;
		product_element->SetAttribute("id", rproduct_element->Attribute("id"));
		TiXmlElement* status_element = GetNodePointerByName(rproduct_element, "status");
		if(strcmp(status_element->GetText(), "0"))
			continue;
		TiXmlElement* components_element = GetNodePointerByName(product_element, "components");
		TiXmlElement* rcomponents_element = GetNodePointerByName(rproduct_element, "components");
		if(components_element == NULL || rcomponents_element == NULL) {
			//printf("can not find components node!\n");
			return false;
		}
		TiXmlElement* component_element = components_element->FirstChildElement();
		TiXmlElement* rcomponent_element = NULL;
		for(; component_element; component_element = component_element->NextSiblingElement()) {
			rcomponent_element = findNodeFromResult(component_element, rcomponents_element, isSameComponent);
			if(rcomponent_element == NULL)
				continue;
			component_element->SetAttribute("version", rcomponent_element->Attribute("update_version"));
		}
	}
	productInfo.clear();
	GetInfoNode(productInfo, pro->RootElement(), 0);
	pro->Clear();
	res->Clear();
	delete pro;
	delete res;
	return true;
}

