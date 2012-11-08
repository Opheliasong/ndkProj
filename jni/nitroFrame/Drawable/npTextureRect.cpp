/*
 * npTextureRect.cpp
 *
 *  Created on: 2012. 11. 3.
 *      Author: Ophelia
 */

#include "npTextureRect.h"

npTextureRect::npTextureRect(){
	this->vertex[0] = -1.0f;
	this->vertex[1] =  1.0f;
	this->vertex[2] = 0;

	//왼쪽아래
	this->vertex[3] = -1.0f;
	this->vertex[4] = -1.0f;
	this->vertex[5] = 0;

	//오른쪽 위
	this->vertex[6] = 1.0f;
	this->vertex[7] = 1.0f;
	this->vertex[8] = 0;

	//오른쪽 아래
	this->vertex[9] = 1.0f;
	this->vertex[10] = -1.0f;
	this->vertex[11] = 0;

	LOGE("Call CTOR");
}

npTextureRect::npTextureRect(screenplayTag TAG){
	this->tag = TAG;
	this->sprtie = npContainerDAO::GetInstance().getSpriteByTAG(this->tag);
	//왼쪽 위
	this->vertex[0] = -1.0f;
	this->vertex[1] =  1.0f;
	this->vertex[2] = 0;

	//왼쪽아래
	this->vertex[3] = -1.0f;
	this->vertex[4] = -1.0f;
	this->vertex[5] = 0;

	//오른쪽 위
	this->vertex[6] = 1.0f;
	this->vertex[7] = 1.0f;
	this->vertex[8] = 0;

	//오른쪽 아래
	this->vertex[9] = 1.0f;
	this->vertex[10] = -1.0f;
	this->vertex[11] = 0;
}

npTextureRect::npTextureRect(const npTextureRect& source){
	this->tag = source.tag;
	this->sprtie = source.sprtie;
	this->height = source.height;
	this->width = source.width;
	this->positionX = source.positionX;
	this->positionY = source.positionY;

	//this->vertex
	memcpy(this->vertex,source.vertex,12*sizeof(GLfloat));
}

npTextureRect::~npTextureRect(){
}

void npTextureRect::PreSettingDraw() {
	npContainerDAO::GetInstance().BindingFrameBySprite(*this->sprtie);
}

void npTextureRect::DrawThis() {

	glPushMatrix();
	glVertexPointer(3, GL_FLOAT, 0,this->vertex);
	glTranslatef(this->positionX, this->positionY , 0.f);
	glScalef(this->height, this->width, 1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glPopMatrix();

}

void npTextureRect::SetSize(int width, int Height) {
	this->width = width;
	this->height = Height;
}

/***
 * @fn TextureRect의 TextureTAG를 입력하고 Factory에서 이 태그와 동일한 Sprite를 받아온다.
 * @param TAG screenplay의 TAG
 * @return TAG의 값이 정확한지 여부를 알려주는 bool객체를 리턴한다
 * @details TAG를 설정함과 동시에 ContainerDAO 측을 통해서 Sprite객체를 받아 온다.
 *
 */
bool npTextureRect::SetTextureTAG(screenplayTag TAG) {
	this->sprtie = npContainerDAO::GetInstance().getSpriteByTAG(TAG);

	if(NP_IS_EMPTY(this->sprtie)){
		LOGE("not Fine Sprite Tag");
		return false;
	}
	return true;
}

sprite* npTextureRect::getSprite() {
	return this->sprtie;
}
