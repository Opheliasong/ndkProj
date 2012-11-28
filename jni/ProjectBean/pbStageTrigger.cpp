
#include "pbStageTrigger.h"
#include <vector>
#include <algorithm>

float pbStageValue::m_fGettingFeverGauge = 0;
float pbStageValue::m_fStageMoveX = 0;
float pbStageValue::m_fStageMoveSpeed = WORLD_MOVESPEED;
int pbStageValue::m_iNumLife = 0;
int pbStageValue::m_TotalScore = 0;
int pbStageValue::m_GettingScore = 0;


pbStageTrigger::pbStageTrigger() {
	m_bGamePause = false;
	m_Stateindex = 0;
}
pbStageTrigger::~pbStageTrigger() {
	m_StageStateVector.clear();
}

void pbStageTrigger::Initialize() {
	m_bGamePause = false;

	m_Stateindex = 0;
	m_StageStateVector.clear();
	m_StageStateVector.reserve(5);

	pbStageValue::m_fStageMoveX = 0;
	pbStageValue::m_fStageMoveSpeed = WORLD_MOVESPEED;
}

void pbStageTrigger::AddState(float fStartStatePosX, void(Func)() ) {
	pbStageState tempState(fStartStatePosX, Func);
	m_StageStateVector.push_back(tempState);
	std::sort(m_StageStateVector.begin(), m_StageStateVector.end(), StageState_CompLess() );
}

void pbStageTrigger::Update(float fTime) {
	if(!m_bGamePause)	{
		pbStageValue::m_fStageMoveX += pbStageValue::m_fStageMoveSpeed*fTime;

		if(m_Stateindex <  m_StageStateVector.size() ) {
			pbStageState* pState = &(m_StageStateVector[m_Stateindex]);
			if( pbStageValue::m_fStageMoveX > pState->m_fStartStatePosX){
				(*pState->m_fpFunc)();
				m_Stateindex++;
				LOGE("DEBUG pbStageTrriger On");
			}
		}

	}
}
