/*
 * testTextureRect.h
 *
 *  Created on: 2012. 11. 7.
 *      Author: Ophelia
 */

#ifndef TESTTEXTURERECT_H_
#define TESTTEXTURERECT_H_

//#include "npTextureRect.h"
#include "../nitroFrame/NitroFrame.h"

class testTextureRect: public npTextureRect,public iTouchObserver{
public:
	testTextureRect();
	virtual ~testTextureRect();

	void notify();
	void DrawThis();
	void PreSettingDraw();

	bool touchOn;
	npTextureRect* indicate;
};

#endif /* TESTTEXTURERECT_H_ */
