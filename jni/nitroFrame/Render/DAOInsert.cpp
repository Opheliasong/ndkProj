/*
 * DAOInsert.cpp
 *
 *  Created on: 2012. 10. 23.
 *      Author: Ophelia
 */


#include "DAOInsert.h"

/***
 * @author 박성민
 *
 */


void npInsertModuleDAO::InsertDummyRawSource(){

	//FramePalette Dummy Code

	npFramePalette* paletteInstance;
	paletteInstance = &npFramePalette::getInstance();

	UVPacket dummy;
	dummy.bindTextureID = 0;
	GLfloat dummyArray[8] = {0.f ,0.f ,0.f ,0.f ,0.f, 0.f, 0.f, 0.f};
	memcpy(dummy.texture, dummyArray, 8*sizeof(GLfloat));
	paletteInstance->FrameContainer.insert(FramePair(0, dummy));

	dummy.bindTextureID = 1;
	GLfloat dummyArray1[8] = {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f};
	memcpy(dummy.texture, dummyArray1, 8*sizeof(GLfloat));
	paletteInstance->FrameContainer.insert(FramePair(1, dummy));

	dummy.bindTextureID = 3;
	GLfloat dummyArray2[8] = {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f};
	memcpy(dummy.texture, dummyArray1, 8*sizeof(GLfloat));
	paletteInstance->FrameContainer.insert(FramePair(2, dummy));

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
}

void npInsertModuleDAO::DeleteDummyRawSource(){
	npFramePalette* paletteInstance;
	paletteInstance = &npFramePalette::getInstance();

	FrameContainnerIterator iter = paletteInstance->FrameContainer.find(1);
	if(iter != paletteInstance->FrameContainer.end()){
		paletteInstance->FrameContainer.erase(iter);
	}

}

/**
 *
 */
void npInsertModuleDAO::PrintDummyRawSource(){
	npFramePalette* paletteInstance;
	paletteInstance = &npFramePalette::getInstance();

	FrameContainnerIterator iter;
	for(iter = paletteInstance->FrameContainer.begin(); iter != paletteInstance->FrameContainer.end(); ++iter){
		LOGfloatString("Frame ID :",iter->first);
	}

	screenplayStore* screenplayStore = &screenplayStore::getInstance();
	//screenplayStore->screenplayContainer.clear();

	sprite* dummySprite = DAOspriteFactory::getSpriteByTAG("DUMMY1_Screenplay");

	if(dummySprite == NULL){
		LOGE("Not get Sprite Dummy");
	}else{
		LOGfloatString("Dummy Sprite ID: ",dummySprite->currentScreenplay->getKernel());
		LOGfloatString("Dummy Sprite ID: ",dummySprite->currentScreenplay->getNext()->getKernel());
		LOGfloatString("Dummy Sprite ID: ",dummySprite->currentScreenplay->getNext()->getNext()->getKernel());
		LOGfloatString("Dummy Sprite ID: ",dummySprite->screenplay->getKernel());
	}
}

npInsertModuleDAO::npInsertModuleDAO(){

}

npInsertModuleDAO::~npInsertModuleDAO(){

}
