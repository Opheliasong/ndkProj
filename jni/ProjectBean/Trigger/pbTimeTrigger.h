/*
 * pbTimeTrigger.h
 *
 *  Created on: 2012. 12. 5.
 *      Author: NitroSoft
 */

#ifndef PBTIMETRIGGER_H_
#define PBTIMETRIGGER_H_

#include "../stdafx.h"


////----------------------------------------pbRandomTimeTrigger--------------------------------------------//
class pbRandomTimeTrigger {
public:
	pbRandomTimeTrigger();
	~pbRandomTimeTrigger();

	void SetTriggerCondition(float fMaximumTime, float fMinimumTime);
	void TriggerStart();
	void TriggerEnd();

	void Update(float fTime);

	bool IsCompleteCondition() { return m_bTakeTimeFinish; }

private:
	bool m_bTriggerOn;
	bool m_bTakeTimeFinish;

	//rand 계산의 편의를 위해 1000단위 int형으로 시간을 저장한다
	int m_iMaximumTime;
	int m_iMinimumTime;
	float m_fTakeTime;
};

#endif /* PBTIMETRIGGER_H_ */
