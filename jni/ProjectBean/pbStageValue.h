/*
 * pbStageValue.h
 *
 *  Created on: 2012. 12. 5.
 *      Author: NitroSoft
 */

#ifndef PBSTAGEVALUE_H_
#define PBSTAGEVALUE_H_

#include "stdafx.h"


class pbStageValue {
public:
	pbStageValue();
	~pbStageValue();

	/////-----------스코어--------------////
	inline static void IncreaseScore(int Score) {m_GettingScore += Score; }
	inline static int GetScoreTotal() { return m_TotalScore;}
	static int CalcScoreData();

	//--스피드 감소 아이템 능력치--//
	inline static int GetScoreWeight() { return m_ScoreWeight; }
	static void SetScoreWeight(int ItemCount);

	/////-----------라이프--------------////
	static int CalcLifeData();
	inline static int GetLifeTotal() { return m_iNumLife; }

	//--라이프 아이템 능력치--//
	static void SetLifeWeight(int ItemCount);

	/////-----------스테이지--------------////
	inline static float GetStageX() {	return m_fStageMoveX;	}
	inline static float GetStageMoveSpeed() { return m_fStageMoveSpeed; }
	inline static float GetStageMaxLength() { return m_fMaxStageLength;	}

	inline static void SetStageX(float X) { m_fStageMoveX = X;	}
	inline static void PlusStageMoveX(float X) { m_fStageMoveX += X;	}
	inline static void SetStageMaxLength(float Length) { m_fMaxStageLength = Length; }
	inline static void SetStageMoveSpeed(float Speed) { m_fStageMoveSpeed = Speed;	}

	//--스피드 감소 아이템 능력치--//
	inline static float GetSpeedWeight() { return m_fSpeedWeight; }
	static void SetSpeedWeight(int ItemCount);

	/////-----------피버게이지--------------////
	inline static void FeverGaugeReset() {m_fTotalFeverGauge = 0;	m_bChangeFeverGauge = true;	}
	inline static bool IsFeverGaugeMaximum() {return m_fTotalFeverGauge == (float)MAX_FEVERGAUGE;	}

	static void IncreaseFeverGauge(float Point);
	static float GetFeverGauge();

	//--피버 포인트 아이템 능력치--//
	inline static float GetFeverPoint() { return m_fFeverPointWeight; }
	static void SetFeverPoint(int ItemCount);

	enum {MAX_FEVERGAUGE = 100 };
private:
	/////-----------스코어--------------////
	static int m_TotalScore;
	static int m_GettingScore;

	static int m_ScoreWeight;	 // 점수 가중치
	/////-----------라이프--------------////
	static bool m_bChangedLife;
	static int m_iNumLife;

	/////-----------스테이지--------------////
	static float m_fStageMoveX;
	static float m_fStageMoveSpeed;
	static float m_fMaxStageLength;

	static float m_fSpeedWeight;	 // 속도 가중치

	/////-----------피버게이지--------------////
	static float m_fTotalFeverGauge;
	static bool m_bChangeFeverGauge;

	static float m_fFeverPointWeight;	//피버포인트 가중치
};


#endif /* PBSTAGEVALUE_H_ */
