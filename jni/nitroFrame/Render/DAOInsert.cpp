/*
 * DAOInsert.cpp
 *
 *  Created on: 2012. 10. 23.
 *      Author: Ophelia
 */


#include "DAOInsert.h"

//TODO InserDAO는 예외처리 부분을 생략하고 제작하였다. 이를 처리해주어야 프로그램의 안정성이 높아진다.

/**
 * @fn XML Parser와 TextureGenerater에서 작성된 Data를 분석하여 Container에게 삽입하는 로직 실행하게 하는 Interface
 * @param RawPacket UV Array정보 + TAG + TextureBind Index 로 구성되어져 있는 Struct
 * @details RawData중 TAG 와 ID List를 수렴하는 ScreenplayContainer와 Texture ID+UV Array로 구성이 된 FrameStore에게 데이터를 삽입하는 Interface이다.
 */

void npInsertDAO::SendRawSourceByModule(const RawDataPacket& rawSource) {
	UVPacket uvPackets;
	ScreenplayPacket screenplayPacket;

	//Raw Source Decoding 하는 로직
	ParsingRawSource(rawSource,uvPackets,screenplayPacket);

	//TexutreAtlasMap에 insert 하는 로직
	InsertUVPacket(uvPackets, screenplayPacket.screenplayID);

	//ScreenPlay에 Insert 하는 로직
	InsertScreenplayPackets(screenplayPacket);
}

/***
 * @fn Rawsource를 기반으로 UvPacket과 ScreenplayPacket을 분석하여 데이터를 입력하는 함수
 * @param rawSource xmlParser에게서 입력된 RawDataPacket
 * @param outputUvPacket RawSource를 기반으로 데이터가 삽입되어질 UvPacket변수
 * @param screenOutputPacket RawSource를 기반으로 데이터가 삽입되어질 ScreenplayPacket변수
 * @details
 */
void npInsertDAO::ParsingRawSource(const RawDataPacket& rawSource, UVPacket& outputUvPacket, ScreenplayPacket& screenOutputPacket) {
	screenOutputPacket.tag = rawSource.tag;
	outputUvPacket.bindTextureID = rawSource.textureBindIndex;

	CalcUVArrayByRawSource(rawSource,outputUvPacket);		//UV Array Calculate
	screenOutputPacket.screenplayID = GetFramewIndex();
}

/***
 * @fn TexutreAtlasMap에 등록하기 위한 First를 얻기 위한 함수
 * @return TexutreAtlasMap의 First Key
 */
/*TODO Index 키를 관리하기 하기 위한 Logic을 추가 해주자.
 *지금은 Static int를 이용해서-2,147,483,648 ~ 2,147,483,647 까지 가능하다.
 */
int npInsertDAO::GetFramewIndex() {
	static int Index=-1;
	return ++Index;
}

/***
 * @fn UvPacket을 기반으로 TexutreAtlasMap에 삽입하는 함수
 * @param UvPackets 삽이되어질 UvPacket
 */
void npInsertDAO::InsertUVPacket(UVPacket& UvPackets,int UvKeys) {
	//npTexutreAtlasMap* paletteInstance;
	//TODO DAO 자체에서 Insert Frame ID를 생성한다면 이 생성 Key를 FramePair의 First에 넣어주어야 한다.
	npAtlasMap::getInstance().FrameContainer.insert(TextureAtlasPair(UvKeys,UvPackets));
}

/***
 * @fn ScreenplayStore에 screenplayPacket을 Insert한다.
 * @param screenplayPacket 삽입할 screenplayPacket
 */
void npInsertDAO::InsertScreenplayPackets(ScreenplayPacket& screenplayPacket) {
	screenplayStore* screenplayStore = &screenplayStore::getInstance();

	screenMapIter screenplayIter = screenplayStore->screenplayContainer.find(screenplayPacket.tag);


	if(screenplayIter == screenplayStore->screenplayContainer.end()){
		//TODO Pair의 Second인 List를 추가해야 한다
		npLinkNode<int>* insertSceond = new npLinkNode<int>;
		insertSceond->setKernel(screenplayPacket.screenplayID);
		screenplayStore->screenplayContainer.insert(ScreenplayPair(screenplayPacket.tag, *insertSceond));

		LOGI("Insert Dummy");
	}else{
		npLinkNode<int>::addTail(screenplayPacket.screenplayID , &screenplayIter->second);
		LOGI("Add List Dummy");
	}
}


/***
 * @fn Raw Source를 기반으로 outputPacket의 UV Array를 설정하는 함수
 * @param rawSource outputPacket에 삽입할 RawData 를 가지고 있는 RawDataPacket
 * @param outputPacket RawData를 기반으로 UV Array를 삽입되어질 UVPacket을 말한다.
 */
void npInsertDAO::CalcUVArrayByRawSource(const RawDataPacket& rawSource,UVPacket& outputPacket) {
	outputPacket.bindTextureID = rawSource.textureBindIndex;

	outputPacket.texture[0] = rawSource.x/ rawSource.textureWidth;
	outputPacket.texture[1] = rawSource.y / rawSource.textureHeight;

	outputPacket.texture[2] = rawSource.x / rawSource.textureWidth;
	outputPacket.texture[3] = (rawSource.y + rawSource.height) / rawSource.textureHeight;

	outputPacket.texture[4] = (rawSource.x + rawSource.width) / rawSource.textureWidth;
	outputPacket.texture[5] = rawSource.y / rawSource.textureHeight;

	outputPacket.texture[6] = (rawSource.x + rawSource.width) / rawSource.textureWidth;
	outputPacket.texture[7] = (rawSource.y + rawSource.height) / rawSource.textureHeight;
}

void npInsertDAO::InsertDummyRawSource(){
	/*
	npAtlasMap* paletteInstance;
	paletteInstance = &npAtlasMap::getInstance();

	UVPacket dummy;
	dummy.bindTextureID = 0;
	GLfloat dummyArray[8] = {0.f ,0.f ,0.f ,0.f ,0.f, 0.f, 0.f, 0.f};
	memcpy(dummy.texture, dummyArray, 8*sizeof(GLfloat));
	paletteInstance->FrameContainer.insert(TextureAtlasPair(0, dummy));

	dummy.bindTextureID = 1;
	GLfloat dummyArray1[8] = {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f};
	memcpy(dummy.texture, dummyArray1, 8*sizeof(GLfloat));
	paletteInstance->FrameContainer.insert(TextureAtlasPair(1, dummy));

	dummy.bindTextureID = 3;
	GLfloat dummyArray2[8] = {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f};
	memcpy(dummy.texture, dummyArray1, 8*sizeof(GLfloat));
	paletteInstance->FrameContainer.insert(TextureAtlasPair(2, dummy));

	//Screenplay Dummy code
	screenplayStore* screenplayStore;
	screenplayStore = &screenplayStore::getInstance();

	screenplayTag dummyTag1 = "DUMMY1_Screenplay";
	npLinkNode<int>* dummyScreenplay = new npLinkNode<int>;
	int dummyFrameNumber = 1;
	dummyScreenplay->setKernel(dummyFrameNumber);

	screenMapIter screenplayIter = screenplayStore->screenplayContainer.find(dummyTag1);

	if(screenplayIter == screenplayStore->screenplayContainer.end()){
		screenplayStore->screenplayContainer.insert(ScreenplayPair(dummyTag1, *dummyScreenplay));
		LOGI("Insert Dummy");
	}else{
		//npLinkNode<int>::addTail(dummyFrameNumber, &screenplayIter->second);
		LOGI("Add List Dummy");
	}

	screenplayTag dummyTag2 = "DUMMY1_Screenplay";
	npLinkNode<int>* dummyScreenplay2 = new npLinkNode<int>;
	int dummyFrameNumber2 = 2;
	dummyScreenplay2->setKernel(dummyFrameNumber2);

	screenMapIter screenplayIter2 = screenplayStore->screenplayContainer.find(dummyTag2);

	if(screenplayIter2 == screenplayStore->screenplayContainer.end()){
		screenplayStore->screenplayContainer.insert(ScreenplayPair(dummyTag2, *dummyScreenplay2));
	}else{
		npLinkNode<int>::addTail(dummyFrameNumber2, &screenplayIter2->second);
		LOGfloatString("Insert Second Dummy Screenplay Value:",dummyScreenplay2->getKernel());
	}

	delete dummyScreenplay;
	delete dummyScreenplay2;
	*/
}

void npInsertDAO::DeleteDummyRawSource(){
	/*
	npAtlasMap* paletteInstance;
	paletteInstance = &npAtlasMap::getInstance();

	npAtlasMap iter = paletteInstance->FrameContainer.find(1);
	if(iter != paletteInstance->FrameContainer.end()){
		paletteInstance->FrameContainer.erase(iter);
	}
	*/
}

/**
 *
 */
void npInsertDAO::PrintDummyRawSource(){
	/*
	npAtlasMap* paletteInstance;
	paletteInstance = &npAtlasMap::getInstance();

	TextureAtlasIter iter;
	for(iter = paletteInstance->FrameContainer.begin(); iter != paletteInstance->FrameContainer.end(); ++iter){
		LOGfloatString("Frame ID :",iter->first);
	}

	screenplayStore* screenplayStore = &screenplayStore::getInstance();
	//screenplayStore->screenplayContainer.clear();

	sprite* dummySprite = npFactoryDAO::getSpriteByTAG("DUMMY1_Screenplay");

	if(dummySprite == NULL){
		LOGE("Not get Sprite Dummy");
	}else{
		LOGfloatString("Dummy Sprite ID: ",dummySprite->currentScreenplay->getKernel());
		LOGfloatString("Dummy Sprite ID: ",dummySprite->currentScreenplay->getNext()->getKernel());
		LOGfloatString("Dummy Sprite ID: ",dummySprite->currentScreenplay->getNext()->getNext()->getKernel());
		LOGfloatString("Dummy Sprite ID: ",dummySprite->screenplay->getKernel());
	}
	*/
}

npInsertDAO::npInsertDAO(){

}


npInsertDAO::~npInsertDAO(){

}


