/*
 * pbNoteElement.cpp
 *
 *  Created on: 2012. 11. 26.
 *      Author: Ophelia
 */

#include "pbNoteElement.h"
#include "pbNoteProcessor.h"

GLfloat pbNoteElement::vertex[12];

pbNoteElement::pbNoteElement() {
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

	m_bEndPhase = false;
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


