/*
 * npTimer.cpp
 *
 *  Created on: 2012. 7. 20.
 *      Author: Ophelia
 */

#include "npTimer.h"

namespace nitroFrame{

npTimer::npTimerIPC::npTimerIPC():observer(NULL),m_AlertTime(0.f),m_RegistedTime(0.f),m_IntervalByRegistTime(0.f){

}

npTimer::npTimerIPC::~npTimerIPC(){
	this->observer = NULL;
}

npTimer::npTimerIPC& npTimer::npTimerIPC::operator =(npTimerIPC& rightSide) {
	this->m_AlertTime = rightSide.m_AlertTime;
	this->m_IntervalByRegistTime = rightSide.m_IntervalByRegistTime;
	this->m_RegistedTime = rightSide.m_RegistedTime;
	this->m_TimerStatus = rightSide.m_TimerStatus;
	this->observer = rightSide.observer;
	return *this;
}

/*==============
 * Timer Area
 * ============== */
npTimer::npTimer():m_fCurrentTime(0),m_fTotalTime(0){
	DataCellStore = new npTimerLinkNode;
	DataCellStore->setHeader();

	DeleteQueue = new npTimerLinkNode;
	DeleteQueue->setHeader();
	LOGE("Create TImer Instance");
}

npTimer::~npTimer(){
	npTimerLinkNode::destroyList(DataCellStore);
	npTimerLinkNode::destroyList(DeleteQueue);

	LOGI("npTimer Release");
}

void npTimer::registerObserver(npTimerObserver* apObserver, float fAlertSec){

	npTimerIPC* registedOne = new npTimerIPC;
	registedOne->observer = apObserver;
	registedOne->m_RegistedTime = this->m_fCurrentTime;
	registedOne->m_AlertTime = fAlertSec;
	registedOne->m_TimerStatus = ONCE;
	registedOne->m_IntervalByRegistTime = 0;

	npTimerLinkNode::addTail(*registedOne,this->DataCellStore);
}

void npTimer::registerObserver(npTimerObserver* apObserver, float fAlertSec, float RepeatInterval){

	npTimerIPC* registedOne = new npTimerIPC;
	registedOne->observer = apObserver;
	registedOne->m_RegistedTime = this->m_fCurrentTime;
	registedOne->m_AlertTime = fAlertSec;
	registedOne->m_TimerStatus = REPEATLOOP;
	registedOne->m_IntervalByRegistTime = RepeatInterval;

	npTimerLinkNode::addTail(*registedOne,this->DataCellStore);
}

void npTimer::registerObserver(npTimerObserver* apObserver, TIMERSTATUS Status,	float fAlertSec) {
	npTimerIPC* registedOne = new npTimerIPC;
	registedOne->observer = apObserver;
	registedOne->m_RegistedTime = this->m_fCurrentTime;
	registedOne->m_AlertTime = fAlertSec;
	registedOne->m_TimerStatus = Status;
	registedOne->m_IntervalByRegistTime = fAlertSec;

	LOGE("Registed Observer");
	npTimerLinkNode::addTail(*registedOne,this->DataCellStore);
}

void npTimer::registerObserver(npTimerObserver* apObserver, TIMERSTATUS Status,	float fAlertSec, float RepeatInterval) {
	npTimerIPC* registedOne = new npTimerIPC;
	registedOne->observer = apObserver;
	registedOne->m_RegistedTime = this->m_fCurrentTime;
	registedOne->m_AlertTime = fAlertSec;
	registedOne->m_TimerStatus = Status;
	registedOne->m_IntervalByRegistTime = RepeatInterval;

	npTimerLinkNode::addTail(*registedOne,this->DataCellStore);
}

void npTimer::findRemove(npTimerObserver* apObserver){
	npTimerLinkNode* iterator;

	for(iterator = this->DataCellStore->getNext(); iterator == this->DataCellStore; iterator = iterator->getNext()){
		if(iterator->getKernel().observer == apObserver){
			npTimerLinkNode::destroyNode(iterator);
			delete iterator;
			return;
		}
	}
}

void npTimer::updateTime(float fTime){
	this->m_fCurrentTime += fTime * 1000;

	//1) DeleteQueue에 Node가 등록이 되어 있는지 확인한다.
	/*
	if(!DeleteQueue->IsEmpty()){
		//1-1) DeleteQueue에 node가 등록이 되어 있을 경우 DataCellStore에 있는 Instance를 지운다.
		npTimerLinkNode* deleteIterator = DeleteQueue->getNext();
		npTimerLinkNode::destroyNode(deleteIterator);
	}
	 */
	npTimerLinkNode* iterator = this->DataCellStore->getNext();

	for(; iterator != this->DataCellStore;iterator = iterator->getNext()){
		//iterator->getKernel().m_AlertTime += fTime;
		//만약 checkAndNotifyByAlertTime 함수에서 false Return signal을 보낸다면 loop도중의 현재 iterator 객체를 잠시 빼둔다.
		//if(!checkAndNotifyByAlertTime(iterator)){
		//	npTimerLinkNode* deleteTarget = iterator;
		//	iterator = iterator->getNext();
			//npTimerLinkNode::destroyNode(deleteTarget);
		//	npTimerLinkNode::destroyNode(iterator);
		//}else{
		//}
		checkAndNotifyByAlertTime(iterator);
	}

}

bool npTimer::checkAndNotifyByAlertTime(npTimerLinkNode* TimerNode){

	npTimerIPC* pIPC = &TimerNode->getKernel();
	float DelayTime = this->m_fCurrentTime - pIPC->m_RegistedTime;

	//TimerNode의 상태가 Paused가 아니고, Delay타임이  증가된 Observer만 TimeNotification을 한다.
	if(pIPC->m_AlertTime <= DelayTime && pIPC->m_TimerStatus != PAUSED){
		//LOGE("Alert Time :%f || Delay Time:%f",pIPC->m_AlertTime, DelayTime);
		pIPC->observer->onTimeAlerts();

		switch(pIPC->m_TimerStatus){
			case ONCE:{
				//한번 실행시 Loop를 그만두기 위해 delete Stack에 Node를 Add tail한다.
				npTimerLinkNode::addTail(TimerNode,DeleteQueue);
				return false;
			}
			case CHANGEDLOOP:
			case REPEATLOOP:{
				//LOGE("REPEATLOOP");
				pIPC->m_AlertTime = pIPC->m_IntervalByRegistTime;
				pIPC->m_RegistedTime = this->m_fCurrentTime;
				return true;
			}
		}
	}
	return true;
}



/***
 * @fn TimerObjserver의 객체를 DataCellStore에서 찾아내서 내부의 RepeatTime을 수정한다.
 * @param apObserver 수정대상 observer
 * @param ChangedTime 수정될 Time
 */
void npTimer::changeRepeatsTime(npTimerObserver* apObserver,	 float ChangedTime) {

}

npTimer& npTimer::getInstance() {
	static npTimer Instance;
	return Instance;
}

//
//bool npTimer::CheckPausedTimerIPC(npTimerIPC* pIPC, float fTime)
//{
//	if( pIPC->observer->m_bNotifyPause)
//		pIPC->m_RegistedTime += fTime;
//
//	return pIPC->observer->m_bNotifyPause;
//}

//void npTimer::initialize(){
//	if(SingleObject == NULL){
//		SingleObject = new npTimer;
//		SingleObject->DataCellStore = new npTimerIPC;
//		SingleObject->DataCellStore->setHeader();
//	}
//}
//
//void npTimer::release(){
//	npTimerIPC* iterator;
//
//	for(iterator = SingleObject->DataCellStore->next; iterator != SingleObject->DataCellStore; ){
//		npTimerIPC* nextTarget = iterator->next;
//
//		delete iterator;
//
//		iterator = nextTarget;
//	}
//	delete SingleObject->DataCellStore;
//	delete SingleObject;
//
//	LOGI("npTimer Release");
//}
/*==============
 * IPC Area
 * ============== */

//npTimer::npTimerIPC::npTimerIPC():prev(NULL),next(NULL),observer(NULL),CurrentType(NONE){
//}
//
//npTimer::npTimerIPC::~npTimerIPC(){
//
//}
//
//void npTimer::npTimerIPC::setHeader(){
//	this->CurrentType = npTimerIPC::HEAD;
//}
//npObserver* npTimer::npTimerIPC::findObserverByID(int ID, npTimerIPC* pHeader){
//
//}
//
//
//void npTimer::npTimerIPC::add(npTimerIPC* newOne, npTimerIPC* prev, npTimerIPC* next){
//
//	next->prev = newOne;
//	newOne->next = next;
//	newOne->prev = prev;
//	prev->next = newOne;
//
//}
//
//void npTimer::npTimerIPC::addTail(npObserver* pAdder,float fAlertTime, npTimerIPC* pHeader){
//	npTimerIPC* newOne = new npTimerIPC;
//	newOne->m_AlertTime = fAlertTime;
//	newOne->observer = pAdder;
//
//	add(newOne , pHeader->next , pHeader);
//}
//
//void npTimer::npTimerIPC::addTail(npTimerIPC* pAdder, npTimerIPC* pHeader){
//	//add(pAdder,pHeader->next,pHeader->prev);
//	add(pAdder , pHeader->prev, pHeader);
//}
//
//void npTimer::npTimerIPC::remove(npTimerIPC* pRemover){
//	if(pRemover->next != NULL && pRemover->prev != NULL){
//		LOGE("remove the prev & next");
//		pRemover->next->prev = pRemover->prev;
//		pRemover->prev->next = pRemover->next;
//	}
//	pRemover->prev = NULL;
//	pRemover->next = NULL;
//}
//
//void npTimer::npTimerIPC::findRemove(npTimerIPC* pRemover, npTimerIPC* pHeader){
//	npTimerIPC* iterator;
//
//	for(iterator =  pHeader->next; iterator != pHeader; iterator = iterator->next){
//		if(iterator == pRemover){
//			remove(iterator);
//		}
//	}
//}
//

}
