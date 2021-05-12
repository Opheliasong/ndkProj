/*
 * pbTimeTrigger.cpp
 *
 *  Created on: 2012. 12. 5.
 *      Author: NitroSoft
 */

#include "pbTimeTrigger.h"

////--------------------------------------------------------pbRandomTimeTrigger----------------------------------------------------------//
pbRandomTimeTrigger::pbRandomTimeTrigger() {
	m_bTriggerOn = false;
	m_bTakeTimeFinish = false;
	m_iMaximumTime = 0;
	m_iMinimumTime = 0;
	m_fTakeTime = 0.0f;
}
pbRandomTimeTrigger::~pbRandomTimeTrigger() {

}

void pbRandomTimeTrigger::SetTriggerCondition(float fMaximumTime,	float fMinimumTime) {
	srand(time(NULL));

	int iMaximumTime = (int)(fMaximumTime*1000.f);
	int iMinimumTime = (int)(fMinimumTime*1000.f);
	m_iMaximumTime = iMaximumTime - iMinimumTime;
	m_iMinimumTime = iMinimumTime;

}

void  pbRandomTimeTrigger::TriggerStart() {
	int ms = rand()%m_iMaximumTime + m_iMinimumTime;
	m_fTakeTime = (float)ms/1000.f;
	m_bTakeTimeFinish = false;
	m_bTriggerOn = true;

	LOGfloatString("pbRandomTimeTrigger:TakeTime", m_fTakeTime);
}

void  pbRandomTimeTrigger::TriggerEnd() {
	m_bTriggerOn = false;
}

void pbRandomTimeTrigger::Update(float fTime) {
	if( m_bTriggerOn) {
		m_fTakeTime -= fTime;

		if( m_fTakeTime <= 0.0f) {
			m_bTakeTimeFinish = true;
			m_bTriggerOn = false;
		}
	}
}

