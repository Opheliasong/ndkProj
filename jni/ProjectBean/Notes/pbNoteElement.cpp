/*
 * pbNoteElement.cpp
 *
 *  Created on: 2012. 11. 26.
 *      Author: Ophelia
 */

#include "pbNoteElement.h"
#include "pbNoteProcessor.h"

pbNoteElement::pbNoteElement() {
	m_bEndPhase = false;
	noteState = NOTECREATE;

	BodyActor = 0;
	Effector = 0;
	DeadActor = 0;
}

pbNoteElement::~pbNoteElement() {
	NP_SAFE_DELETE(BodyActor);
	NP_SAFE_DELETE(Effector);
	NP_SAFE_DELETE(DeadActor);
}

bool pbNoteElement::setBaseTextureTAG(screenplayTag TAG) {
	this->BodyActor->SetTextureTAG(TAG);

	if(NP_IS_EMPTY(this->BodyActor)){
		return false;
	}
	return true;
}

bool pbNoteElement::setEffectTextureTAG(screenplayTag TAG) {
	this->Effector->SetTextureTAG(TAG);

	if(NP_IS_EMPTY(this->Effector)){
		return false;
	}
	return true;
}

bool pbNoteElement::setDeadEffectSprite(screenplayTag TAG) {
	this->DeadActor->SetTextureTAG(TAG);

	if(NP_IS_EMPTY(this->DeadActor)){
		return false;
	}
	return true;
}

void pbNoteElement::setNotePosition(float x, float y) {
	this->positionX = x;
	this->positionY = y;
}

void pbNoteElement::setNoteSize(float width, float height) {
	this->noteWidth = width;
	this->noteHeight = height;
	this->BodyActor->SetSize(width,height);
}

void pbNoteElement::ResetNoteState() {
	positionX = 0.f;
	positionY = 0.f;
	noteWidth = 0.f;
	noteHeight = 0.f;
	onEffectMode = false;
	m_bEndPhase = false;
	noteState = NONE;
}

void pbNoteElement::ReleaseNote() {
	pbNoteProcessor::GetInstance()->RequestRemoveNote(this);

	//Touch Layer에서 Remove
	TouchLayer::GetInstance().RemovedObserver(this);
}

void pbNoteElement::setGlTranslateByPosition() {
	glTranslatef(this->positionX,this->positionY,0.f);
}

void pbNoteElement::prepareDrawUnitRendering() {
	//glPushMatrix();
	//setGlTranslateByPosition();
}

bool pbNoteElement::IsHitThis() {
	float left = this->positionX - this->noteWidth/2;
	float right = this->positionX + this->noteWidth/2;
	float top = this->positionY + this->noteHeight/2;
	float bottom = this->positionY - this->noteHeight/2;

	int TouchPointX = TouchLayer::GetInstance().pointX;
	int TouchPointY = TouchLayer::GetInstance().pointY;

	//TODO 해상도의 Width에 맞추어서 Remaster 할 수 있게 하자.
//	float RemasteredY = 480.f - TouchPointY;

	if(TouchPointX > left && TouchPointX < right){
		if(TouchPointY < top && TouchPointY > bottom){
			return true;
		}
	}
	return false;
}

void pbNoteElement::NoteIsDead() {
	this->m_bEndPhase = true;
}




