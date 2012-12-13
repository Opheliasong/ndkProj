/*
 * pbTabNotes.h
 *
 *  Created on: 2012. 11. 27.
 *      Author: Ophelia
 */

#ifndef PBTABNOTES_H_
#define PBTABNOTES_H_

#include "../stdafx.h"
#include "pbTargetingNotes.h"

class pbTabNotes: public pbTargetingNotes {
public:
	pbTabNotes();
	~pbTabNotes();

	void PreSettingDraw();
	void DrawThis();
	void notify();
	void onTimeAlerts();
	void Update(float fTime);
	void ResetNoteState();
	//void ReleaseNote();
};

#endif /* PBTABNOTES_H_ */
