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
};

#endif /* PBLONGPRESSNOTES_H_ */
