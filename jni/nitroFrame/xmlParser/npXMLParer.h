/*
 * npXMLParer.h
 *
 *  Created on: 2012. 10. 26.
 *      Author: Ophelia
 */

#ifndef NPXMLPARER_H_
#define NPXMLPARER_H_

#include <jni.h>
#include <string>
#include "../include/tinyxml.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "../npDebugUtill.h"
#include "../npNitroFrameMacro.h"
#include "../include/zipint.h"
#include "../include/zip.h"

/***
 * @author 박성민
 * @brief XML Parser들의 Interface Class
 * @details xmlParser는 env객체와 assetManager가 있어야 객체를 생성 할 수 있다.
 * xmlParser인터페이스는 Private  method로 OpenAssetByPath와 CloseAssetsAndBuffer를 가지고 있는데
 * xmlParser는 DoParsing()을 통해 인터페이스가 묶여져 있는데 DeleteParser와 InsertParser는 이 DoParsing을 실체화 한다.
 * SetupXMLPath()함수를 통해 xmlPath를 입력하고 asset을 Open 해둔다.
 */

typedef std::string xmlPath;

class npXMLParser{
public:
	//void ParseTextureDataByPath(xmlPath path);
	virtual void SetupXMLPath(xmlPath path);
	virtual void SetupApkPath(std::string apkPath);
	virtual void DoParsing()=0;

	npXMLParser(JavaVM* javaVM, jobject* aAssetManager);
	virtual ~npXMLParser();
private:
	virtual char* OpenAssetsByPath(xmlPath path);
	virtual void CloseAssetsAndBuffer(char* buffer);

protected:
	npXMLParser();

	char* xmlTextBuffer;

	//JNIEnv* env;
	JavaVM* javaVm;
	jobject* assetManager;
	AAsset* assets;

	//libzip Testing
	zip* apkArchive;
	zip_file* apkZipFile;
};

#endif /* NPXMLPARER_H_ */
