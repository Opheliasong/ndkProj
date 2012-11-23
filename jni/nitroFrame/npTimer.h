/*
 * npTimer.h
 *
 *  Created on: 2012. 7. 14.
 *      Author: Ophelia
 */

#ifndef NPTIMER_H_
#define NPTIMER_H_
#include "npDebugUtill.h"
#include "npSubject.h"

namespace nitroFrame{

class npTimer : public npSubject{
public:
	/*	Ÿ�̸ӿ��� Observer�� �����Ҷ� �ѹ� Call Back�� �䱸�� ��� Once��
	 * 	Ÿ�̸ӿ��� OBserver�� ����Ҷ� ������ �ð��� �Ǹ� ����ؼ� Call Back�� �䱸�� ��� REPEATLOOP�� �����ϸ� �ȴ�.
	 */
enum TIMERSTATUS {ONCE, REPEATLOOP,CHANGEDLOOP};
protected:

	/*Timer ���ο��� ���Ǵ� ��ü.
	 * ���� Collection�� �����ϱ� ���� �뵵 �� �ð� Data�� �����ϰ�, Update�Ǵ� �ð��� ��ϵ� �ð��� ���̸� ����
	 * ��ϵ� ��ü���� notify ����� �� �ð��� ã�Ƴ��� �뵵�� ����Ѵ�.
	 */
	class npTimerIPC{
	public:
		enum NODETYPE{NONE=0,HEAD,INDATA};

		npTimerIPC();
		~npTimerIPC();

		npTimerIPC* prev;
		npTimerIPC* next;
		npObserver* observer;
		float m_AlertTime;			//Notify �� �ð��� ���Ѵ�.
		float m_RegistedTime;		//��ϵ� �ð��� ���Ѵ�.
		float m_IntervalByRegistTime;	//��ϵ��� �귯�� �ð��� ���Ѵ�.
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

	//Timer Class�� ���� Data �� �Լ���
	npTimerIPC* DataCellStore;

	float m_fCurrentTime;
	float m_fTotalTime;

	void checkAndNotifyByAlertTime(npTimerIPC* pIPC);
	//[FIXLOG:8-31] �߰� : �Ͻ������� IPC ó��
	bool CheckPausedTimerIPC(npTimerIPC* pIPC, float fTime);
public:

	npTimer();
	~npTimer();

private:
	//�⺻ �����
	void registerObserver(npObserver* apObserver);
	void removeObserver(npObserver* apObserver);
	void notifyAllObservers();
	static npTimer* SingleObject;

public:
	//Observer ��ü�� �ð��� Ÿ�԰� �Բ� ����Ѵ�,
	/*
	 * registerObserver�� uiAlertSec�� ������ ms.
	 * ex)fAlertSec = 1000 = 1��
	 *    fAlertSec = 100 = 0.1��
	 */
	static void registerObserver(npObserver* apObserver, TIMERSTATUS setTimer,float uiAlertSec);

	//static npObserver* removeObserver(npObserver* apObserver);

	//��ü�� �־�ΰ� �� ��ü�� ������ Remove ����� ã�� �����.
	static void findRemove(npObserver* apObserver);

	static void updateTime(float fTime);

	static void initialize();
	static void release();
};


}
#endif /* NPTIMER_H_ */
