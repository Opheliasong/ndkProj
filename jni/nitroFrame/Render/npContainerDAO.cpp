/*
 * npContainerDAO.cpp
 *
 *  Created on: 2012. 10. 29.
 *      Author: Ophelia
 */

#include "npContainerDAO.h"

char* npContainerDAO::apkPath = NULL;
JNIEnv* npContainerDAO::env = NULL;
jobject npContainerDAO::AssetManager;

/**
 * @fn RawDataPacket을 받아와서 RawPacket의 내용을 InsertModule에 전달하는 작업을 한다.
 * @param rawPacket Insert하게 될 UV Data를 말한다.
 */
void npContainerDAO::InsertUVData(const RawDataPacket& rawPacket){
	/*
	if(NP_IS_EMPTY(rawPacket)){
		LOGE("npContainerDAO) Insert Non RawDataPacket!");
		return;
	}*/
	InsertModule->SendRawSourceByModule(rawPacket);
}

/**
 * @fn TAG를 가지고 ScreenplayStore에서 Sprite를 가져오기 위한 함수를 말한다.
 * @param tag ScreenplayStore에서 가져올 식별자(tag)
 * @return  Factory에서 생성하거나 만들어진 Sprite.
 */
sprite* npContainerDAO::getSpriteByTAG(screenplayTag tag) {
	return FactoryModule->getSpriteByTAG(tag);
}

/**
 * @fn ContainerDAO 에게 DeleteModule을 실행하기 위한 함수.
 * @details DeleteModule은 Scene에서 사용한 XML을 이용해서 FramePalette와 ScreenplayStore의 내용을 삭제 하게 된다.
 */

/**
 * @fn sprite를 Binding하기 위해 사용되는 함수를 말한다.
 * @brief Vertex를 그리기 이전에 UV의 셋팅을 위해서 사용되는 함수로, Container가 넘겨주는 Sprite객체를 이용한다.
 * @param sprite
 * @details sprite객체를 이용하여 Binding하게 되는데 사용자(또는 Drawalbe객체)는 자신이 가진 sprite객체를 넘겨주면 된다.
 * 모든 통신은 Sprite를 통해서 하게 된다.
 */
void npContainerDAO::BindingFrameBySprite(sprite& sprite) {
	TextureBindingModule->BindingFrameBySprite(sprite);
}

/**
 * @fn npContainerDAO의 Ctor이다.
 * @details ContainerDAO의 생성자에서는 자신이 가진 내부 모둘들을 생성하게 된다.
 * @warning GetInstance()의 호출전에 apkPath가 설정되어 있어야 한다.
 */
npContainerDAO::npContainerDAO() {
	//TODO Try catch를 통한 에러 처리가 필요하다
	TextureBindingModule = new npBindingDAO;
	FactoryModule = new npFactoryDAO;
	DeleteModule = new npDeleteDAO;
	InsertModule = new npInsertDAO;
	InsertXMLParser = new npInsertParser(env,&AssetManager);
	DeleteXMLParser = new npDeleterParser(env,&AssetManager);
	TextureGenerator = new npTextureGenerator(apkPath);
}

/**
 * @fn ContainerDAO의 Instance를 받아오는 Static Method.
 * @return 실제 ContainerDAO 객체
 * @details static variable 을 이용하여 객체를 생성하고 삭제 하게 된다, Mayer Singleton을 사용하는 방식이다.
 */
npContainerDAO& npContainerDAO::GetInstance() {
	static npContainerDAO Instance;
	return Instance;
}

/***
 * @fn XMLParser에서 호출하는 Interface로, rawDataPacket을 넘겨주어서
 * @param rawPacket xmlParser에서 넘겨주는 RawDataPacket 데이터이다.
 * @details rawPacket을 DeleteModule로 넘겨주어서 Tag기반으로 Screenplay와, FramePalette의 내용을 제거한다.
 */
void npContainerDAO::DeleteUVData(const screenplayTag& deleteTag){
	DeleteModule->DeleteFrameByTag(deleteTag);
}

/***
 * @fn Scene 관리 쪽에서 Texture XML  Path를 보내주어서 InsertParser의 기능을 활성화 시킨다.
 * @param xmlPath xml의 경로
 * @brief Scene쪽에서 Data를 Container에게 불러오게 하기 위한 Interface.
 */
void npContainerDAO::LoadTextureByXMLpath(const char* xmlPath) {
	LOGE("Load Texture By XML Path: %s",xmlPath);
	InsertXMLParser->SetupXMLPath(xmlPath);
	LOGE("Done Setup XML path");
	InsertXMLParser->DoParsing();
}

/***
 * @fn  Scene 관리 쪽에서 Texture XML Path를 보내주어서 DeleteParser의 기능을 활성화 시킨다.
 * @param xmlPath 제거할 texture의 path 경로
 * @details deleteParser의 기능 중 ScreenplayStore와 FramePalette의 내용만 삭제하게 한다.
 */
void npContainerDAO::DeleteTextureByXMlpath(const char* xmlPath) {
	DeleteXMLParser->SetupXMLPath(xmlPath);
	DeleteXMLParser->DoParsing();
}

void npContainerDAO::SetupTheAPKPath(char* aApkPath) {
	apkPath = aApkPath;
}

/***
 *
 * @param aEnv
 * @param aAssetManager
 * @param aApkPath
 */
void npContainerDAO::SetupBaseInitialize(JNIEnv* aEnv, jobject& aAssetManager,char* aApkPath){
	env = aEnv;
	AssetManager = aAssetManager;
	apkPath = aApkPath;
}


GLuint npContainerDAO::GenerateTextureByPNGPath(const char* textureName) {
	TextureGenerator->GenerateTextureByPNGPath(textureName);
}

npTextureGenerator* npContainerDAO::getTextureGenerator() {
	return this->TextureGenerator;
}

/***
 *
 */
npContainerDAO::~npContainerDAO() {
	delete TextureBindingModule;
	delete FactoryModule;
	delete DeleteModule;
	delete InsertModule;
}

