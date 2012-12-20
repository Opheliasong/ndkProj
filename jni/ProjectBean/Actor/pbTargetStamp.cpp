/*
 * pbTargetStamp.cpp
 *
 *  Created on: 2012. 12. 4.
 *      Author: NitroSoft
 */

#include "pbTargetStamp.h"

////-------------------------------------------------------------pbTargetStamp------------------------------------------------------------------//
pbTargetStamp::pbTargetStamp() {
	m_bOnDraw = false;
	m_bTargetOn = false;
	m_bStop = false;
	m_iPhase = TARGET_NONE;

	m_fRotate = 0.0f;
	m_fRotateDir = 0.0f;

	m_fScale = 1.0f;
	m_fAlpha = 0.0f;

	m_fLimitTime  = 0.0f;
	m_fAniTime = 0.0f;

	m_fLimitFadeSpeed = -LIMIT_ALPHA_DIFF_VALUE;

	m_pDrawUnit = new pbBasicDrawUnit();
	m_pTrigger = new pbRandomTimeTrigger();

}
pbTargetStamp::~pbTargetStamp() {
	delete m_pDrawUnit;
	delete m_pTrigger;
}

void pbTargetStamp::SetTag(screenplayTag Tag, float fWidth, float fHeight ) {
	m_pDrawUnit->SetTextureTAG(Tag);
	m_pDrawUnit->SetSize(fWidth, fHeight);
}

void pbTargetStamp::SetTarget() {
	m_bOnDraw = true;
	m_bTargetOn = true;
	m_iPhase = TARGET_START;
	m_fScale = 3.0f;
	m_fAlpha = 0.0f;
}

void pbTargetStamp::ReleaseTarget() {
	m_bTargetOn = false;
	m_iPhase = TARGET_TOUCH_END;
}

void pbTargetStamp::SetRotateValue(float fRotateSpeed) {
	m_fRotateDir = fRotateSpeed;
}

void pbTargetStamp::SetLimitTime(float LimitTime) {
	m_fLimitTime = LimitTime;
}

void pbTargetStamp::StampingStart() {
	m_bStop = false;
	m_fAniTime = 0.0f;
	m_iPhase = TARGET_NONE;
	GetTrigger()->TriggerStart();
}
void pbTargetStamp::StampingStop() {
	m_bStop = true;
	m_bOnDraw = false;
	m_bTargetOn = false;
	m_iPhase = TARGET_NONE;
}

void pbTargetStamp::Draw() {
	if( m_bOnDraw ) {
		glPushMatrix();
		glColor4f(1.0f, 1.0f, 1.0f, m_fAlpha);
		m_pDrawUnit->PreSettingDraw();
		glRotatef(m_fRotate, 0.0f, 0.0f, 1.0f);
		glScalef(m_fScale, m_fScale, 1.0f);
		m_pDrawUnit->DrawThis();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glPopMatrix();
	}
}

void pbTargetStamp::Update(float fTime) {
	if( !m_bStop) {
		m_fRotate += 360.0f*(1/m_fRotateDir)*fTime;

		if( m_iPhase == TARGET_NONE) {
			m_pTrigger->Update(fTime);
			if( m_pTrigger->IsCompleteCondition() ) {
				SetTarget();
			}
		}
		else if( m_iPhase == TARGET_START ) {
			Animaition_Start(fTime);
		}
		else if( m_iPhase == TARGET_ING ) {
			Animaition_Targeting(fTime);
		}
		else if( m_iPhase == TARGET_TOUCH_END ) {
			Animaition_TouchEnd(fTime);
		}
		else if( m_iPhase == TARGET_LIMIT_END ) {
			Animaition_LimitEnd(fTime);
		}
	}
}

////--------------------------------------------------------------------애니메이션 루프 함수-----------------------------------------------------------------------//
void pbTargetStamp::Animaition_Start(float fTime) {
	m_fAniTime += fTime;

	m_fScale = npLerp(m_fScale, 1.0f, m_fAniTime);

	m_fAlpha = npLerp(m_fAlpha, 1.0f, m_fAniTime);

	if( m_fAniTime >= 1.0f) {
		m_fAniTime = 0.0f;
		m_fAlpha = 1.0f;
		m_fScale = 1.0f;

		m_iPhase = TARGET_ING;
	}
}

void pbTargetStamp::Animaition_Targeting(float fTime) {
	m_fAniTime += fTime;

	if( m_fAniTime > m_fLimitTime*0.4f) {
		m_fAlpha += m_fLimitFadeSpeed*fTime;

		if( m_fAlpha <= 0.1f)
			m_fLimitFadeSpeed = LIMIT_ALPHA_DIFF_VALUE;
		else if( m_fAlpha >= 1.0f)
			m_fLimitFadeSpeed = -LIMIT_ALPHA_DIFF_VALUE;
	}

	if( m_fAniTime > m_fLimitTime) {
		m_fAlpha = 1.0f;
		m_fAniTime = 0.0f;
		m_iPhase = TARGET_LIMIT_END;
		m_bTargetOn = false;
	}
}

void pbTargetStamp::Animaition_TouchEnd(float fTime){
/*	m_fAniTime += 2*fTime;

	if( m_fAniTime >= 1.0f) {
		m_fAniTime = 0.0f;
	}*/
	m_pTrigger->TriggerStart();
	m_fAniTime = 0.0f;
	m_iPhase = TARGET_NONE;
	m_bOnDraw  = false;
}

void pbTargetStamp::Animaition_LimitEnd(float fTime){
	m_fAniTime += fTime/2;

	m_fScale = npLerp(m_fScale, 0.0f, m_fAniTime);

	m_fAlpha = npLerp(m_fAlpha, 0.0f, m_fAniTime);

	if( m_fAniTime >= 1.0f) {
		m_fAniTime = 0.0f;
		m_fAlpha = 0.0f;
		m_fScale = 3.0f;

		m_bOnDraw  = false;

		m_pTrigger->TriggerStart();
		m_iPhase = TARGET_NONE;
	}
}

