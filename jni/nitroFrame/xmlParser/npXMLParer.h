/*
 * npXMLParer.h
 *
 *  Created on: 2012. 10. 26.
 *      Author: Ophelia
 */

#ifndef NPXMLPARER_H_
#define NPXMLPARER_H_

#include "../NitroFrame.h"
#include "../include/tinyxml.h"

/***
 * @author 박성민
 * @brief Scene이 가지고 있는 xml Data를 기반으로 XML을 Parsing하여 데이터를 추출해내는 객체
 * @details xmlParser는 env객체와 assetManager가 있어야 객체를 생성 할 수 있다.
 *
 */

class npTextureRawPacket;

typedef std::string xmlPath;

class npXMLParser{
public:
	void ParseTextureDataByPath(xmlPath path);


	npXMLParser(JNIEnv* aEnv, jobject* aAssetManager);
	~npXMLParser();
private:
	char* OpenAssetsByPath(xmlPath path);
	void CloseAssetsAndBuffer(char* buffer);

	npXMLParser();

	JNIEnv* env;
	jobject* assetManager;
	AAsset* assets;

};

#endif /* NPXMLPARER_H_ */
