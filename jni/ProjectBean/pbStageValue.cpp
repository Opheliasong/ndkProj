/*
 * pbStageValue.cpp
 *
 *  Created on: 2012. 12. 5.
 *      Author: NitroSoft
 */

#include "pbStageValue.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////----------------------------------------------------		pbStageValue		------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////-----------스코어--------------////
int pbStageValue::m_TotalScore = 0;
int pbStageValue::m_GettingScore = 0;
/////-----------라이프--------------////
bool pbStageValue::m_bChangedLife = false;
int pbStageValue::m_iNumLife = 0;
/////-----------스테이지--------------////
float pbStageValue::m_fStageMoveX = 0;
float pbStageValue::m_fStageMoveSpeed = WORLD_MOVESPEED;
float pbStageValue::m_fMaxStageLength = 0;
/////-----------피버게이지--------------////
float pbStageValue::m_fTotalFeverGauge = 0;
bool pbStageValue::m_bChangeFeverGauge = 0;
////--------------아이템 가중치----------------//
pbStatusWeights pbStageValue::m_StatusWeight;
/////-----------네비게이트--------------////
int pbStageValue::m_StageLevel = 0;
int pbStageValue::m_ShopLevel = 0;
bool pbStageValue::m_ShopRoute = false;	// 상점가는 길
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

/////----------------------------------------------------------스테이지------------------------------------------------------------////


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

/////----------------------------------------------------------아이템 가중치 -----------------------------------------------------------////
void pbStageValue::CalcLifeWeight(int ItemCount) {
	pbStageValue::m_iNumLife += ItemCount;

	LOGE("pbStageValue:Life : %d", m_iNumLife);
}

void pbStageValue::CalcItemScoreWeight(int ItemCount) {
	m_StatusWeight.SetScoreWeight(120); //기본수치

	m_StatusWeight.PlusScoreWeight(ItemCount* ( 20) );

//	LOGE("pbStageValue:ScoreWeight : %d", m_StatusWeight.GetScoreWeight());
}

void pbStageValue::CalcItemSpeedWeight(int ItemCount) {
	m_StatusWeight.SetSpeedWeight(1.2f); //기본수치

	m_StatusWeight.PlusSpeedWeight(-ItemCount* ( 0.02f) );

//	LOGE("pbStageValue:SpeedWeight : %f", m_StatusWeight.GetSpeedWeight());
}

void pbStageValue::CalcItemFeverPointWeight(int ItemCount) {
	m_StatusWeight.SetFeverPointWeight(5.0f);		// 기본 수치

	m_StatusWeight.PlusFeverPointWeight(ItemCount * 5.0f);

//	LOGE("pbStageValue:FeverPointWeight : %f", m_StatusWeight.GetFeverPointWeight());
}

void pbStageValue::PrintWeightLog() {
	LOGE("pbStageValue:ScoreWeight : %d", m_StatusWeight.GetScoreWeight());
	LOGE("pbStageValue:SpeedWeight : %f", m_StatusWeight.GetSpeedWeight());
	LOGE("pbStageValue:FeverPointWeight : %f", m_StatusWeight.GetFeverPointWeight());
}

/////-----------네비게이트--------------////
void pbStageValue::IncreaeStageLevel() {
	m_StageLevel++;
	m_ShopRoute = false;
	if( m_StageLevel == 3) {
		m_ShopLevel = 1;
		m_ShopRoute = true;
	}
	else if( m_StageLevel == 6) {
		m_ShopLevel = 2;
		m_ShopRoute = true;
	}
	else if( m_StageLevel == 9) {
		m_ShopLevel = 3;
		m_ShopRoute = true;
	}
	else if( m_StageLevel == 11) {
		m_ShopLevel = 0;
		m_ShopRoute = true;
		m_StageLevel = 0;
	}

	pbStageValue::PrintLevelLog();
}

void pbStageValue::SetStageLevel(int Level) {
	m_StageLevel = Level;
}

void pbStageValue::IncreaeShopLevelForItemAction() {
	m_ShopRoute = false;
	m_ShopLevel++;
	if( m_ShopLevel == 1)
		m_StageLevel = 3;
	else if( m_ShopLevel == 2)
		m_StageLevel = 6;
	else if( m_ShopLevel == 3)
		m_StageLevel = 9;
	else if( m_ShopLevel == 4) {
		m_StageLevel = 0;
		m_ShopLevel = 0;
	}

	pbStageValue::PrintLevelLog();
}

void pbStageValue::SetShopLevel(int Level) {
	m_ShopRoute = false;
	m_ShopLevel = Level;

	if( m_ShopLevel == 1)
		m_StageLevel = 3;
	else if( m_ShopLevel == 2)
		m_StageLevel = 6;
	else if( m_ShopLevel == 3)
		m_StageLevel = 9;
	else if( m_ShopLevel == 4) {
		m_StageLevel = 0;
		m_ShopLevel = 0;
	}

	pbStageValue::PrintLevelLog();
}

void pbStageValue::PrintLevelLog() {
	LOGE("pbStageValue::STAGE LEVEL : %d", m_StageLevel);
	LOGE("pbStageValue::SHOP LEVEL : %d", m_ShopLevel);

	if( m_ShopRoute)
		LOGE("pbStageValue::NEXT ROUTE : SHOP");
	else
		LOGE("pbStageValue::NEXT ROUTE : STAGE");
}
