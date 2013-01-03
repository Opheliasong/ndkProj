/*
 * pbTargetingNotes.cpp
 *
 *  Created on: 2012. 11. 27.
 *      Author: Ophelia
 */

#include "pbTargetingNotes.h"

pbTargetingNotes::pbTargetingNotes():m_fAlertTime(0.f),alertTime(0.f),targetingIndicate(false),TargetMarker(0) {
}

pbTargetingNotes::~pbTargetingNotes() {
	NP_SAFE_DELETE(TargetMarker);
}

bool pbTargetingNotes::setTargetTextureTAG(screenplayTag TAG) {
//	this->targetingSprite = npContainerDAO::GetInstance().getSpriteByTAG(TAG);
	this->TargetMarker->SetTextureTAG(TAG);

	if(NP_IS_EMPTY(this->TargetMarker)){
		return false;
	}
	return true;
}

void pbTargetingNotes::SetTimerService(float intervalTime) {
	nitroFrame::npTimer::getInstance().registerObserver(this, intervalTime);
}

void pbTargetingNotes::ChangeTimerInterval(float intervalTime) {
	nitroFrame::npTimer::getInstance().changeRepeatsTime(this,intervalTime);
}

void pbTargetingNotes::Update(float fTime) {
	//Targeting On/Off 상태 확인 및 처리
	if(targetingIndicate){
		TargetMarkerAngle += 360*fTime;

		m_fAnimationPercent += (1 / 0.5) * fTime;
		if (m_fAnimationPercent > 1.0f) {
			m_fAnimationPercent = 1.0f;
		}
		m_EffectColor.G = 1.0f -m_fAnimationPercent;
		m_EffectColor.B =m_EffectColor.G;
		//TargetMarkWidth = Lerp(3.0f, 1.0f,m_fAnimationPercent);
		//TargetMarkHeight = Lerp(3.0f, 1.0f,m_fAnimationPercent);
	}
	this->TargetMarker->SetRotationByZAxis(this->TargetMarkerAngle);
}

void pbTargetingNotes::setTargetMarkSize(float width, float height) {
	this->setTargetMarkWidth(width);
	this->setTargetMarkHegiht(height);
}

void pbTargetingNotes::ResetNoteState() {
	targetingIndicate = false;
	TargetMarkerAngle = 0.f;
	m_fAnimationPercent = 0.f;
}

void pbTargetingNotes::SetOnTargeting() {
	targetingIndicate = true;
}
