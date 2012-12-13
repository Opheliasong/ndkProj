/*
 * pbTabNotes.cpp
 *
 *  Created on: 2012. 11. 27.
 *      Author: Ophelia
 */

#include "pbTabNotes.h"

pbTabNotes::pbTabNotes() {
	noteType = TABNOTE;

	this->BodyActor = new npTextureRect("ci");
	this->BodyActor->SetPosition(&this->positionX, &this->positionY);

	this->TargetMarker = new npTextureRect("ci");
	this->TargetMarker->SetPosition(&this->positionX, &this->positionY);

	this->DeadActor = new npTextureRect("run");
	this->DeadActor->SetPosition(&this->positionX, &this->positionY);
}

pbTabNotes::~pbTabNotes() {
}

/***
 * @fn TabNote의 Draw이전에 하여야 될 작업들을 작성한다.
 */
void pbTabNotes::PreSettingDraw() {
	this->BodyActor->SetSize(this->noteWidth, this->noteHeight);
	this->TargetMarker->SetSize(this->TargetMarkWidth,this->TargetMarkHeight);
	this->DeadActor->SetSize(this->noteWidth, this->noteHeight);
}

/***
 * @fn TabNote의 실제 Darw Logic
 */
void pbTabNotes::DrawThis() {
	if(!m_bEndPhase){ // 1) 기본적인 Rendering 처리
		//1-1) Body Sprite Rendering
		npRenderprocess::getInstance().DoDraw(*this->BodyActor);

		//1-2)Effect Sprite Rendering
		if(onEffectMode){
			npRenderprocess::getInstance().DoDraw(*this->Effector);
		}

		//1-3)Targeting이 되었을 경우 Targeting Sprite Rendering
		if(targetingIndicate){
			npRenderprocess::getInstance().DoDraw(*this->TargetMarker);
		}

	}else{ // 2) Hit 되었을 경우 Effect 처리
		npRenderprocess::getInstance().DoDraw(*this->DeadActor);

		if(this->DeadActor->IsFinish()){
			ReleaseNote();
			return;
		}
	}
}

/***
 * @fn Touch Event에 의해 notify되어져 처리되어질 Logic
 * @details 점수 스코어가 증가하고, Combo 처리가 진행, Dead Phase로 진행
 */
void pbTabNotes::notify() {
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

	if(TouchPointX > left && TouchPointX < right){
		if(RemasteredY < top && RemasteredY > bottom){
			//Targeting 이 On 되었는지 안되었는지 확인
			if(targetingIndicate == true){
				//TODO Targeting On이 되었을 경우 점수 및 Combo 처리
			}else{
				//TODO Targeting On이 되지 않았을 경우 Comobo Reset 처리
			}

			//Rendering Sprite 전
			m_bEndPhase = true;
		}
	}
}

/***
 * @fn Timer에 의해 event Signal을 받을 경우 처리하게 될 Logic
 */
void pbTabNotes::onTimeAlerts() {

}

void pbTabNotes::Update(float fTime) {
	//Position Update
	//float x = positionX + pbGlobalInGameVariable::fWorldMoveX;
	float x = positionX;
	float y = positionY;

	pbTargetingNotes::Update(fTime);
}

void pbTabNotes::ResetNoteState() {
	pbNoteElement::ResetNoteState();
	pbTargetingNotes::ResetNoteState();
	this->noteWidth = 90.f;
	this->noteHeight = 90.f;
}

//void pbTabNotes::ReleaseNote() {
//	pbNoteElement::ReleaseNote();
//}
