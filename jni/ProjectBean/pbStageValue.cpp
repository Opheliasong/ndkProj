/*
 * pbStageValue.cpp
 *
 *  Created on: 2012. 12. 5.
 *      Author: NitroSoft
 */

#include "pbStageValue.h"

float pbStageValue::m_fTotalFeverGauge = 0;
bool pbStageValue::m_bChangeFeverGauge = 0;
float pbStageValue::m_fFeverPointWeight = 0;
float pbStageValue::m_fStageMoveX = 0;
float pbStageValue::m_fStageMoveSpeed = WORLD_MOVESPEED;
float pbStageValue::m_fSpeedWeight = 1.0f;
float pbStageValue::m_fMaxStageLength = 0;
bool pbStageValue::m_bChangedLife = false;
int pbStageValue::m_iNumLife = 0;
int pbStageValue::m_TotalScore = 0;
int pbStageValue::m_GettingScore = 0;
int pbStageValue::m_ScoreWeight = 0;

/////----------------------------------------------------------스코어------------------------------------------------------------////
int pbStageValue::CalcScoreData() {
	if( pbStageValue::m_GettingScore != 0) {
		pbStageValue::m_TotalScore += pbStageValue::m_GettingScore;
		pbStageValue::m_GettingScore  = 0;

		return pbStageValue::m_TotalScore ;
	}
	return -1;
}

void pbStageValue::SetScoreWeight(int ItemCount) {
	m_ScoreWeight = 120; //기본수치

	m_ScoreWeight += ItemCount* ( 20);

	LOGE("pbStageValue:ScoreWeight : %d", m_ScoreWeight);
}

/////----------------------------------------------------------라이프------------------------------------------------------------////
int pbStageValue::CalcLifeData() {
	if( pbStageValue::m_bChangedLife) {
		return pbStageValue::m_iNumLife ;
	}
	return -1;
}

void pbStageValue::SetLifeWeight(int ItemCount) {
	pbStageValue::m_iNumLife += ItemCount;

	LOGE("pbStageValue:Life : %d", m_iNumLife);
}

/////----------------------------------------------------------스테이지------------------------------------------------------------////
void pbStageValue::SetSpeedWeight(int ItemCount) {
	m_fSpeedWeight = 1.2f; //기본수치

	m_fSpeedWeight -= ItemCount* ( 0.02f);

	LOGE("pbStageValue:SpeedWeight : %f", m_fSpeedWeight);
}

/////----------------------------------------------------------피버게이지------------------------------------------------------------////
void pbStageValue::IncreaseFeverGauge(float Point) {
	pbStageValue::m_fTotalFeverGauge += Point;

	if(pbStageValue::m_fTotalFeverGauge < 0.0f ) {
		pbStageValue::m_fTotalFeverGauge = 0.0f;
	}
	else if(pbStageValue::m_fTotalFeverGauge > (float)MAX_FEVERGAUGE) {
		pbStageValue::m_fTotalFeverGauge = (float)MAX_FEVERGAUGE;
	}

	pbStageValue::m_bChangeFeverGauge = true;
}

float pbStageValue::GetFeverGauge() {
	if( pbStageValue::m_bChangeFeverGauge) {
		pbStageValue::m_bChangeFeverGauge = false;
		return pbStageValue::m_fTotalFeverGauge;
	}
	return -1.0f;
}

void pbStageValue::SetFeverPoint(int ItemCount) {
	m_fFeverPointWeight = 5.0f;		// 기본 수치

	m_fFeverPointWeight += ItemCount * 5.0f;

	LOGE("pbStageValue:FeverPointWeight : %f", m_fFeverPointWeight);
}

