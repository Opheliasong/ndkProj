/*
 * TouchLayer.h
 *
 *  Created on: 2012. 11. 7.
 *      Author: Ophelia
 */

#ifndef TOUCHLAYER_H_
#define TOUCHLAYER_H_

#include <jni.h>
#include "../Render/npRenderProcess.h"
#include "../npLinkNode.h"
#include "TouchObserver.h"

#ifdef __cplusplus
extern "C" {
#endif
//Touch Native JNI 함수
JNIEXPORT void JNICALL Java_nps_nitroframe_lib_npNativeEvent_npOnTouchEvent(JNIEnv* env, jclass thiz, jint x, jint y, jint touchFlag, jint pointerNumber);
JNIEXPORT void JNICALL Java_nps_nitroframe_lib_npNativeEvent_npDoubleTap(JNIEnv *env, jclass thiz, jint x, jint y);
}

class TouchLayer {
	/***
	 * @warning 멀티쓰레드에 매우 안정적이지 못하다.\
	 * input 자체는 main Thread에서 들어오게 되고,\
	 * GLView는 게임의 update를 관장한다.이로 인해 문제가 발생할 가능성이 있다.
	 */
public:
	virtual ~TouchLayer();
	npLinkNode<iTouchObserver*> RegistedList;

	int pointX;
	int pointY;
	int activePointer;
	int touchFlag;

	void RegistedObserver(iTouchObserver* pointer);
	void RemovedObserver(iTouchObserver* pointer);
	void PausedObserver(iTouchObserver* pointer,unsigned int pauseMS);

	//Touch 관련 interface
	virtual void onTouchPoint(int x, int y, int touchFlag, int touchPoint);
	virtual void onTouchDoubleTap(int x, int y);

	static TouchLayer& GetInstance();

private:TouchLayer();
};

 /* namespace nitroFrame */

#endif /* TOUCHLAYER_H_ */
