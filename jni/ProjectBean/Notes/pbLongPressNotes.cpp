/*
 * pbLongPressNotes.cpp
 *
 *  Created on: 2012. 11. 27.
 *      Author: Ophelia
 */

#include "pbLongPressNotes.h"

pbLongPressNotes::pbLongPressNotes() {
	noteType = LONGPRESS;
}

pbLongPressNotes::~pbLongPressNotes() {
	// TODO Auto-generated destructor stub
}

void pbLongPressNotes::PreSettingDraw() {
}

void pbLongPressNotes::DrawThis() {
}

void pbLongPressNotes::notify() {
}

void pbLongPressNotes::onTimeAlerts() {
}

void pbLongPressNotes::Update(float fTime) {
	float x = positionX + pbGlobalInGameVariable::fWorldMoveX;
	float y = positionY;

	pbTargetingNotes::Update(fTime);
}
