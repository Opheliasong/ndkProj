
#include "pbStageTrigger.h"
#include <vector>
#include <algorithm>

float pbStageValue::m_fTotalFeverGauge = 0;
bool pbStageValue::m_bChangeFeverGauge = 0;
float pbStageValue::m_fStageMoveX = 0;
float pbStageValue::m_fStageMoveSpeed = WORLD_MOVESPEED;
float pbStageValue::m_fMaxStageLength = 0;
bool pbStageValue::m_bChangedLife = false;
int pbStageValue::m_iNumLife = 0;
int pbStageValue::m_TotalScore = 0;
int pbStageValue::m_GettingScore = 0;

int pbStageValue::GetScoreData() {
	if( pbStageValue::m_GettingScore != 0) {
		pbStageValue::m_TotalScore += pbStageValue::m_GettingScore;
		pbStageValue::m_GettingScore  = 0;

		return pbStageValue::m_TotalScore ;
	}
	return -1;
}

int pbStageValue::GetLifeData() {
	if( pbStageValue::m_bChangedLife) {
		return pbStageValue::m_iNumLife ;
	}
	return -1;
}

void pbStageValue::ResetFeverGauge() {
	pbStageValue::m_fTotalFeverGauge = 0;
	pbStageValue::m_bChangeFeverGauge = true;
}

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

bool pbStageValue::IsMaximumGauge() {
	return pbStageValue::m_fTotalFeverGauge == (float)pbStageValue::MAX_FEVERGAUGE;
}

float pbStageValue::GetStageX() {
	return pbStageValue::m_fStageMoveX;
}


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

	pbStageValue::m_fStageMoveX = 0;
	pbStageValue::m_fStageMoveSpeed = WORLD_MOVESPEED;
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
		pbStageValue::m_fStageMoveX += pbStageValue::m_fStageMoveSpeed*fTime;
		if(m_Stateindex <  m_PosStateVector.size() ) {
			pbStagePosState* pState = &(m_PosStateVector[m_Stateindex]);

			if( pbStageValue::m_fStageMoveX > pState->m_fStartStatePosX){
				(*pState->m_fpFunc)();
				m_Stateindex++;
				LOGE("DEBUG pbPosStageTrigger On");
			}

		}

	}
}
