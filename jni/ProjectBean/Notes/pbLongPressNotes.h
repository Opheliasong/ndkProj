/*
 * pbLongPressNotes.h
 *
 *  Created on: 2012. 11. 27.
 *      Author: Ophelia
 */

#ifndef PBLONGPRESSNOTES_H_
#define PBLONGPRESSNOTES_H_

#include "pbTargetingNotes.h"

class pbLongPressNotes: public pbTargetingNotes {
public:
	pbLongPressNotes();
	virtual ~pbLongPressNotes();

	void PreSettingDraw();
	void DrawThis();
	void notify();
	void onTimeAlerts();
	void Update(float fTime);
	void ResetNoteState();

	NP_DEFINE_PROPERTY(float, pressTime, PressTime);	//Touch Event가 발생 하였을 경우 증가하게 되는 PressTime 속성
	NP_DEFINE_PROPERTY(bool, pressOn, PressOn);
	NP_DEFINE_PROPERTY(float, RequirePressTouchTime, RequirePressTouchTime);
};

#endif /* PBLONGPRESSNOTES_H_ */
