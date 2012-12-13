/*
 * pbTargetingNotes.h
 *
 *  Created on: 2012. 11. 27.
 *      Author: Ophelia
 */

#ifndef PBTARGETINGNOTES_H_
#define PBTARGETINGNOTES_H_

#include "../stdafx.h"
#include "../../nitroFrame/npNitroFrameMacro.h"
#include "../../nitroFrame/Timer/npTimer.h"
#include "pbNoteElement.h"

class pbTargetingNotes: public pbNoteElement {
public:
	pbTargetingNotes();
	virtual ~pbTargetingNotes();

	virtual void PreSettingDraw()=0;
	virtual void DrawThis()=0;
	virtual void notify()=0;
	virtual void onTimeAlerts()=0;
	virtual void Update(float fTime);
	virtual void ResetNoteState();

	NP_DEFINE_PROPERTY(float, alertTime, AlertTime);
	NP_DEFINE_PROPERTY(bool, targetingIndicate, TargetIndicate);
	NP_DEFINE_PROPERTY(float, TargetMarkWidth, TargetMarkWidth);
	NP_DEFINE_PROPERTY(float, TargetMarkHeight, TargetMarkHegiht);

	void setTargetMarkSize(float width, float height);
	bool setTargetTextureTAG(screenplayTag TAG);
	void SetTimerService(float intervalTime);  //Timer에 등록하는 함수
	void ChangeTimerInterval(float intervalTime);  //Timer에 Alert Interval 변경하여 등록하는 함수
	void SetOnTargeting();

protected:
	float m_fAlertTime;

//	sprite* targetingSprite;
	float TargetMarkerAngle;
	COLOR_RGBA m_EffectColor;
	float m_fAnimationPercent;

	npTextureRect* TargetMarker;
};

#endif /* PBTARGETINGNOTES_H_ */
