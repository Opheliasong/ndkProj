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

/***
 * @fn ContainerDAO에서 넘겨 받는 Tag를 기반으로 ScreenplayStore와 FramePalette내용을 삭제 한다.
 * @param deleteTag
 * @details screenplayTag를 기반으로 ScreenplayStore의 내용을 가져오고, 이 새부 내용들을 기반으로 FramePalette를 삭제 한다.
 * 그 후 ScreenPlayStore의 내용도 삭제 한다.
 * 이 함수는 실제로 Container에게 삭제를 명령하게 된다.
 */
void npDeleteDAO::DeleteFrameByTag(const screenplayTag& deleteTag) {
	screenMapIter iterator = screenplayStore::getInstance().screenplayContainer.find(deleteTag);

	//만약 Tag를 찾지 못하였을 경우
	if(IsFindingTagFromScreenplayStore(iterator)){
		//태그를 찾지 못하였을 경우 그냥 생략
		return;
	}
	//Tag를 찾았을 경우
	else{
		//Iterator에 있는 second 내용을 가지고 FramePalette의 내용을 삭제한다.
		//Iterator의 second의 내용을 가지고 List Loop를 돌린다.
		//루프를 돌면서 Iterator의 second내용은 FramePalette의 first Key이다.
		npLinkNode<int>* screenList = &iterator->second;

		screenplays* screenplayIterator = screenList->getNext();
		//TODO 하위 로직 제작 (STL 에러로 인해 잠시 중단

		for(;screenplayIterator != screenList; screenplayIterator = screenplayIterator->getNext()){
 			TextureAtlasIter iterator = npAtlasMap::getInstance().FrameContainer.find(screenList->getKernel());
			npAtlasMap::getInstance().FrameContainer.erase(iterator);
		}

		//FramePalette의 내용이 모두 삭제 되었으므로, 이제 ScreenplayStore에서 ScreenplayIterator를 삭제한다.
		screenplayStore::getInstance().screenplayContainer.erase(iterator);

	}
}

bool npDeleteDAO::IsFindingTagFromScreenplayStore(screenMapIter& iterator) {
	//return iterator == screenplayStore::getInstance().screenplayContainer.end();
	return true;
}
