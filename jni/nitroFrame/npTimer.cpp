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

/*Observer원형과 time을 이용하는 add함수
 *
 * @pAdder: 추가될 Observer
 * @fAlertTime: 알려질 Time
 * @pHeader: 리스트의 Header
 */
void npTimer::npTimerIPC::addTail(npObserver* pAdder,float fAlertTime, npTimerIPC* pHeader){
	npTimerIPC* newOne = new npTimerIPC;
	newOne->m_AlertTime = fAlertTime;
	newOne->observer = pAdder;

	add(newOne , pHeader->next , pHeader);
}

/*IPC객체를 가지고 사용하는 Adder함수
 *
 * @pAdder: 추가될 IPC 객체
 * @pHeader: 리스트의 Header
 */
void npTimer::npTimerIPC::addTail(npTimerIPC* pAdder, npTimerIPC* pHeader){
	//add(pAdder,pHeader->next,pHeader->prev);
	add(pAdder , pHeader->prev, pHeader);
}

/*IPC객체의 LInked 속성을 끊어 버리는 함수
 * 이 함수에 들어온 Remover는 리스트에서 분리되어지게 된다.
 *
 *@pRemover: 링크 속성을 없애 단독으로 만들 IPC
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

/*IPC객체 자체를 찾아서 Remove하는 함수
 *
 *@pRemover: Remove될 대상 IPC
 *@pHeader: 리스트의 헤더
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

/*타이머에게 Observer객체를 등록한다.
 * Time의 변화를 알려줄 상태(ONCE(일회성), REPEATLOOP(지속적 반복))과 시간(혹은 주기)를 같이 등록하여
 * 등록된 주기만큼 처리 하게 된다.
 *
 *@npObserver: 등록될 Observer객체
 *@setTimer: 타이머의 상태를 말한다. (일회성, 지속적 반복 등의 형태)
 *@fAlerSec: ms를 단위로 하는 알림 시간을 말한다.
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

/* 해당 observer 의 앞 노드와 뒷 노드의 관계를 끊어 버린다.
 * 해당 리스트에서 옵저버는 빠지게 된다.
 */
void npTimer::removeObserver(npObserver* apObserver){
}

/* Observer를 가진 IPC를 찾아서 IPC List에서 해당 IPC를 해지 시킨다.
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

/*타이머에게 시간의 갱신을 알려주는 함수
 *메인 프로세스에서 타이머의 갱신을 위해 꼭 호출되어야 하며, 이 호출된 값에 따라 등록된 Observer들의 Notify가 실행되게 된다.
 */
void npTimer::updateTime(float fTime){
	//현재 시각을 갱신한다.
	SingleObject->m_fCurrentTime += fTime;

	//Collection 에서 등록된 등록된 시각과 현재의 시각의 차이가 AlertTime과 같은 Node 를 찾는다.
	npTimerIPC* iterator;
	for(iterator = SingleObject->DataCellStore->next; iterator != SingleObject->DataCellStore; iterator = iterator->next){
		iterator->m_IntervalByRegistTime += fTime;
		if( !SingleObject->CheckPausedTimerIPC(iterator, fTime) ) //[FIXLOG:8-31] 추가 : 일시정지 된 IPC는 노티파이 체크하지 않는다
			SingleObject->checkAndNotifyByAlertTime(iterator);
	}
}

/* 현재의 시각과 IPC의 등록된 시각의 차를 가지고 notify 함수의 호출여부를 판단하는 함수.
 * 여기서 IPC가 가지고 있는 Alert Status를 파악하여 다시 재호출을 위해 준비할 것인지,
 *  일회성이므로 IPC를 삭제할 것인지 파악하고 처리 하게 된다.
 *  타이머의 가장 중요한 기능을 하는 기능함수
 *
 *  @pIPC : 확인할 IPC객체
 */
void npTimer::checkAndNotifyByAlertTime(npTimerIPC* pIPC){
	//IPC가 등록된 시각과 현재의 시간의 차이를 구한다.
	//IPC가 등록된 시각 - 등록된지 지난 시간
	//float DelayTime = pIPC->m_RegistedTime - pIPC->m_IntervalByRegistTime;
	float DelayTime = SingleObject->m_fCurrentTime - pIPC->m_RegistedTime;

/*	LOGfloatString("Gap TIme : ",DelayTime);
	LOGfloatString("AlertTime : ",pIPC->m_AlertTime);*/

	//차이의 시간이 IPC가 가진 AlertTime보다 많다면 Observer 에게 notify한다.
	if(DelayTime >= pIPC->m_AlertTime){
		pIPC->observer->wakeUpNotify();

		switch(pIPC->m_TimerStatus){
			case ONCE:{
				//해당 IPC를 Timer Checking에서 빠트린다.
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
				// Repeatloop의 경우 다른 행위를 하지 않는다.
				break;
			}
		}
	}
}

//[FIXLOG:8-31] 추가 : 일시정지된 IPC 처리
bool npTimer::CheckPausedTimerIPC(npTimerIPC* pIPC, float fTime)
{
	//등록된시간을 같이 늘이면 시간이 가지않는것과 같은 효과가 생긴다
	//딜레이타임 구하기 공식 때문에
	if( pIPC->observer->m_bNotifyPause)
		pIPC->m_RegistedTime += fTime;

	return pIPC->observer->m_bNotifyPause;
}

/* 타이머를 초기화 하는 함수.
 * 타이머는 단일 객체만을 가지고 있으므로 이를 생성하고 타이머를 사용할수 있게 하기 위해 사용되는 함수다.
 * 타이머를 사용하기전에는 항상 호출하여야 한다.
 */
void npTimer::initialize(){
	if(SingleObject == NULL){
		SingleObject = new npTimer;
		SingleObject->DataCellStore = new npTimerIPC;
		SingleObject->DataCellStore->setHeader();
	}
}

/*타이머의 사용이 모두 끝나고, 메인 프로세스가 종료되는 시점에 호출되어야 할 함수.
 * 타이머 내부에 생성된 객체들을 정리하고 메모리 Deallocate 작업을 수행한다.
 */
void npTimer::release(){
	//DataStore(Observer Collection을 모두 Deallocate한다)
	npTimerIPC* iterator;

	//destroyListHead의 node들을 돌면서 객체들을 실제로 지우는 작업을 한다.
	for(iterator = SingleObject->DataCellStore->next; iterator != SingleObject->DataCellStore; ){
		npTimerIPC* nextTarget = iterator->next;

		delete iterator;

		iterator = nextTarget;
	}
	delete SingleObject->DataCellStore;
	//SingleObject자체를 삭제하여 Timer의 사용을 그만둔다.
	delete SingleObject;

	LOGI("npTimer Release");
}
}
