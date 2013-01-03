/*
 * pbDTabNotes.h
 *
 *  Created on: 2012. 11. 27.
 *      Author: Ophelia
 */

#ifndef PBDTABNOTES_H_
#define PBDTABNOTES_H_

#include "pbTargetingNotes.h"

class pbDTabNotes: public pbTargetingNotes {
public:
	pbDTabNotes();
	virtual ~pbDTabNotes();

	void PreSettingDraw();
	void DrawThis();
	void notify();
	void onTimeAlerts();
	void Update(float fTime);
	void setNotePosition(float x, float y);
	void ResetNoteState();

	NP_DEFINE_PROPERTY(unsigned int, noteTouchCount, TouchCounts);
	NP_DEFINE_PROPERTY(bool, DTabTouchIndicate, DTabIndicate);

protected:
	npTextureRect* HelmetObject;
	float helmetXposition;
	float helmetYposition;
	float helmetWidth;
	float helmetHeight;

};

#endif /* PBDTABNOTES_H_ */
