/*
 * pbStatusWeight.h
 *
 *  Created on: 2012. 12. 13.
 *      Author: NitroSoft
 */

#ifndef PBSTATUSWEIGHT_H_
#define PBSTATUSWEIGHT_H_

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////----------------------------------------------------		pbStatusWeights		------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////

class pbStatusWeights {
public:
	pbStatusWeights() {	InitValue(0, 0.0f, 0.0f);	}
	~pbStatusWeights() {	}

	void InitValue(int ScoreWeight, float fSpeedWeight, float fFeverPointWeight) {
		m_iScoreWeight  = ScoreWeight;
		m_fSpeedWeight = fSpeedWeight;
		m_fFeverPointWeight = fFeverPointWeight;
	}

	void PlusStatusWeight(pbStatusWeights& Weights) {
		PlusScoreWeight(Weights.m_iScoreWeight);
		PlusSpeedWeight(Weights.m_fSpeedWeight);
		PlusFeverPointWeight(Weights.m_fFeverPointWeight);
	}

	//--스코어 아이템 능력치--//
	int GetScoreWeight() { return m_iScoreWeight; }
	void SetScoreWeight(int Weight) { m_iScoreWeight = Weight; }
	void PlusScoreWeight(int Weight) { m_iScoreWeight += Weight; }

	//--스피드 감소 아이템 능력치--//
	float GetSpeedWeight() { return m_fSpeedWeight; }
	void SetSpeedWeight(float Weight) { m_fSpeedWeight = Weight; }
	void PlusSpeedWeight(float Weight) { m_fSpeedWeight += Weight; }

	//--피버 포인트 아이템 능력치--//
	float GetFeverPointWeight() { return m_fFeverPointWeight; }
	void SetFeverPointWeight(float Weight) { m_fFeverPointWeight = Weight; }
	void PlusFeverPointWeight(float Weight) { m_fFeverPointWeight += Weight; }

private:
	int m_iScoreWeight;	 // 점수 가중치
	float m_fSpeedWeight;	 // 속도 가중치
	float m_fFeverPointWeight;	//피버포인트 가중치
};

#endif /* PBSTATUSWEIGHT_H_ */
