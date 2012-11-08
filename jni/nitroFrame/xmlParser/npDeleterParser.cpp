/*
 * npDeleterParser.cpp
 *
 *  Created on: 2012. 10. 29.
 *      Author: Ophelia
 */

#include "npDeleterParser.h"

npDeleterParser::npDeleterParser(JavaVM* javaVM, jobject* aAssetManager){
	javaVm=javaVM;
	assetManager = aAssetManager;
}

npDeleterParser::npDeleterParser() {
	// TODO Auto-generated constructor stub

}

npDeleterParser::~npDeleterParser() {
	// TODO Auto-generated destructor stub
}

void npDeleterParser::DoParsing() {
	if(xmlTextBuffer == NULL){
		//textBuffer Not Ready!!
		LOGE("InsertPartser)  textBuffer Not Ready!!");
		return;
	}

	TiXmlDocument tiXMLdocument;
	tiXMLdocument.Parse(xmlTextBuffer);		//TextureText의 내용을 tinyXML 분석한다.


	if(tiXMLdocument.Error()){
		LOGE("npXMLParser::ParseByPath)Encounter tiXML Error/ Escape the parse Logic");
		return;
	}

	int textureWidth;
	int textureHeight;
	TiXmlNode* textureHeadNode = tiXMLdocument.FirstChild("texture");

	//-----------------UV 파싱----------------------//

	RawDataPacket tempRawPacket;
	TiXmlNode* spriteNode = textureHeadNode->FirstChild("sprite");
	char* tempTag;
	while(spriteNode !=NULL)
	{
		//tag 정보
		tempRawPacket.tag = spriteNode->ToElement()->Attribute("tag");

		//DeleterModule에게 tag를 기반으로 delete를 실행한다.
		npContainerDAO::GetInstance().DeleteUVData(tempRawPacket.tag);
		spriteNode = spriteNode->NextSibling();
	}
}
