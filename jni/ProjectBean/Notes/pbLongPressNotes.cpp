/*
 * pbLongPressNotes.cpp
 *
 *  Created on: 2012. 11. 27.
 *      Author: Ophelia
 */

#include "pbLongPressNotes.h"

pbLongPressNotes::pbLongPressNotes() {
	noteType = LONGPRESS;

	this->pressTime = 0;
	this->RequirePressTouchTime = 1000.f;

	this->BodyActor = new npTextureRect("knight");
	this->BodyActor->SetPosition(&this->positionX, &this->positionY);

	this->TargetMarker = new npTextureRect("target");
	this->TargetMarker->SetPosition(&this->positionX, &this->positionY);

	this->DeadActor = new npTextureRect("run");
	this->DeadActor->SetPosition(&this->positionX, &this->positionY);
}

pbLongPressNotes::~pbLongPressNotes() {
}

void pbLongPressNotes::PreSettingDraw() {
	this->BodyActor->SetSize(this->noteWidth, this->noteHeight);
//	LOGE("%d: Size) W:%f, H:%f",this->m_RentalIndex,this->noteWidth,this->noteHeight);
	this->TargetMarker->SetSize(this->TargetMarkWidth * m_fScale, this->TargetMarkHeight * m_fScale);
	this->DeadActor->SetSize(this->noteWidth, this->noteHeight);
}

void pbLongPressNotes::DrawThis() {

	if(!m_bEndPhase){

		//1-1) Body Sprite Rendering
		npRenderprocess::getInstance().DoDraw(*this->BodyActor);

		//1-2) Effect Sprite Rendering
		if(onEffectMode){
			npRenderprocess::getInstance().DoDraw(*this->Effector);
		}

		if(targetingIndicate){
			npRenderprocess::getInstance().DoDraw(*this->TargetMarker);
		}

	}else{
		npRenderprocess::getInstance().DoDraw(*this->DeadActor);
		if(this->DeadActor->IsFinish()){
			ReleaseNote();
			return;
		}
	}
}

/***
 * @fn 터치 이벤트가 발생되면 Call 될 logic
 */
void pbLongPressNotes::notify() {
	if(TouchLayer::GetInstance().touchFlag == 0 || TouchLayer::GetInstance().touchFlag == 2){
		if(IsHitThis()){
			//Targeting 이 On 되었는지 안되었는지 확인
			if(targetingIndicate == true){
				//Touch Event가 Down or  move 이면서, 현재의 Rect내에 Touch Point가 존재한다면, pressOn을 True로 켜둔다.
			}else{
				//TODO Targeting On이 되지 않았을 경우 Comobo Reset 처리
				pbComboManager::GetInstance()->ResetCombo();
			}
			this->pressOn = true;
		}else{
			this->pressOn = false;
		}
	}else{
		this->pressOn = false;
	}
}

void pbLongPressNotes::onTimeAlerts() {
}

void pbLongPressNotes::Update(float fTime) {
//	float x = positionX + pbGlobalInGameVariable::fWorldMoveX;
	//float x = positionX;
	float x = positionX - (pbStageValue::GetStageMoveSpeed()*fTime);
	float y = positionY;

	setNotePosition(x,y);
	pbTargetingNotes::Update(fTime);

	if(pressOn){
		this->pressTime += (fTime*1000);
		//PressTime이 RequirePressTouchTime 보다 긴(클) 경우 endPhase로 돌입
		if(pressTime >= RequirePressTouchTime){
			//TODO Combo처리 및 점수 처리
			if(!m_bEndPhase ) {
				pbComboManager::GetInstance()->IncreaseCombo(300);
				pbStageValue::IncreaseFeverGauge(10);
			}

			m_bEndPhase = true;
			noteState = DEAD;
		}
	}
}

void pbLongPressNotes::ResetNoteState() {
	pbNoteElement::ResetNoteState();
	pbTargetingNotes::ResetNoteState();
	this->noteWidth = 250.f;
	this->noteHeight = 150.f;
	this->pressOn = false;
	this->pressTime = 0.f;
}

