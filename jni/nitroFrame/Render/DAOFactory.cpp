
/*
 * DAOFactory.cpp
 *
 *  Created on: 2012. 10. 25.
 *      Author: Ophelia
 */

#include "DAOFactory.h"

	/***
	 * @author 박성민
	 */
sprite* npFactoryDAO::getSpriteByTAG(screenplayTag tag) {
	screenMapIter iter;
	screenplayMap* screenplayContainer = &screenplayStore::getInstance().screenplayContainer;

	iter = screenplayContainer->find(tag);
	if(iter != screenplayContainer->end()){
		sprite* retSprite = new sprite;
		retSprite->screenplay = &iter->second;
		retSprite->currentScreenplay = retSprite->screenplay;
//		LOGI("Done Copy Sprite Node");
		return retSprite;
	}else{
		LOGI("Not Copy Sprite, Not find TAG");
		LOGI("TAG Name Is : %s",tag.c_str());
		return NULL;
	}
}

npFactoryDAO::npFactoryDAO() {
}

npFactoryDAO::~npFactoryDAO() {
}
