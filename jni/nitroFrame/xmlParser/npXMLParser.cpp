/*
 * npXMLParser.cpp
 *
 *  Created on: 2012. 10. 26.
 *      Author: Ophelia
 */

#include "npXMLParer.h"

void npXMLParser::ParseTextureDataByPath(xmlPath path) {
	const char* TextureXMLText = OpenAssetsByPath(path);

	TiXmlDocument tiXMLdocument;
	tiXMLdocument.Parse(TextureXMLText);		//TextureText의 내용을 tinyXML 분석한다.

	if(tiXMLdocument.Error()){
		LOGE("npXMLParser::ParseByPath)Encounter tiXML Error/ Escape the parse Logic");
		return;
	}
	npTextureRawPacket* returnRawPacket = new npTextureRawPacket;


	const char* texturePath = TextureNode->ToElement()->GetText();
	npTextureGenerator::getInstance()->GenerateTextureByPNGPath(
			texturePath(texturePath));

	//-----------------UV 파싱----------------------//
	GLfloat TempUV[8] = {0,1, 0,0, 1,1, 1,0};
	TextureSize* pBindTexSize = pbDataStorage::GetBindTexSize();
	TiXmlNode* DataHeadNode = xmlDoc.FirstChild("Texture");

	TiXmlNode* UVNode = DataHeadNode->FirstChild("UV");
	while(UVNode !=NULL)
	{
		//UVNode->ToElement()->Attribute("BindID", &BindID);
		//UVNode->ToElement()->Attribute("X", &x);
		//UVNode->ToElement()->Attribute("Y", &y);
		//UVNode->ToElement()->Attribute("Width", &width);
		//UVNode->ToElement()->Attribute("Height", &height);

		//LOGI(UVNode->ToElement()->GetText());

		//SetUVByLeftTop(TempUV,  pBindTexSize[BindID].Width,  pBindTexSize[BindID].Height, x, y, width, height);
		//pbDataStorage::makeUVPacket(BindID, TempUV, (float)width, (float)height);

		UVNode = UVNode->NextSibling();
	}

}

char* npXMLParser::OpenAssetsByPath(xmlPath path) {
	AAssetManager* manager = AAssetManager_fromJava(this->env,this->assetManager);
	if(manager == NULL){
		LOGE("Not Have AssetManager");
		return NULL;
	}

	this->assets= AAssetManager_open(manager, path.c_str(),AASSET_MODE_UNKNOWN);
	if(this->assets == NULL){
		LOGE("Asset not found");
		return NULL;
	}

	off_t size = AAsset_getLength(assets);
	//LOGfloatString("Size", size);
	char* buffer = new char[size];
	AAsset_read(this->assets,buffer,size);
	LOGI("Done Asset Reads");

	return buffer;
}




/***
 * @fn asset을 닫는 함수.
 * @param buffer char*타입의 Text 정보가 담겨져 있는 buffer를 말한다.
 */
void npXMLParser::CloseAssetsAndBuffer(char* buffer) {
	if(buffer != NULL)
		delete buffer;

	AAsset_close(this->assets);

	this->assets = NULL;
}

npXMLParser::npXMLParser(JNIEnv* aEnv, jobject* aAssetManager):env(aEnv),assetManager(aAssetManager),assets(NULL) {
}
npXMLParser::npXMLParser():env(NULL),assetManager(NULL),assets(NULL){
}

npXMLParser::~npXMLParser() {
}
