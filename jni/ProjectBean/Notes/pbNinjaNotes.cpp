/*
 * pbNinjaNotes.cpp
 *
 *  Created on: 2012. 11. 27.
 *      Author: Ophelia
 */

#include "pbNinjaNotes.h"

pbNinjaNotes::pbNinjaNotes() {
	noteType = NINJA;

	this->noteWidth = 110.f;
	this->noteHeight = 110.f;

	this->avoidPoints = 3;
	this->DoAvoid = false;

	this->BodyActor = new npTextureRect("ghost");
	this->BodyActor->SetPosition(&this->positionX, &this->positionY);

	this->TargetMarker = new npTextureRect("target");
	this->TargetMarker->SetPosition(&this->positionX, &this->positionY);

	this->DeadActor  = new npTextureRect("run");
	this->DeadActor->SetPosition(&this->positionX, &this->positionY);
}

pbNinjaNotes::~pbNinjaNotes() {
	// TODO Auto-generated destructor stub
}

void pbNinjaNotes::PreSettingDraw() {
	this->BodyActor->SetSize(this->noteWidth, this->noteHeight);
	this->TargetMarker->SetSize(this->TargetMarkWidth * m_fScale, this->TargetMarkHeight * m_fScale);
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

	}else{
		npRenderprocess::getInstance().DoDraw(*this->DeadActor);
		if(this->DeadActor->IsFinish()){
			ReleaseNote();
			return;
		}
	}
}

void pbNinjaNotes::notify() {
	if(TouchLayer::GetInstance().touchFlag == 0){
		if(IsHitThis()){
			//Targeting 이 On 되었는지 안되었는지 확인
			if(targetingIndicate == true){
				this->DoAvoid = true;
				this->avoidPoints--;
//				LOGE("Ninja Note Avoid Points:%d",this->avoidPoints);
				//					LOGE("NinJa Note State : %d",this->m_bEndPhase);
				//					LOGE("Ninja Note Body Actor Position) X:%f, Y:%f ",*this->BodyActor->getX(), *this->BodyActor->getY());
				//					LOGE("Ninja Note Body Actor size) w:%f, h:%f ",this->BodyActor->getWidthSize(), this->BodyActor->getHeightSize());
				pbComboManager::GetInstance()->IncreaseCombo(25);
				pbStageValue::IncreaseFeverGauge(2);
			}else{
				//TODO Targeting On이 되지 않았을 경우 Comobo Reset 처리
				m_bEndPhase = true;;
				pbComboManager::GetInstance()->ResetCombo();
			}
		}
	}
}

void pbNinjaNotes::onTimeAlerts() {
}

void pbNinjaNotes::Update(float fTime) {
	//Position Update
	float x = positionX - (pbStageValue::GetStageMoveSpeed()*fTime);
	float y = positionY;

	pbTargetingNotes::Update(fTime);

	if(this->avoidPoints <= 0){
		m_bEndPhase = true;
		noteState = DEAD;
		this->DoAvoid = false;
	}

	//만약 Avoid 상태일 경우 새로운 위치를 구하고, 그 위치로 이동한다.
	if(this->DoAvoid){
		//1) 새로운 위치를 구한다.
		//새로운 위치는 X 축의 값은 증가해야 하며, Y 축의 값은 화면내의 범위라면 제한이 없다.
		srand(time(NULL));
		float increaseX = (rand() % 15)*20;
		float changedYPosition = (rand()% 10)*30 + 100;
		//this->setNotePosition(x + increaseX, changedYPosition);
		x += increaseX;
		y = changedYPosition;

		//Avoid 상태 해지
		this->DoAvoid = false;
	}
	setNotePosition(x,y);
}

void pbNinjaNotes::ResetNoteState() {
	pbNoteElement::ResetNoteState();
	pbTargetingNotes::ResetNoteState();
	this->noteWidth = 110.f;
	this->noteHeight = 110.f;
	this->avoidPoints = 3;
	this->DoAvoid = false;
	if(NP_IS_NOT_EMPTY(BodyActor)){
		BodyActor->SetPosition(&this->positionX, &this->positionY);
	}
	if(NP_IS_NOT_EMPTY(Effector)){
		BodyActor->SetPosition(&this->positionX, &this->positionY);
	}
	if(NP_IS_NOT_EMPTY(DeadActor)){
		BodyActor->SetPosition(&this->positionX, &this->positionY);
	}
}

