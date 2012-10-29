/*
 * FramePalette.cpp
 *
 *  Created on: 2012. 10. 23.
 *      Author: Ophelia
 */

#include "FramePalette.h"
//npFramePalette* npFramePalette::PaletteInstance = NULL;

UVPacket::UVPacket(){

}

UVPacket::~UVPacket(){

}

UVPacket& UVPacket::operator= (const UVPacket& rightSide){
	/**
	 * @fn UVPacket& UVPacket::operator=(const UVPacket& rightSide)
	 * @brief UVPacket의 Deep Copy를 실행하는 대입 연산자
	 * @details textureID는 int로 단일 복사하고, UV Array는 GLfloat의 8개 만큼 복사한다.
	 * @param const UVpacket& rightSide 대입연산자의 우측항
	 * @return UVPacket& 복사연산자의 좌측항
	 *
	*/
	this->bindTextureID = rightSide.bindTextureID;
	memcpy(this->texture, rightSide.texture, 8*sizeof(GLfloat));
}

npFramePalette& npFramePalette::getInstance(){
	static npFramePalette singleTone;
	return singleTone;
}

npFramePalette::npFramePalette(){

}

npFramePalette::~npFramePalette(){
	this->FrameContainer.clear();
	LOGI("Terminate Frame Palette");
}
