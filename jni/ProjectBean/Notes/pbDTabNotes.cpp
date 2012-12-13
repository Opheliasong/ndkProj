/*
 * pbDTabNotes.cpp
 *
 *  Created on: 2012. 11. 27.
 *      Author: Ophelia
 */

#include "pbDTabNotes.h"

pbDTabNotes::pbDTabNotes() {
	noteType = DTABNOTE;

	this->BodyActor = new npTextureRect("ci");
	this->BodyActor->SetPosition(&this->positionX, &this->positionY);

	this->TargetMarker = new npTextureRect("ci");
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
	//	LOGE("DTab) DrawThis");

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

	//LOGE("Device Height : %d",npRenderprocess::getInstance().getDeviceHeight());
	//LOGE("posX: %f, posY: %f",this->positionX,this->positionY);
	//LOGE("wid: %f, hei: %f",this->noteWidth,this->noteHeight);
	//LOGE("-------------------------");
	//LOGE("L: %f, R : %f, T: %f, B:%f",left,right,top,bottom);
	//LOGE("x:%d, y:%f",TouchPointX,RemasteredY);
	//LOGE("===========================");
	if(TouchLayer::GetInstance().touchFlag == 0){
		if(TouchPointX > left && TouchPointX < right){
			if(RemasteredY < top && RemasteredY > bottom){
				//Targeting 이 On 되었는지 안되었는지 확인
				if(targetingIndicate == true){
					if(this->DTabTouchIndicate == false){
						LOGE("DTab Touch Indicate is false");
						this->DTabTouchIndicate = true;
					}else if(this->DTabTouchIndicate == true){
						LOGE("DTab Touch indicate is True");
						m_bEndPhase = true;
					}
				}else{
					//TODO Targeting On이 되지 않았을 경우 Comobo Reset 처리
				}
			}
		}
	}
}

void pbDTabNotes::onTimeAlerts() {
}

void pbDTabNotes::Update(float fTime) {
	//float x = this->positionX + pbGlobalInGameVariable::fWorldMoveX;
	float x = this->positionX;
	float y = this->positionY;

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
	this->noteWidth = 90.f;
	this->noteHeight = 90.f;
	this->helmetWidth = 90.f;
	this->helmetHeight = 60.f;
	this->DTabTouchIndicate = false;
}


