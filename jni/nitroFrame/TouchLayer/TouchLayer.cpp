/*
 * TouchLayer.cpp
 *
 *  Created on: 2012. 11. 7.
 *      Author: Ophelia
 */

#include "TouchLayer.h"

TouchLayer& TouchLayer::GetInstance() {
	static TouchLayer Instance;
	return Instance;
}

void TouchLayer::onTouchPoint(int x, int y, int touchFlag, int touchPoint) {

	/*
	this->pointX = x-240;
	this->pointY = -(y-400);
	*/

	this->pointX = ((x / npRenderprocess::getInstance().getWidthRatio()));
	this->pointY = ((y / npRenderprocess::getInstance().getHeightRatio()));

	//[FIXLOG:11-24]: 1사분면 좌표로 변경
	this->pointY = ( npRenderprocess::getInstance().getRenderHeight() - this->pointY );

	/*
	this->pointX = this->pointX-(npRenderprocess::getInstance().getRenderWidth()/2);
	this->pointY = -(this->pointY-(npRenderprocess::getInstance().getRenderHeight()/2));
	*/
	/*
	LOGE("touch X: %d",this->pointX);
	LOGE("touch Y: %d",this->pointY);
	*/
	/*
	LOGE("Width Ratio : %f",npRenderprocess::getInstance().getWidthRatio());
	LOGE("Height Ratio : %f",npRenderprocess::getInstance().getHeightRatio());
	 */
	this->touchFlag = touchFlag;
	/*
	this->pointX = x;
	this->pointX = 480 / 480.f;
	*/
	npLinkNode<iTouchObserver*>* iterator;
	iterator = this->RegistedList.getNext();

	for(;iterator->getKernel() != this->RegistedList.getKernel(); iterator = iterator->getNext()){
		iterator->getKernel()->notify();
	}
}

void TouchLayer::onTouchDoubleTap(int x, int y) {
	//Double Tap 관련 로직을 작성한다.
}

TouchLayer::TouchLayer():pointX(0),pointY(0),activePointer(0),touchFlag(0) {
	this->RegistedList.setHeader();
}

TouchLayer::~TouchLayer() {
	//npLinkNode<iTouchObserver*>::remove(&this->RegistedList);
}

void TouchLayer::RegistedObserver(iTouchObserver* pointer) {
	npLinkNode<iTouchObserver*>::addTail(pointer,&this->RegistedList);
}

void TouchLayer::RemovedObserver(iTouchObserver* pointer) {
	npLinkNode<iTouchObserver*>::findRemove(pointer,&this->RegistedList);
}

void TouchLayer::PausedObserver(iTouchObserver* pointer, unsigned int pauseMS) {
}


void Java_nps_nitroframe_lib_npNativeEvent_npOnTouchEvent(
		JNIEnv* env, jclass thiz, jint x, jint y, jint touchFlag,
		jint pointerNumber) {
	TouchLayer::GetInstance().onTouchPoint(x,y,touchFlag,pointerNumber);
}

void Java_nps_nitroframe_lib_npNativeEvent_npDoubleTap(JNIEnv* env,jclass thiz, jint x, jint y) {
	TouchLayer::GetInstance().onTouchDoubleTap(x,y);
}
