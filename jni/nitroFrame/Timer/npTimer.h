/*
 * npTimer.h
 *
 *  Created on: 2012. 7. 14.
 *      Author: Ophelia
 */

#ifndef NPTIMER_H_
#define NPTIMER_H_

#include <ctime>
#include "../npLinkNode.h"
#include "../npDebugUtill.h"
#include "../npNitroFrameMacro.h"
#include "npTimerObserver.h"

namespace nitroFrame{

class npTimer{
public:
enum TIMERSTATUS {ONCE, REPEATLOOP,CHANGEDLOOP,PAUSED};
protected:
	class npTimerIPC{
	public:
		npTimerIPC();
		~npTimerIPC();

		npTimerObserver* observer;
		float m_AlertTime;				//Notify 될 시간을 말한다.
		float m_RegistedTime;			//Timer에 등록이 된 당시의 시각을 말한다.
		float m_IntervalByRegistTime;	//Registetd Time에서의 Replay Interval Timer을 말한다.
		TIMERSTATUS m_TimerStatus;

		npTimerIPC& operator=(npTimerIPC& rightSide);
	};

	typedef npLinkNode<npTimerIPC> npTimerLinkNode;

	//Timer Class의 Registed Container
	//npTimerIPC* DataCellStore;
	npTimerLinkNode* DataCellStore;
	npTimerLinkNode* DeleteQueue;

	float m_fCurrentTime;
	float m_fTotalTime;

	bool checkAndNotifyByAlertTime(npTimerLinkNode* TimerNode);
	//bool CheckPausedTimerIPC(npTimerIPC* pIPC, float fTime);

protected: npTimer();
public:
static npTimer& getInstance();
	~npTimer();

public:
	//Observer
	/*
	 * registerObserver에서의 인자값 fAlertSec의 단위는ms.
	 * ex)fAlertSec = 1000 = 1sec
	 *    fAlertSec = 100 = 0.1sec
	 */
	//observer를 Once형태로 등록한다.
	void registerObserver(npTimerObserver* apObserver, float fAlertSec);

	//Observer를 RepeatLoop형태로 등록한다
	void registerObserver(npTimerObserver* apObserver, float fAlertSec, float RepeatInterval);

	//Observer를 Status에 맞추어 등록한다. 이때 Interval Time의 경우 fAlertSec와 동일
	void registerObserver(npTimerObserver* apObserver, TIMERSTATUS Status, float fAlertSec);

	//Observer를 명시된 Status에 맞추어 등록한다. 이때 Interval 타임은 RepeatInveal과 동일
	void registerObserver(npTimerObserver* apObserver, TIMERSTATUS Status, float fAlertSec, float RepeatInterval);

	void findRemove(npTimerObserver* apObserver);
	void changeRepeatsTime(npTimerObserver* apObserver, float ChangedTime);

	void updateTime(float fTime);

	//static void initialize();
	//static void release();
	/*
		class npTimerIPC{
		public:
			enum NODETYPE{NONE=0,HEAD,INDATA};

			npTimerIPC();
			~npTimerIPC();

			npTimerIPC* prev;
			npTimerIPC* next;
			npObserver* observer;
			float m_AlertTime;				//Notify 될 시간을 말한다
			float m_RegistedTime;			//Timer에 등록이 된 당시의 시각을 말한다.
			float m_IntervalByRegistTime;	//
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
	*/
};


}
#endif /* NPTIMER_H_ */
