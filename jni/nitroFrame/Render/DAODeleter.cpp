/*
 * DAODeleter.cpp
 *
 *  Created on: 2012. 11. 2.
 *      Author: Ophelia
 */


#include "DAODeleter.h"

npDeleteDAO::npDeleteDAO(){

}

npDeleteDAO::~npDeleteDAO(){

}


void npDeleteDAO::DeleteFrameByTag(const screenplayTag& deleteTag) {
	/***
	 * @fn ContainerDAO에서 넘겨 받는 Tag를 기반으로 ScreenplayStore와 FramePalette내용을 삭제 한다.
	 * @param deleteTag
	 * @details screenplayTag를 기반으로 ScreenplayStore의 내용을 가져오고, 이 새부 내용들을 기반으로 FramePalette를 삭제 한다.
	 * 그 후 ScreenPlayStore의 내용도 삭제 한다.
	 * 이 함수는 실제로 Container에게 삭제를 명령하게 된다.
	 */
	screenMapIter iterator = screenplayStore::getInstance().screenplayContainer.find(deleteTag);

	//만약 Tag를 찾지 못하였을 경우
	if(IsFindingTagFromScreenplayStore(iterator)){
		//태그를 찾지 못하였을 경우 그냥 생략
		LOGE("DeletetrDAO) Not Found Tag!");
		return;
	}
	//Tag를 찾았을 경우
	else{
		//Iterator에 있는 second 내용을 가지고 FramePalette의 내용을 삭제한다.
		//Iterator의 second의 내용을 가지고 List Loop를 돌린다.
		//루프를 돌면서 Iterator의 second내용은 FramePalette의 first Key이다.
		npLinkNode<int>* screenList = &iterator->second;
		screenplays* screenplayIterator = screenList->getNext();
		GLuint textureIndex;

		LOGE("Find Delete Tag!");

		//단일 Node의 Circular List일 경우 처리
		if(screenList == screenplayIterator){
 			TextureAtlasIter iterator = npAtlasMap::getInstance().FrameContainer.find(screenList->getKernel());
			npAtlasMap::getInstance().FrameContainer.erase(iterator);
			textureIndex = iterator->second.bindTextureID;
		}

		//루핑 돌리기
		for(;screenplayIterator != screenList; screenplayIterator = screenplayIterator->getNext()){
			LOGE("Looping");
 			TextureAtlasIter iterator = npAtlasMap::getInstance().FrameContainer.find(screenplayIterator->getKernel());
			textureIndex = iterator->second.bindTextureID;
			npAtlasMap::getInstance().FrameContainer.erase(iterator);
		}
		glDeleteTextures(1,&textureIndex);

		//FramePalette의 내용이 모두 삭제 되었으므로, 이제 ScreenplayStore에서 ScreenplayIterator를 삭제한다.
		//iterator의 list Container 삭제
		//npLinkNode<int>::destroyList(screenList);
		if(screenList != screenplayIterator){
			npLinkNode<int>* iterator = screenList->getNext();
			for(; iterator != screenList;){
				npLinkNode<int>* nextTarget = iterator->getPrev();
				npLinkNode<int>::destroyNode(iterator);
				iterator = nextTarget;
			}
		}
		LOGE("Delete Done");
		//실제 Map에서의 iterator삭제
		screenplayStore::getInstance().screenplayContainer.erase(iterator);
	}
	LOGE("Done Delete Frame By Tag");
}

bool npDeleteDAO::IsFindingTagFromScreenplayStore(screenMapIter& iterator) {
	return iterator == screenplayStore::getInstance().screenplayContainer.end();
	//return true;
}
