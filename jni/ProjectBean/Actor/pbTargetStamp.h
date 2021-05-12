/*
 * pbTargetStamp.h
 *
 *  Created on: 2012. 12. 4.
 *      Author: NitroSoft
 */

#ifndef PBTARGETSTAMP_H_
#define PBTARGETSTAMP_H_

#include "../stdafx.h"

////----------------------------------------pbTargetStamp--------------------------------------------//
class pbRandomTimeTrigger;
class pbTargetStamp {
public:
	pbTargetStamp();
	~pbTargetStamp();

	///---------변수 설정 함수-----------------//
	void SetTag(screenplayTag Tag, float fWidth, float fHeight);
	void SetRotateValue(float fRotateSpeed);
	void SetLimitTime(float LimitTime);

	////--------타겟상태 조절--------------//
	void SetTarget();
	void ReleaseTarget();

	void StampingStart();
	void StampingStop();

	void Draw();
	void Update(float fTime);

	////-----------애니메이션 루프 함수--------------//
	void Animaition_Start(float fTime);
	void Animaition_Targeting(float fTime);
	void Animaition_TouchEnd(float fTime);
	void Animaition_LimitEnd(float fTime);

	bool IsTargetOn() { return m_bTargetOn; }

	////--------인스턴스--------------//
	pbRandomTimeTrigger* GetTrigger() {return  m_pTrigger;}

	enum { TARGET_NONE = 0, TARGET_START, TARGET_ING, TARGET_TOUCH_END, TARGET_LIMIT_END};
	enum { LIMIT_ALPHA_DIFF_VALUE = 3 };
private:
	pbBasicDrawUnit* m_pDrawUnit;
	bool m_bOnDraw;
	bool m_bTargetOn;
	bool m_bStop;
	int m_iPhase;

	float m_fAniTime;

	float m_fRotateDir;
	float m_fRotate;

	float m_fScale;
	float m_fAlpha;

	float m_fLimitTime;
	float m_fLimitFadeSpeed;


	pbRandomTimeTrigger* m_pTrigger;
};




#endif /* PBTARGETSTAMP_H_ */
