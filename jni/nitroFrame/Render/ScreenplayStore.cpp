/*
 * ScreenplayStore.cpp
 *
 *  Created on: 2012. 10. 24.
 *      Author: Ophelia
 */

#include "ScreenplayStore.h"

screenplayStore& screenplayStore::getInstance(){
	static screenplayStore singletone;
	return singletone;
}

screenplayStore::screenplayStore(){

}

screenplayStore::~screenplayStore(){
	this->screenplayContainer.clear();
	LOGI("Terminate Screenplay Sotre");
}
