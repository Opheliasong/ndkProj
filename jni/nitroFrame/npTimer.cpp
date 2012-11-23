/*
 * npTimer.cpp
 *
 *  Created on: 2012. 7. 20.
 *      Author: Ophelia
 */

#include "npTimer.h"
#include "pbNote.h"

namespace nitroFrame{

npTimer* npTimer::SingleObject;

/*==============
 * IPC Area
 * ============== */

npTimer::npTimerIPC::npTimerIPC():prev(NULL),next(NULL),observer(NULL),CurrentType(NONE){
}

npTimer::npTimerIPC::~npTimerIPC(){

}

void npTimer::npTimerIPC::setHeader(){
	this->next = this;
	this->prev = this;
	this->CurrentType = npTimerIPC::HEAD;
}
npObserver* npTimer::npTimerIPC::findObserverByID(int ID, npTimerIPC* pHeader){

}


void npTimer::npTimerIPC::add(npTimerIPC* newOne, npTimerIPC* prev, npTimerIPC* next){

	next->prev = newOne;
	newOne->next = next;
	newOne->prev = prev;
	prev->next = newOne;

}

/*Observer������ time�� �̿��ϴ� add�Լ�
 *
 * @pAdder: �߰��� Observer
 * @fAlertTime: �˷��� Time
 * @pHeader: ����Ʈ�� Header
 */
void npTimer::npTimerIPC::addTail(npObserver* pAdder,float fAlertTime, npTimerIPC* pHeader){
	npTimerIPC* newOne = new npTimerIPC;
	newOne->m_AlertTime = fAlertTime;
	newOne->observer = pAdder;

	add(newOne , pHeader->next , pHeader);
}

/*IPC��ü�� ������ ����ϴ� Adder�Լ�
 *
 * @pAdder: �߰��� IPC ��ü
 * @pHeader: ����Ʈ�� Header
 */
void npTimer::npTimerIPC::addTail(npTimerIPC* pAdder, npTimerIPC* pHeader){
	//add(pAdder,pHeader->next,pHeader->prev);
	add(pAdder , pHeader->prev, pHeader);
}

/*IPC��ü�� LInked �Ӽ��� ���� ������ �Լ�
 * �� �Լ��� ���� Remover�� ����Ʈ���� �и��Ǿ����� �ȴ�.
 *
 *@pRemover: ��ũ �Ӽ��� ���� �ܵ����� ���� IPC
 */
void npTimer::npTimerIPC::remove(npTimerIPC* pRemover){
	if(pRemover->next != NULL && pRemover->prev != NULL){
		LOGE("remove the prev & next");
		pRemover->next->prev = pRemover->prev;
		pRemover->prev->next = pRemover->next;
	}
	pRemover->prev = NULL;
	pRemover->next = NULL;
}

/*IPC��ü ��ü�� ã�Ƽ� Remove�ϴ� �Լ�
 *
 *@pRemover: Remove�� ��� IPC
 *@pHeader: ����Ʈ�� ���
 */
void npTimer::npTimerIPC::findRemove(npTimerIPC* pRemover, npTimerIPC* pHeader){
	npTimerIPC* iterator;

	for(iterator =  pHeader->next; iterator != pHeader; iterator = iterator->next){
		if(iterator == pRemover){
			remove(iterator);
		}
	}
}

/*==============
 * Timer Area
 * ============== */

npTimer::npTimer():DataCellStore(NULL),m_fCurrentTime(0),m_fTotalTime(0){

}

npTimer::~npTimer(){

}

void npTimer::notifyAllObservers(){

}

void npTimer::registerObserver(npObserver* apObserver){

}

/*Ÿ�̸ӿ��� Observer��ü�� ����Ѵ�.
 * Time�� ��ȭ�� �˷��� ����(ONCE(��ȸ��), REPEATLOOP(������ �ݺ�))�� �ð�(Ȥ�� �ֱ�)�� ���� ����Ͽ�
 * ��ϵ� �ֱ⸸ŭ ó�� �ϰ� �ȴ�.
 *
 *@npObserver: ��ϵ� Observer��ü
 *@setTimer: Ÿ�̸��� ���¸� ���Ѵ�. (��ȸ��, ������ �ݺ� ���� ����)
 *@fAlerSec: ms�� ������ �ϴ� �˸� �ð��� ���Ѵ�.
 */
void npTimer::registerObserver(npObserver* apObserver, TIMERSTATUS setTimer,float fAlertSec){
	npTimerIPC* registedOne = new npTimerIPC;
	registedOne->CurrentType = npTimerIPC::INDATA;
	registedOne->observer = apObserver;
	registedOne->m_RegistedTime = SingleObject->m_fCurrentTime;
	registedOne->m_AlertTime = fAlertSec;
	registedOne->m_TimerStatus = setTimer;
	npTimerIPC::addTail(registedOne,SingleObject->DataCellStore);
}

/* �ش� observer �� �� ���� �� ����� ���踦 ���� ������.
 * �ش� ����Ʈ���� �������� ������ �ȴ�.
 */
void npTimer::removeObserver(npObserver* apObserver){
}

/* Observer�� ���� IPC�� ã�Ƽ� IPC List���� �ش� IPC�� ���� ��Ų��.
 *
 */
void npTimer::findRemove(npObserver* apObserver){

	npTimerIPC* iterator;

	for(iterator = SingleObject->DataCellStore->next; iterator != SingleObject->DataCellStore; iterator = iterator->next){
		if(iterator->observer == apObserver){
			npTimerIPC::remove(iterator);
			delete iterator;
			return;
		}
	}

}

/*Ÿ�̸ӿ��� �ð��� ������ �˷��ִ� �Լ�
 *���� ���μ������� Ÿ�̸��� ������ ���� �� ȣ��Ǿ�� �ϸ�, �� ȣ��� ���� ���� ��ϵ� Observer���� Notify�� ����ǰ� �ȴ�.
 */
void npTimer::updateTime(float fTime){
	//���� �ð��� �����Ѵ�.
	SingleObject->m_fCurrentTime += fTime;

	//Collection ���� ��ϵ� ��ϵ� �ð��� ������ �ð��� ���̰� AlertTime�� ���� Node �� ã�´�.
	npTimerIPC* iterator;
	for(iterator = SingleObject->DataCellStore->next; iterator != SingleObject->DataCellStore; iterator = iterator->next){
		iterator->m_IntervalByRegistTime += fTime;
		if( !SingleObject->CheckPausedTimerIPC(iterator, fTime) ) //[FIXLOG:8-31] �߰� : �Ͻ����� �� IPC�� ��Ƽ���� üũ���� �ʴ´�
			SingleObject->checkAndNotifyByAlertTime(iterator);
	}
}

/* ������ �ð��� IPC�� ��ϵ� �ð��� ���� ������ notify �Լ��� ȣ�⿩�θ� �Ǵ��ϴ� �Լ�.
 * ���⼭ IPC�� ������ �ִ� Alert Status�� �ľ��Ͽ� �ٽ� ��ȣ���� ���� �غ��� ������,
 *  ��ȸ���̹Ƿ� IPC�� ������ ������ �ľ��ϰ� ó�� �ϰ� �ȴ�.
 *  Ÿ�̸��� ���� �߿��� ����� �ϴ� ����Լ�
 *
 *  @pIPC : Ȯ���� IPC��ü
 */
void npTimer::checkAndNotifyByAlertTime(npTimerIPC* pIPC){
	//IPC�� ��ϵ� �ð��� ������ �ð��� ���̸� ���Ѵ�.
	//IPC�� ��ϵ� �ð� - ��ϵ��� ���� �ð�
	//float DelayTime = pIPC->m_RegistedTime - pIPC->m_IntervalByRegistTime;
	float DelayTime = SingleObject->m_fCurrentTime - pIPC->m_RegistedTime;

/*	LOGfloatString("Gap TIme : ",DelayTime);
	LOGfloatString("AlertTime : ",pIPC->m_AlertTime);*/

	//������ �ð��� IPC�� ���� AlertTime���� ���ٸ� Observer ���� notify�Ѵ�.
	if(DelayTime >= pIPC->m_AlertTime){
		pIPC->observer->wakeUpNotify();

		switch(pIPC->m_TimerStatus){
			case ONCE:{
				//�ش� IPC�� Timer Checking���� ��Ʈ����.
				findRemove(pIPC->observer);
				break;
			}
			case CHANGEDLOOP:{
				pIPC->m_AlertTime = pIPC->observer->resetWakeTime();
				pIPC->m_RegistedTime = SingleObject->m_fCurrentTime;
				LOGfloatString("[[npTimer::checkAndNotify]]AlertTime:",pIPC->m_AlertTime);
				break;
			}
			case REPEATLOOP:{
				// Repeatloop�� ��� �ٸ� ������ ���� �ʴ´�.
				break;
			}
		}
	}
}

//[FIXLOG:8-31] �߰� : �Ͻ������� IPC ó��
bool npTimer::CheckPausedTimerIPC(npTimerIPC* pIPC, float fTime)
{
	//��ϵȽð��� ���� ���̸� �ð��� �����ʴ°Ͱ� ���� ȿ���� �����
	//������Ÿ�� ���ϱ� ���� ������
	if( pIPC->observer->m_bNotifyPause)
		pIPC->m_RegistedTime += fTime;

	return pIPC->observer->m_bNotifyPause;
}

/* Ÿ�̸Ӹ� �ʱ�ȭ �ϴ� �Լ�.
 * Ÿ�̸Ӵ� ���� ��ü���� ������ �����Ƿ� �̸� �����ϰ� Ÿ�̸Ӹ� ����Ҽ� �ְ� �ϱ� ���� ���Ǵ� �Լ���.
 * Ÿ�̸Ӹ� ����ϱ������� �׻� ȣ���Ͽ��� �Ѵ�.
 */
void npTimer::initialize(){
	if(SingleObject == NULL){
		SingleObject = new npTimer;
		SingleObject->DataCellStore = new npTimerIPC;
		SingleObject->DataCellStore->setHeader();
	}
}

/*Ÿ�̸��� ����� ��� ������, ���� ���μ����� ����Ǵ� ������ ȣ��Ǿ�� �� �Լ�.
 * Ÿ�̸� ���ο� ������ ��ü���� �����ϰ� �޸� Deallocate �۾��� �����Ѵ�.
 */
void npTimer::release(){
	//DataStore(Observer Collection�� ��� Deallocate�Ѵ�)
	npTimerIPC* iterator;

	//destroyListHead�� node���� ���鼭 ��ü���� ������ ����� �۾��� �Ѵ�.
	for(iterator = SingleObject->DataCellStore->next; iterator != SingleObject->DataCellStore; ){
		npTimerIPC* nextTarget = iterator->next;

		delete iterator;

		iterator = nextTarget;
	}
	delete SingleObject->DataCellStore;
	//SingleObject��ü�� �����Ͽ� Timer�� ����� �׸��д�.
	delete SingleObject;

	LOGI("npTimer Release");
}
}
