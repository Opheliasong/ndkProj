/*
 * DAOBinder.cpp
 *
 *  Created on: 2012. 10. 26.
 *      Author: Ophelia
 */

#include "DAOBinder.h"

void DAOBinder::BindingFrameBySprite(sprite& sprite) {
	//Binding 순서
	//넘겨진 Sprite 객체의 Current Screenplay가 가진 Frame ID를 가져온다.
	int currentPlayFrameID = sprite.currentScreenplay->getKernel();

	//가져온 Frame ID에 맞추어서 FramePalette에서 GL Texture ID와 UV정보를 얻어온다.
	UVPacket* uvPacket = npFramePalette::getInstance().FrameContainer.find(currentPlayFrameID);

	//OpenGL API를 이용하여 Texture 정보를 Bind한다.
	glBindTexture(GL_TEXTURE_2D,  uvPacket->bindTextureID );
	glTexCoordPointer(2,GL_FLOAT, 0,  uvPacket->texture);	//텍스처좌표(UV) 배열 넘겨주기
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//Bind가 완료되었으면 sprite의 CP를 다음으로 이동시킨다.
	sprite.ReadyForNextScreenplay();
}

DAOBinder::DAOBinder() {
}

DAOBinder::~DAOBinder() {
}
