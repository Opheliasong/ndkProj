/*
 * pbStageValue.h
 *
 *  Created on: 2012. 12. 5.
 *      Author: NitroSoft
 */

#ifndef PBSTAGEVALUE_H_
#define PBSTAGEVALUE_H_

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////----------------------------------------------------		pbStageValue		------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 스테이지에서 사용하는 종속된 변수의 묶음
/// 아이템 가중치의 사용 : 플레이씬 로딩때마다 가중치 초기화 후, 아이템 개수를 세고 기체 능력치 적용을 반복한다

class pbStageValue {
public:
	pbStageValue();
	~pbStageValue();

	/////-----------스코어--------------////
	inline static void IncreaseScore(int Score) {m_GettingScore += Score; }
	inline static int GetScoreTotal() { return m_TotalScore;}
	static int CalcScoreData();

	/////-----------라이프--------------////
	static int CalcLifeData();
	inline static int GetLifeTotal() { return m_iNumLife; }

	/////-----------스테이지--------------////
	inline static float GetStageX() {	return m_fStageMoveX;	}
	inline static float GetStageMoveSpeed() { return m_fStageMoveSpeed; }
	inline static float GetStageMaxLength() { return m_fMaxStageLength;	}

	inline static void SetStageX(float X) { m_fStageMoveX = X;	}
	inline static void PlusStageMoveX(float X) { m_fStageMoveX += X;	}
	inline static void SetStageMaxLength(float Length) { m_fMaxStageLength = Length; }
	inline static void SetStageMoveSpeed(float Speed) { m_fStageMoveSpeed = Speed;	}

	/////-----------피버게이지--------------////
	inline static void FeverGaugeReset() {m_fTotalFeverGauge = 0;	m_bChangeFeverGauge = true;	}
	inline static bool IsFeverGaugeMaximum() {return m_fTotalFeverGauge == (float)MAX_FEVERGAUGE;	}

	static void IncreaseFeverGauge(float Point);
	static float GetFeverGauge();


	////--------------아이템 가중치----------------//
	static void CalcLifeWeight(int ItemCount);
	static void CalcItemScoreWeight(int ItemCount);
	static void CalcItemSpeedWeight(int ItemCount);
	static void CalcItemFeverPointWeight(int ItemCount);
	static pbStatusWeights& GetStatusWeight() { return m_StatusWeight; }

	static void PrintWeightLog();

	/////-----------네비게이트--------------////
	static bool IsNextShopRoute() { return m_ShopRoute;}
	static int GetStageLevel() { return m_StageLevel; }
	static void SetStageLevel(int Level);
	static void IncreaeStageLevel();

	static int GetShopLevel() { return m_ShopLevel; }
	static void SetShopLevel(int Level);
	static void IncreaeShopLevelForItemAction();

	static void ResetShopRoute() { m_ShopRoute = false; }
	static void PrintLevelLog();

	enum {MAX_FEVERGAUGE = 100 };
private:
	/////-----------스코어--------------////
	static int m_TotalScore;
	static int m_GettingScore;

	/////-----------라이프--------------////
	static bool m_bChangedLife;
	static int m_iNumLife;

	/////-----------스테이지--------------////
	static float m_fStageMoveX;
	static float m_fStageMoveSpeed;
	static float m_fMaxStageLength;


	/////-----------피버게이지--------------////
	static float m_fTotalFeverGauge;
	static bool m_bChangeFeverGauge;

	////--------------아이템 가중치----------------//
	static pbStatusWeights m_StatusWeight;

	/////-----------네비게이트--------------////
	static int m_StageLevel;
	static int m_ShopLevel;
	static bool m_ShopRoute;	// 상점가는 길

};


#endif /* PBSTAGEVALUE_H_ */
