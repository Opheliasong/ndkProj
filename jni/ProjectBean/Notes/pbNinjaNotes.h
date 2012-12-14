/*
 * pbNinjaNotes.h
 *
 *  Created on: 2012. 11. 27.
 *      Author: Ophelia
 */

#ifndef PBNINJANOTES_H_
#define PBNINJANOTES_H_

#include "pbTargetingNotes.h"

class pbNinjaNotes: public pbTargetingNotes {
public:
	pbNinjaNotes();
	virtual ~pbNinjaNotes();

	void PreSettingDraw();
	void DrawThis();
	void notify();
	void onTimeAlerts();
	void Update(float fTime);
	void ResetNoteState();

	NP_DEFINE_PROPERTY(int, avoidPoints, AvoidPoint);
protected:
	bool DoAvoid;

};

#endif /* PBNINJANOTES_H_ */
