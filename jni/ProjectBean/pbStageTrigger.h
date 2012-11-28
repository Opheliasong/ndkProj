/*
 * pbStageManager.h
 *
 *  Created on: 2012. 11. 27.
 *      Author: NitroSoft
 */

#ifndef PBSTAGETRIGGER_H_
#define PBSTAGETRIGGER_H_

#include "stdafx.h"


class pbStageState {
	pbStageState();
public:
	pbStageState(float fStartStatePosX, void(Func)()) {m_fStartStatePosX = fStartStatePosX; m_fpFunc = Func; }
	~pbStageState() {};

	float m_fStartStatePosX;
	 void (*m_fpFunc)();
};

class StageState_CompLess : public std::binary_function<pbStageState, pbStageState, bool>{
public:
	bool operator()(const pbStageState& lhs, const pbStageState& rhs) const{
		return lhs.m_fStartStatePosX < rhs.m_fStartStatePosX;
	}
};


class pbStageValue {
public:
	pbStageValue();
	virtual ~pbStageValue();

	static float m_fGettingFeverGauge;
	static float m_fStageMoveX;
	static float m_fStageMoveSpeed;
	static int m_iNumLife;
	static int m_TotalScore;
	static int m_GettingScore;
};
/*
 * 스테이지 매니저의 정체
 * 보스의 등장, 스코어 계산, 다음스테이지로 이동
 * 각 스테이트의 시기를 체크, 작동 하게 함
 */
class pbStageTrigger{
public:
	typedef enum { STATE_PLAY, STATE_BOSS_BATTLE, STATE_SCORE_RESULT, STATE_STAGE_CHANGE} StageState;
public:
	pbStageTrigger();
	virtual ~pbStageTrigger();

	void Initialize();

	void AddState(float fStartStatePosX, void(Func)() );

	void Update(float fTime);

	inline bool IsPaused() { return m_bGamePause;}


private:
	typedef std::vector<pbStageState> StageStateVector;
	StageStateVector m_StageStateVector;

	bool m_bGamePause;
	int m_Stateindex;
};





#endif /* PBSTAGEMANAGER_H_ */
