/*
 * pbStageValue.cpp
 *
 *  Created on: 2012. 12. 5.
 *      Author: NitroSoft
 */

#include "pbStageValue.h"

float pbStageValue::m_fTotalFeverGauge = 0;
bool pbStageValue::m_bChangeFeverGauge = 0;
float pbStageValue::m_fStageMoveX = 0;
float pbStageValue::m_fStageMoveSpeed = WORLD_MOVESPEED;
float pbStageValue::m_fMaxStageLength = 0;
bool pbStageValue::m_bChangedLife = false;
int pbStageValue::m_iNumLife = 0;
int pbStageValue::m_TotalScore = 0;
int pbStageValue::m_GettingScore = 0;

/////----------------------------------------------------------스코어------------------------------------------------------------////
int pbStageValue::CalcScoreData() {
	if( pbStageValue::m_GettingScore != 0) {
		pbStageValue::m_TotalScore += pbStageValue::m_GettingScore;
		pbStageValue::m_GettingScore  = 0;

		return pbStageValue::m_TotalScore ;
	}
	return -1;
}

/////----------------------------------------------------------라이프------------------------------------------------------------////
int pbStageValue::CalcLifeData() {
	if( pbStageValue::m_bChangedLife) {
		return pbStageValue::m_iNumLife ;
	}
	return -1;
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

