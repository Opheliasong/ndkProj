
#include "pbStageTrigger.h"
#include <vector>
#include <algorithm>

/////----------------------------------------------------------스테이지 트리거---------------------------------------------------------------////
pbStageTrigger::pbStageTrigger() {
	m_bGamePause = false;
	m_Stateindex = 0;
}
pbStageTrigger::~pbStageTrigger() {
	m_PosStateVector.clear();
	m_IDStateVector.clear();
}

void pbStageTrigger::Initialize() {
	m_bGamePause = false;

	m_Stateindex = 0;
	m_PosStateVector.clear();
	m_PosStateVector.reserve(5);

	m_IDStateVector.clear();
	m_IDStateVector.reserve(5);

	pbStageValue::SetStageX(0);
	pbStageValue::SetStageMoveSpeed(WORLD_MOVESPEED);
}

void pbStageTrigger::AddPosState(float fStartStatePosX, void(Func)() ) {
	pbStagePosState tempState(fStartStatePosX, Func);
	m_PosStateVector.push_back(tempState);
	std::sort(m_PosStateVector.begin(), m_PosStateVector.end(), StagePosState_CompLess() );
}

void pbStageTrigger::AddIDState(int ID, void(Func)() ) {
	pbStageIDState tempState(ID, Func);
	m_IDStateVector.push_back(tempState);
	std::sort(m_IDStateVector.begin(), m_IDStateVector.end(), StageIDState_CompLess() );
}

void pbStageTrigger::ActivateIDState(int ID) {
//	StageIDState_FindLess comp();
	std::pair<IDStateVector::iterator, IDStateVector::iterator> PairResult;
	PairResult = std::equal_range(m_IDStateVector.begin(), m_IDStateVector.end(),  ID, StageIDState_CompLess() );

	if( PairResult.first != PairResult.second ) {
		(*(PairResult.first->m_fpFunc))();
		LOGE("DEBUG pbIDStageTrigger Activate");
	}
}

void pbStageTrigger::Update(float fTime) {
	if(!m_bGamePause)	{
		pbStageValue::PlusStageMoveX(pbStageValue::GetStageMoveSpeed()*fTime);
		if(m_Stateindex <  m_PosStateVector.size() ) {
			pbStagePosState* pState = &(m_PosStateVector[m_Stateindex]);

			if( pbStageValue::GetStageX() > pState->m_fStartStatePosX){
				(*pState->m_fpFunc)();
				m_Stateindex++;
				LOGE("DEBUG pbPosStageTrigger On");
			}

		}

	}
}
