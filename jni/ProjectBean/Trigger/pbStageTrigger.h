/*
 * pbStageManager.h
 *
 *  Created on: 2012. 11. 27.
 *      Author: NitroSoft
 */

#ifndef PBSTAGETRIGGER_H_
#define PBSTAGETRIGGER_H_

#include "../stdafx.h"


class pbStagePosState {
	pbStagePosState();
public:
	pbStagePosState(float fStartStatePosX, void(Func)()) {m_fStartStatePosX = fStartStatePosX; m_fpFunc = Func; }
	~pbStagePosState() {};

	float m_fStartStatePosX;
	 void (*m_fpFunc)();
};

class StagePosState_CompLess : public std::binary_function<pbStagePosState, pbStagePosState, bool>{
public:
	bool operator()(const pbStagePosState& lhs, const pbStagePosState& rhs) const{
		return lhs.m_fStartStatePosX < rhs.m_fStartStatePosX;
	}
};

class pbStageIDState {
	pbStageIDState();
public:
	pbStageIDState(int ID, void(Func)()) {m_ID= ID; m_fpFunc = Func; }
	~pbStageIDState() {};

	inline bool operator<(const pbStageIDState& rhs) {
		return m_ID < rhs.m_ID;
	}

	int m_ID;
	 void (*m_fpFunc)();
};

class StageIDState_CompLess : public std::binary_function<pbStageIDState, pbStageIDState, bool>{
public:
	bool operator()(const pbStageIDState& lhs, const pbStageIDState& rhs) const{
		return lhs.m_ID < rhs.m_ID;
	}
	bool operator()(const pbStageIDState& lhs, const int& ID) const{
		return lhs.m_ID < ID;
	}
	bool operator()(const int& ID, const pbStageIDState& rhs) const{
		return ID < rhs.m_ID;
	}

};

/////----------------------------------------------------------pbStageTrigger------------------------------------------------------------////
class pbStageTrigger{
public:
	typedef enum { STATE_PLAY, STATE_BOSS_BATTLE, STATE_SCORE_RESULT, STATE_STAGE_CHANGE} StageState;
public:
	pbStageTrigger();
	virtual ~pbStageTrigger();

	void Initialize();

	void AddPosState(float fStartStatePosX, void(Func)() );
	void AddIDState(int ID, void(Func)() );
	void ActivateIDState(int ID);

	void Update(float fTime);

	inline bool IsPaused() { return m_bGamePause;}

private:
	typedef std::vector<pbStagePosState> PosStateVector;
	PosStateVector m_PosStateVector;

	typedef std::vector<pbStageIDState> IDStateVector;
	IDStateVector m_IDStateVector;

	bool m_bGamePause;
	int m_Stateindex;
};





#endif /* PBSTAGEMANAGER_H_ */