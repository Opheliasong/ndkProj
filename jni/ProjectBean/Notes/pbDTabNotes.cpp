/*
 * pbDTabNotes.cpp
 *
 *  Created on: 2012. 11. 27.
 *      Author: Ophelia
 */

#include "pbDTabNotes.h"

pbDTabNotes::pbDTabNotes() {
	noteType = DTABNOTE;

	this->BodyActor = new npTextureRect("armored");
	this->BodyActor->SetPosition(&this->positionX, &this->positionY);

	this->TargetMarker = new npTextureRect("target");
	this->TargetMarker->SetPosition(&this->positionX, &this->positionY);

	this->DeadActor = new npTextureRect("run");
	this->DeadActor->SetPosition(&this->positionX, &this->positionY);

	this->HelmetObject = new npTextureRect("ci");
	this->HelmetObject->SetPosition(&this->helmetXposition, &this->helmetYposition);
}

pbDTabNotes::~pbDTabNotes() {
	NP_SAFE_DELETE(HelmetObject);
}

void pbDTabNotes::PreSettingDraw() {
	this->BodyActor->SetSize(this->noteWidth, this->noteHeight);
	this->TargetMarker->SetSize(this->TargetMarkWidth, this->TargetMarkHeight);
	this->HelmetObject->SetSize(this->helmetWidth, this->helmetHeight);
	this->DeadActor->SetSize(this->noteWidth, this->noteHeight);
}

void pbDTabNotes::DrawThis() {

	if(!m_bEndPhase){
		//1-1) Body Sprite Rendeirng
		npRenderprocess::getInstance().DoDraw(*this->BodyActor);

		//1-2) Helmet이 있을 경우 헬멧 렌더링
		if(this->DTabTouchIndicate == false){
			npRenderprocess::getInstance().DoDraw(*this->HelmetObject);
		}

		//1-3)Effector Sprite Rendering
		if(onEffectMode){
			npRenderprocess::getInstance().DoDraw(*this->Effector);
		}

		//1-4) Targeting이 되었을 경우 Targeting Sprite Rendering
		if(this->targetingIndicate){
			npRenderprocess::getInstance().DoDraw(*this->TargetMarker);
		}
	}else{
		//DeadActor의 Rendering 및 뒷정리 작업
		npRenderprocess::getInstance().DoDraw(*this->DeadActor);

		if(this->DeadActor->IsFinish()){
			ReleaseNote();
			return;
		}
	}
}

void pbDTabNotes::notify() {

	if(TouchLayer::GetInstance().touchFlag == 0){
		if(IsHitThis()){
			//Targeting 이 On 되었는지 안되었는지 확인
			if(this->DTabTouchIndicate == false){
				if(targetingIndicate == true){
				}else{
					//TODO Targeting On이 되지 않았을 경우 Comobo Reset 처리
				}
				this->DTabTouchIndicate = true;
			}else if(this->DTabTouchIndicate == true){
				m_bEndPhase = true;
				noteState = DEAD;
			}
		}
	}
}

void pbDTabNotes::onTimeAlerts() {
}

void pbDTabNotes::Update(float fTime) {
	//float x = this->positionX + pbGlobalInGameVariable::fWorldMoveX;
	float x = positionX - (200.f*fTime);
	float y = this->positionY;

	setNotePosition(x,y);
	pbTargetingNotes::Update(fTime);
	if(this->DTabTouchIndicate == true){
	//TODO 첫번째 터치가 발생 하였을 경우 헬멧의 움직임을 처리한다.

	}else{
		setNotePosition(x,y);
	}
}

void pbDTabNotes::setNotePosition(float x, float y) {
	this->positionX = x;
	this->positionY = y;

	this->helmetXposition = x;
	this->helmetYposition = y+10.f;
}

void pbDTabNotes::ResetNoteState() {
	pbNoteElement::ResetNoteState();
	pbTargetingNotes::ResetNoteState();
	this->noteWidth = 110.f;
	this->noteHeight = 110.f;
	this->helmetWidth = 90.f;
	this->helmetHeight = 60.f;
	this->DTabTouchIndicate = false;
}


