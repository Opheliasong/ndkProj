/*
 * pbNinjaNotes.cpp
 *
 *  Created on: 2012. 11. 27.
 *      Author: Ophelia
 */

#include "pbNinjaNotes.h"

pbNinjaNotes::pbNinjaNotes() {
	noteType = NINJA;
}

pbNinjaNotes::~pbNinjaNotes() {
	// TODO Auto-generated destructor stub
}

void pbNinjaNotes::PreSettingDraw() {
}

void pbNinjaNotes::DrawThis() {
}

void pbNinjaNotes::notify() {
}

void pbNinjaNotes::onTimeAlerts() {
}

void pbNinjaNotes::Update(float fTime) {
	//Position Update
	float x = positionX + pbGlobalInGameVariable::fWorldMoveX;
	float y = positionY;

	pbTargetingNotes::Update(fTime);
}
