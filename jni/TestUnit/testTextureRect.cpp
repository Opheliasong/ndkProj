/*
 * testTextureRect.cpp
 *
 *  Created on: 2012. 11. 7.
 *      Author: Ophelia
 */

#include "testTextureRect.h"

testTextureRect::testTextureRect():touchOn(false){
	indicate = new npTextureRect;
	indicate->SetSize(50,50);
	indicate->setX(0);
	indicate->setY(-100);

}

testTextureRect::~testTextureRect() {
	delete indicate;
}

void testTextureRect::notify() {
	int x = TouchLayer::GetInstance().pointX;
	int y = TouchLayer::GetInstance().pointY;

	int Touchwidth = positionX + width;
	int TouchHeight = positionY + height;

	/*
	LOGE("touch X: %d",x);
	LOGE("touch Y: %d",y);

	LOGE("touchRect Left:%d",positionX);
	LOGE("touchRect Right:%d",Touchwidth);
	LOGE("touchRect Bottom:%d",positionY);
	LOGE("touchRect Top:%d",TouchHeight);
	 */
	if(x >= positionX && x <= Touchwidth){
		if(y >= positionY && y <= TouchHeight){
			if(TouchLayer::GetInstance().touchFlag == 0){
				touchOn = true;
			}else if(TouchLayer::GetInstance().touchFlag == 1){
				touchOn = false;
			}
		}
	}

}

void testTextureRect::DrawThis() {
	npTextureRect::DrawThis();
	if(touchOn){
		npRenderprocess::getInstance().DoDraw(*this->indicate);
	}
}

void testTextureRect::PreSettingDraw() {
	npTextureRect::PreSettingDraw();
}
