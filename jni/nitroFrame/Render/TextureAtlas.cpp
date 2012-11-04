/*
 * FramePalette.cpp
 *
 *  Created on: 2012. 10. 23.
 *      Author: Ophelia
 */

#include "TextureAtlas.h"
//npFramePalette* npFramePalette::PaletteInstance = NULL;

npAtlasMap& npAtlasMap::getInstance(){
	static npAtlasMap singleTone;
	return singleTone;
}

npAtlasMap::npAtlasMap(){

}

npAtlasMap::~npAtlasMap(){
	this->FrameContainer.clear();
	LOGI("Terminate Frame Palette");
}
