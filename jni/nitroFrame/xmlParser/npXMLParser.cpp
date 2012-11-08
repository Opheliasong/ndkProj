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

void npXMLParser::SetupApkPath(std::string apkPath){

	LOGE("XMLparser)setup APk path : %s",apkPath.c_str());

	this->apkArchive = zip_open(apkPath.c_str(),0,NULL);

	if(this->apkArchive == NULL){
		LOGE("npXMLParsing) Error loading APk");
	}
}

/***
 * @fn Asset을 여는 함수.
 * @param path xml데이터가 위치한 곳의 Path를 받아와서 이를 open 한다
 * @return
 */
char* npXMLParser::OpenAssetsByPath(xmlPath path) {
	/*
	//JavaVM* pJavaVm;
	//this->env->GetJavaVM(&pJavaVm);

	JNIEnv* env = NULL;

	if(NP_IS_EMPTY(this->javaVm)){
		LOGE("Empty JavaVM");
	}

	if(this->javaVm->GetEnv((void**)env,JNI_VERSION_1_4) != JNI_OK){
		this->javaVm->AttachCurrentThread(&env,NULL);
	}


	LOGE("ICS Open Assets boy path test");

	AAssetManager* manager = AAssetManager_fromJava(env,*this->assetManager);
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


	*/

	char pngPath[128];
	sprintf(pngPath,"assets/%s\0",path.c_str());

	LOGE("png Path: %s",pngPath);
	zip_file* apkZipFile = zip_fopen(this->apkArchive,pngPath,0);
	if(NP_IS_EMPTY(apkZipFile)){
		LOGE("XMLparser) not open zipFile");
		return NULL;
	}
	LOGE("done zip fopen");
	LOGE("zip Left Bytes: %d",apkZipFile->bytes_left);

	char* buffer= new char[apkZipFile->bytes_left];
	zip_fread(apkZipFile,buffer,apkZipFile->bytes_left);

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

npXMLParser::npXMLParser(JavaVM* javaVM, jobject* aAssetManager):javaVm(javaVM),assetManager(aAssetManager),assets(NULL),xmlTextBuffer(NULL) {

}

npXMLParser::npXMLParser():javaVm(NULL),assetManager(NULL),assets(NULL),xmlTextBuffer(NULL){

}

npXMLParser::~npXMLParser() {
	delete[] xmlTextBuffer;
}
