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
	this->RequirePressTouchTime = 1500.f;

	this->BodyActor = new npTextureRect("ci");
	this->BodyActor->SetPosition(&this->positionX, &this->positionY);

	this->TargetMarker = new npTextureRect("ci");
	this->TargetMarker->SetPosition(&this->positionX, &this->positionY);

	this->DeadActor = new npTextureRect("run");
	this->DeadActor->SetPosition(&this->positionX, &this->positionY);
}

pbLongPressNotes::~pbLongPressNotes() {
	// TODO Auto-generated destructor stub
}

void pbLongPressNotes::PreSettingDraw() {
	this->BodyActor->SetSize(this->noteWidth, this->noteHeight);
	this->TargetMarker->SetSize(this->TargetMarkWidth, this->TargetMarkHeight);
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
	//1) 우선 Note의 너비와 높이 위치를 가지고 현재 터치 포인트 좌표가 노트 안에 있는지 확인 해야 한다.
	float left = this->positionX - this->noteWidth/2;
	float right = this->positionX + this->noteWidth/2;
	float top = this->positionY + this->noteHeight/2;
	float bottom = this->positionY - this->noteHeight/2;

	int TouchPointX = TouchLayer::GetInstance().pointX;
	int TouchPointY = TouchLayer::GetInstance().pointY;

	//RemasetY 의 경우 480(480 height)에게서 TouchPointY 를 감소하여 Y축을 OpenGL형태로 변환
	//RemasteredY 의 경우 TouchPointY를 OpenGL의 좌표로 변환한 변수
	float RemasteredY = 480.f - TouchPointY;

	if(TouchLayer::GetInstance().touchFlag == 0 || TouchLayer::GetInstance().touchFlag == 2){
		if(TouchPointX > left && TouchPointX < right){
			if(RemasteredY < top && RemasteredY > bottom){
				//Targeting 이 On 되었는지 안되었는지 확인
				if(targetingIndicate == true){
					//Touch Event가 Down or  move 이면서, 현재의 Rect내에 Touch Point가 존재한다면, pressOn을 True로 켜둔다.
					this->pressOn = true;
				}else{
					//TODO Targeting On이 되지 않았을 경우 Comobo Reset 처리
				}
			}
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
	float x = positionX;
	float y = positionY;

	pbTargetingNotes::Update(fTime);

	if(pressOn){
		this->pressTime += (fTime*1000);
		//PressTime이 RequirePressTouchTime 보다 긴(클) 경우 endPhase로 돌입
		if(pressTime >= RequirePressTouchTime){

			//TODO Combo처리 및 점수 처리
			m_bEndPhase = true;
		}
	}
}

void pbLongPressNotes::ResetNoteState() {
	pbNoteElement::ResetNoteState();
	pbTargetingNotes::ResetNoteState();
	this->noteWidth = 130.f;
	this->noteHeight = 130.f;
	this->pressOn = false;
	this->pressTime = 0.f;
}

