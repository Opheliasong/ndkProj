/*
 * npXMLParser.cpp
 *
 *  Created on: 2012. 10. 26.
 *      Author: Ophelia
 */

#include "npXMLParer.h"

/***
 * @fn 기본 SetUPXMLPath함수. OpenAssetsByPath를 통해 AssetManager를 통해 Asset을 생성하고 Assets폴더에서 XML Raw Data를 가져올 준비를 한다.
 * @param path
 * @details 세부적인 Open Setup내용들은 Drived Class에서 작성해야 한다.
 */
void npXMLParser::SetupXMLPath(xmlPath path){
	this->xmlTextBuffer = OpenAssetsByPath(path);
}

/***
 * @fn Asset을 여는 함수.
 * @param path xml데이터가 위치한 곳의 Path를 받아와서 이를 open 한다
 * @return
 */
char* npXMLParser::OpenAssetsByPath(xmlPath path) {
	AAssetManager* manager = AAssetManager_fromJava(this->env,*this->assetManager);
	if(NP_IS_EMPTY(manager)){
		LOGE("Not Have AssetManager");
		return NULL;
	}

	this->assets = AAssetManager_open(manager, path.c_str(),AASSET_MODE_UNKNOWN);
	LOGE("Open Asset Manager");

	if(NP_IS_EMPTY(this->assets)){
		LOGE("Asset not found");
		return NULL;
	}

	off_t size = AAsset_getLength(assets);
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

npXMLParser::npXMLParser(JNIEnv* aEnv, jobject* aAssetManager):env(aEnv),assetManager(aAssetManager),assets(NULL),xmlTextBuffer(NULL) {

}

npXMLParser::npXMLParser():env(NULL),assetManager(NULL),assets(NULL),xmlTextBuffer(NULL){

}

npXMLParser::~npXMLParser() {

}
