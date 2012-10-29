/*
 * npTimer.h
 *
 *  Created on: 2012. 7. 14.
 *      Author: Ophelia
 */

#ifndef NPTIMER_H_
#define NPTIMER_H_
#include "DebugUtill.h"
#include "npSubject.h"

namespace nitroFrame{

class npTimer : public npSubject{
public:
	/*	타이머에게 Observer를 동록할때 한번만 Call Back을 요구할 경우 Once를
	 * 	타이머에게 OBserver를 등록할때 지정된 시간이 되면 계속해서 Call Back을 요구할 경우 REPEATLOOP를 설정하면 된다.
	 */
enum TIMERSTATUS {ONCE, REPEATLOOP,CHANGEDLOOP};
protected:

	/*Timer 내부에서 사용되는 객체.
	 * 내부 Collection에 저장하기 위한 용도 및 시간 Data를 보관하고, Update되는 시각과 등록된 시각의 차이를 통해
	 * 등록된 객체에게 notify 해줘야 할 시각을 찾아내는 용도로 사용한다.
	 */
	class npTimerIPC{
	public:
		enum NODETYPE{NONE=0,HEAD,INDATA};

		npTimerIPC();
		~npTimerIPC();

		npTimerIPC* prev;
		npTimerIPC* next;
		npObserver* observer;
		float m_AlertTime;			//Notify 될 시간을 말한다.
		float m_RegistedTime;		//등록된 시각을 말한다.
		float m_IntervalByRegistTime;	//등록된후 흘러간 시간을 말한다.
		NODETYPE CurrentType;
		TIMERSTATUS m_TimerStatus;

		void setHeader();		//Set the this node is Header
		static npObserver* findObserverByID(int ID, npTimerIPC* pHeader);

		//Adder
		static void add(npTimerIPC* newOne, npTimerIPC* prev, npTimerIPC* next);
		static void addTail(npObserver* pAdder,float fAlertTime, npTimerIPC* pHeader);
		static void addTail(npTimerIPC* pAdder, npTimerIPC* pHeader);

		//Remover
		static void remove(npTimerIPC* pRemover);
		static void findRemove(npTimerIPC* pRemover, npTimerIPC* pHeader);
	};

	//Timer Class의 실제 Data 및 함수군
	npTimerIPC* DataCellStore;

	float m_fCurrentTime;
	float m_fTotalTime;

	void checkAndNotifyByAlertTime(npTimerIPC* pIPC);
	//[FIXLOG:8-31] 추가 : 일시정지된 IPC 처리
	bool CheckPausedTimerIPC(npTimerIPC* pIPC, float fTime);
public:

	npTimer();
	~npTimer();

private:
	//기본 등록형
	void registerObserver(npObserver* apObserver);
	void removeObserver(npObserver* apObserver);
	void notifyAllObservers();
	static npTimer* SingleObject;

public:
	//Observer 객체를 시간과 타입과 함께 등록한다,
	/*
	 * registerObserver의 uiAlertSec의 단위는 ms.
	 * ex)fAlertSec = 1000 = 1초
	 *    fAlertSec = 100 = 0.1초
	 */
	static void registerObserver(npObserver* apObserver, TIMERSTATUS setTimer,float uiAlertSec);

	//static npObserver* removeObserver(npObserver* apObserver);

	//객체를 넣어두고 이 객체를 가지고 Remove 대상을 찾고 지운다.
	static void findRemove(npObserver* apObserver);

	static void updateTime(float fTime);

	static void initialize();
	static void release();
};


}
#endif /* NPTIMER_H_ */
