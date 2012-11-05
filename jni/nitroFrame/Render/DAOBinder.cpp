/*
 * DAOBinder.cpp
 *
 *  Created on: 2012. 10. 26.
 *      Author: Ophelia
 */

#include "DAOBinder.h"

void npBindingDAO::BindingFrameBySprite(sprite& sprite) {
	//Binding 순서
	//넘겨진 Sprite 객체의 Current Screenplay가 가진 Frame ID를 가져온다.
	int currentPlayFrameID = sprite.currentScreenplay->getKernel();

	//가져온 Frame ID에 맞추어서 FramePalette에서 GL Texture ID와 UV정보를 얻어온다.
	TextureAtlasIter textureAtlasIterator =  npAtlasMap::getInstance().FrameContainer.find(currentPlayFrameID);
	UVPacket* uvPacket = &textureAtlasIterator->second;
/*
	LOGE("Binding) uvPacket Binding ID: %d",uvPacket->bindTextureID);
	LOGE("Binding) uvPacket Texture[0] :%f",uvPacket->texture[0]);
	LOGE("Binding) uvPacket Texture[1] :%f",uvPacket->texture[1]);
	LOGE("Binding) uvPacket Texture[2] :%f",uvPacket->texture[2]);
	LOGE("Binding) uvPacket Texture[3] :%f",uvPacket->texture[3]);
	LOGE("Binding) uvPacket Texture[4] :%f",uvPacket->texture[4]);
	LOGE("Binding) uvPacket Texture[5] :%f",uvPacket->texture[5]);
	LOGE("Binding) uvPacket Texture[6] :%f",uvPacket->texture[6]);
	LOGE("Binding) uvPacket Texture[7] :%f",uvPacket->texture[7]);
*/
	//OpenGL API를 이용하여 Texture 정보를 Bind한다.
	glBindTexture(GL_TEXTURE_2D,  uvPacket->bindTextureID );
	glTexCoordPointer(2,GL_FLOAT, 0,  uvPacket->texture);	//텍스처좌표(UV) 배열 넘겨주기
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//Bind가 완료되었으면 sprite의 CP를 다음으로 이동시킨다.
	sprite.ReadyForNextScreenplay();
}

npBindingDAO::npBindingDAO() {
}

npBindingDAO::~npBindingDAO() {
}
