/*
 * npObserver.h
 *
 *  Created on: 2012. 7. 10.
 *      Author: NitroPigSoft02
 */

#ifndef NPOBSERVER_H_
#define NPOBSERVER_H_

#include "NitroFrame.h"

using namespace projectBean;


#define offsetOf(TYPE,MEMBER) ((size_t)&((TYPE*)0)->MEMBER)

/* 옵저버 객체의 Header
 * 이 객체는 interface 객체로 옵저버 패턴을 위해 사용된다.
 * 옵저버들은 notify를 통해서 subject들의 정보를 갱신받게 된다.
 */
class npObserver{
public:
	npObserver(){};
	virtual ~npObserver(){};
	virtual void notify()=0;
	virtual void notify(int x, int y, TOUCHSTATUS::TOUCHSTATUS Touchstatus)=0;
	virtual void notify(TOUCHSTATUS::TOUCHSTATUS Touchstatus)=0;
	virtual void wakeUpNotify()=0;
	virtual float resetWakeTime()=0;

	//[FIXLOG:8-31] 추가 : Notify 일시정지 변수. subject에서 보고 이용한다
	bool m_bNotifyPause;

};
#endif /* NPOBSERVER_H_ */
