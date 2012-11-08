/*
 * npInsertParser.cpp
 *
 *  Created on: 2012. 10. 29.
 *      Author: Ophelia
 */

#include "npInsertParser.h"

npInsertParser::npInsertParser() {
	// TODO Auto-generated constructor stub

}

npInsertParser::npInsertParser(JavaVM* javaVM, jobject* aAssetManager){
	this->javaVm=javaVM;
	this->assetManager=aAssetManager;
}

npInsertParser::~npInsertParser() {
	// TODO Auto-generated destructor stub
}

/***
 * @fn 파싱을 실행한다.
 * @brief Parsing Logic Implementation
 * @details Inset XML 데이터를 읽어 들이는 로직을 구현하는 함수
 */
void npInsertParser::DoParsing(){

	if(xmlTextBuffer == NULL){
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
	//npTextureRawPacket* returnRawPacket = new npTextureRawPacket;
	textureHeadNode->ToElement()->Attribute("width",&textureWidth);
	textureHeadNode->ToElement()->Attribute("height",&textureHeight);

	const char* texturePath = textureHeadNode->ToElement()->Attribute("filename");
	LOGE("Generate Texture");
	GLuint textureIndex = npContainerDAO::GetInstance().GenerateTextureByPNGPath(texturePath);
	LOGE("Done Generate Texture");
	//-----------------UV 파싱----------------------//

	RawDataPacket tempRawPacket;
	tempRawPacket.textureHeight = textureHeight;
	tempRawPacket.textureWidth = textureWidth;
	TiXmlNode* spriteNode = textureHeadNode->FirstChild("sprite");
	char* tempTag;
	while(spriteNode !=NULL)
	{
		//tag 정보
		tempRawPacket.tag = spriteNode->ToElement()->Attribute("tag");
		tempRawPacket.textureBindIndex = textureIndex;

		//uv height
		spriteNode->ToElement()->Attribute("height",&tempRawPacket.height);
		spriteNode->ToElement()->Attribute("width",&tempRawPacket.width);
		spriteNode->ToElement()->Attribute("y",&tempRawPacket.y);
		spriteNode->ToElement()->Attribute("x",&tempRawPacket.x);

		//InsertDAO 에게 UV Packet을 념겨준다.
		npContainerDAO::GetInstance().InsertUVData(tempRawPacket);
		spriteNode = spriteNode->NextSibling();
	}
	LOGE("Done Insert Parser");
}


