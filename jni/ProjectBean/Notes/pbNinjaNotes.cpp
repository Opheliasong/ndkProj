/*
 * pbNinjaNotes.cpp
 *
 *  Created on: 2012. 11. 27.
 *      Author: Ophelia
 */

#include "pbNinjaNotes.h"

pbNinjaNotes::pbNinjaNotes() {
	noteType = NINJA;

	this->avoidPoints = 3;
	this->DoAvoid = false;

	this->BodyActor = new npTextureRect("ci");
	this->BodyActor->SetPosition(&this->positionX, &this->positionY);

	this->TargetMarker = new npTextureRect("ci");
	this->TargetMarker->SetPosition(&this->positionX, &this->positionY);

	this->DeadActor  = new npTextureRect("run");
	this->DeadActor->SetPosition(&this->positionX, &this->positionY);
}

pbNinjaNotes::~pbNinjaNotes() {
	// TODO Auto-generated destructor stub
}

void pbNinjaNotes::PreSettingDraw() {
	this->BodyActor->SetSize(this->noteWidth, this->noteHeight);
	this->TargetMarker->SetSize(this->TargetMarkWidth, this->TargetMarkHeight);
	this->DeadActor->SetSize(this->noteWidth, this->noteHeight);
}

void pbNinjaNotes::DrawThis() {
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

void pbNinjaNotes::notify() {
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

	if(TouchLayer::GetInstance().touchFlag == 0){
		if(TouchPointX > left && TouchPointX < right){
			if(RemasteredY < top && RemasteredY > bottom){
				//Targeting 이 On 되었는지 안되었는지 확인
				if(targetingIndicate == true){
					//TODO Targeting On이 되었을 경우 Avoid Points를 감소하고, 새로운 위치를 재 설정
					this->DoAvoid = true;
					this->avoidPoints--;
				}else{
					//TODO Targeting On이 되지 않았을 경우 Comobo Reset 처리
				}
			}
		}
	}
}

void pbNinjaNotes::onTimeAlerts() {
}

void pbNinjaNotes::Update(float fTime) {
	//Position Update
	float x = positionX + pbGlobalInGameVariable::fWorldMoveX;
	float y = positionY;

	pbTargetingNotes::Update(fTime);

	if(this->avoidPoints == 0){
		m_bEndPhase = true;
		this->DoAvoid = false;
	}

	//만약 Avoid 상태일 경우 새로운 위치를 구하고, 그 위치로 이동한다.
	if(this->DoAvoid){
		//1) 새로운 위치를 구한다.
		//새로운 위치는 X 축의 값은 증가해야 하며, Y 축의 값은 화면내의 범위라면 제한이 없다.
		srand(time(NULL));
		float increaseX = (rand() % 15)*20;
		float changedYPosition = (rand()% 10)*30 + 100;

		this->setNotePosition(x + increaseX, changedYPosition);

		//Avoid 상태 해지
		this->DoAvoid = false;
	}
}

void pbNinjaNotes::ResetNoteState() {
	pbNoteElement::ResetNoteState();
	pbTargetingNotes::ResetNoteState();
	this->noteWidth = 90.f;
	this->noteHeight = 90.f;
	this->avoidPoints = 3;
	this->DoAvoid = false;
}

