/*
 * pbNoteProcesser.cpp
 *
 *  Created on: 2012. 7. 16.
 *      Author: NitroPigSoft02
 */
#include "pbNoteProcessor.h"
//#include "npTimer.h"
//#include "pbTouchLayer.h"
//#include "pbRenderProcess.h"
//#include "pbDataStorage.h"

//=======================================
//Note Process Area
//=======================================
pbNoteDropper::pbNoteDropper() {
	//m_pDriedNoteArray = new pbDriedNoteArray;
	m_fGenerateTime = 0.0f;
	m_bGenerateState = false;

	m_pTargetNoteRentalUnit = new pbMemoryRentalUnit<pbTabNotes>;
	m_pDoubleTapNoteRentalUnit =  new pbMemoryRentalUnit<pbDTabNotes>;
	m_pLongNoteRentalUnit = new pbMemoryRentalUnit<pbLongPressNotes>;
	m_pNinjaNoteRentalunit = new pbMemoryRentalUnit<pbNinjaNotes>;

	m_pDoubleTapNoteRentalUnit->Initialize(10);
	m_pLongNoteRentalUnit->Initialize(10);
	m_pTargetNoteRentalUnit->Initialize(20);
	m_pNinjaNoteRentalunit->Initialize(10);

	LOGE("Create Note Dropper!");
}
pbNoteDropper::~pbNoteDropper() {
	/*
	if( m_pDriedNoteArray != NULL ) {
		m_pDriedNoteArray->ReleaseArray();
		delete m_pDriedNoteArray;

		LOGI("pbNoteDropper Destructor");
	}
	*/

	if (NP_IS_NOT_EMPTY(m_pDoubleTapNoteRentalUnit)) {
		m_pDoubleTapNoteRentalUnit->Release();
		delete m_pDoubleTapNoteRentalUnit;
		m_pDoubleTapNoteRentalUnit = NULL;
	}

	if (NP_IS_NOT_EMPTY(m_pLongNoteRentalUnit)) {
		m_pLongNoteRentalUnit->Release();
		delete m_pLongNoteRentalUnit;
		m_pLongNoteRentalUnit = NULL;
	}

	if (NP_IS_NOT_EMPTY(m_pTargetNoteRentalUnit)) {
		m_pTargetNoteRentalUnit->Release();
		delete m_pTargetNoteRentalUnit;
		m_pTargetNoteRentalUnit = NULL;
	}

	if(NP_IS_NOT_EMPTY(m_pNinjaNoteRentalunit)){
		m_pNinjaNoteRentalunit->Release();
		delete m_pNinjaNoteRentalunit;
		m_pNinjaNoteRentalunit = NULL;
	}
}

void pbNoteDropper::NoteDropCheck(){
}

void pbNoteDropper::RemoveNoteAndReturningMemory(pbNoteElement* pNote) {
	switch(pNote->getNoteType()){
		case pbNoteElement::TABNOTE:{
			pbTabNotes* removeTarget = (pbTabNotes*)pNote;
			RemoveNoteAndReturningMemory(removeTarget);
			break;
		}
		case pbNoteElement::DTABNOTE:{
			pbDTabNotes* removeTarget = (pbDTabNotes*)pNote;
			RemoveNoteAndReturningMemory(removeTarget);
			break;
		}
		case pbNoteElement::LONGPRESS:{
			pbLongPressNotes* removeTarget = (pbLongPressNotes*)pNote;
			RemoveNoteAndReturningMemory(removeTarget);
			break;
		}
		case pbNoteElement::NINJA:{
			pbNinjaNotes* removeTarget = (pbNinjaNotes*)pNote;
			RemoveNoteAndReturningMemory(removeTarget);
			break;
		}
	}
}

void pbNoteDropper::RemoveNoteAndReturningMemory(pbTabNotes* pNote) {
	if ( NP_IS_NOT_EMPTY(pNote)) {
		m_pTargetNoteRentalUnit->ReturningRentalMemory(pNote);
	}
}

void pbNoteDropper::RemoveNoteAndReturningMemory(pbDTabNotes* pNote) {
	if ( NP_IS_NOT_EMPTY(pNote)) {
		m_pDoubleTapNoteRentalUnit->ReturningRentalMemory(pNote);
	}
}

void pbNoteDropper::RemoveNoteAndReturningMemory(pbLongPressNotes* pNote) {
	if ( NP_IS_NOT_EMPTY(pNote)) {
		m_pLongNoteRentalUnit->ReturningRentalMemory(pNote);
	}
}

void pbNoteDropper::RemoveNoteAndReturningMemory(pbNinjaNotes* pNote) {
	if(NP_IS_NOT_EMPTY(pNote)){
		m_pNinjaNoteRentalunit->ReturningRentalMemory(pNote);
	}
}

void pbNoteDropper::onTimeAlerts() {
	float PosY, fSecondValueX, fSecondValueY;
	srand(time(NULL));
	int type = rand() % 4;
//	int type = 2;
	int Stair = rand() % 10;
	PosY = Stair *30 + 100;

	if(type == 0 ) {
		pbTabNotes* newOne = m_pTargetNoteRentalUnit->RentalMemory();
		newOne->ResetNoteState();
		newOne->setNotePosition(900,PosY);
		newOne->setTargetMarkSize(65.f,65.f);
		pbNoteProcessor::GetInstance()->AddNotes(newOne);
	}
	else if(type == 1 ) {
		pbDTabNotes* newOne = m_pDoubleTapNoteRentalUnit->RentalMemory();
		newOne->ResetNoteState();
		newOne->setNotePosition(900, PosY);
		newOne->setTargetMarkSize(65.f,65.f);
		pbNoteProcessor::GetInstance()->AddNotes(newOne);
	}
	else if(type == 2 ) {
		pbLongPressNotes* newOne = m_pLongNoteRentalUnit->RentalMemory();
		newOne->ResetNoteState();
		newOne->setNotePosition(900,PosY);
		newOne->setTargetMarkSize(65.f,65.f);
		pbNoteProcessor::GetInstance()->AddNotes(newOne);
	}
	else if(type == 3 ) {
		pbNinjaNotes* newOne = m_pNinjaNoteRentalunit->RentalMemory();
		newOne->ResetNoteState();
		newOne->setNotePosition(900,PosY);
		newOne->setNoteSize(110.f,110.f);
		newOne->setTargetMarkSize(65.f,65.f);
		pbNoteProcessor::GetInstance()->AddNotes(newOne);
	}

}

//float pbNoteDropper::resetWakeTime() {
//	return m_fGenerateTime;
//}

//======================================================================================//

//=======================================
//Note Process Area
//=======================================
pbNoteProcessor* pbNoteProcessor::SingleObject;

pbNoteProcessor::pbNoteProcessor():m_NoteCounts(0),m_ControlledNoteStore(NULL){
	m_NoteDropper = NULL;

}

pbNoteProcessor::~pbNoteProcessor(){

}

void pbNoteProcessor::Create(){
	LOGE("pbNoteProcessor) Note Processor Create");
	if(SingleObject == NULL){
		LOGE("pbNoteProcessor) Note Processor Not Has");
		SingleObject = new pbNoteProcessor;
		SingleObject->m_NoteDropper = new pbNoteDropper;
		SingleObject->m_ControlledNoteStore = new npLinkNode<pbNoteElement*>;
		SingleObject->m_deleteStacks = new npLinkNode<pbNoteElement*>;
		SingleObject->m_ControlledNoteStore->setHeader();
		SingleObject->m_deleteStacks->setHeader();

		LOGI("pbNoteProcess Initialize Complete");
		//NoteDropper ????????? ??????
		nitroFrame::npTimer::getInstance().registerObserver(SingleObject->m_NoteDropper,nitroFrame::npTimer::REPEATLOOP,1120.f);
		return;
	}

	LOGE("pbNoteProcess Initialize Failed");
}

void pbNoteProcessor::Update(float time){
	npLinkNode<pbNoteElement*>* iterator;
	npLinkNode<pbNoteElement*>* head = m_ControlledNoteStore;

	//1)delete Stack??? ????????? Note??? ??????????????? ??????.
	if(!(npLinkNode<pbNoteElement*>::IsEmpty(m_deleteStacks))){
		//1-1) ?????? Delete StacK??? ????????? note ??? ??????????????? Loop ??? Miss Check??? ???????????? ?????? ControlledList?????? ??????
		npLinkNode<pbNoteElement*>* deleteIterator = m_deleteStacks->getNext();
		int cnt=0;
		for(; deleteIterator != m_deleteStacks;){
			cnt++;
			npLinkNode<pbNoteElement*>* deleteNodeBakup = deleteIterator->getNext();
			npLinkNode<pbNoteElement*>::findRemove(deleteIterator,m_ControlledNoteStore);
			m_NoteDropper->RemoveNoteAndReturningMemory(deleteIterator->getKernel());
			npLinkNode<pbNoteElement*>::destroyNode(deleteIterator);
			deleteIterator = deleteNodeBakup;
		}
	}
	CheckMissNote();
	CheckTargetingAbleOrUnalbe();

	iterator = head->getNext();
	while (iterator != head) {
		pbNoteElement* pNote = iterator->getKernel();
		iterator = iterator->getNext();
		pNote->Update(time);
	}

}

void pbNoteProcessor::CheckMissNote(){
	if(npLinkNode<pbNoteElement*>::IsEmpty(m_ControlledNoteStore)){
		return;
	}

	npLinkNode<pbNoteElement*>* Iterator = m_ControlledNoteStore->getNext();
	pbTargetingNotes* kernel;

	for(;Iterator != m_ControlledNoteStore;Iterator = Iterator->getNext()){
		kernel = (pbTargetingNotes*)Iterator->getKernel();
		//If note states is 'NORMAL' and lower character's X-axis position than Note's X-axis position
		if(kernel->getPositionX() <= 100.f){
//		&& kernel->getPositionX() < pbCharacter::GetInstance()->GetPosX()){
		//this Note was judged miss.
			//Step.1 ) DecreaseLife
			//pbCharacter::GetInstance()->DecreaseLife();

			//Step.2 ) Reset Combo

			//Step.3 ) Releasing Note
			//Step.3-1) That note has removed that Touchlayer
			TouchLayer::GetInstance().RemovedObserver(kernel);
			//Step.3-2) Release note on memory
//			m_NoteDropper->RemoveNoteAndReturningMemory(kernel);
			RequestRemoveNote(kernel);
		}
	}
}

void pbNoteProcessor::RenderingAll() {
	//1) ContorledNoteStore??? Empty??? ?????? ?????? Rendering ????????? ?????? ??????.
	if(!npLinkNode<pbNoteElement*>::IsEmpty(m_ControlledNoteStore)){
		npLinkNode<pbNoteElement*>* iterateNote = m_ControlledNoteStore->getNext();
		for(;iterateNote != m_ControlledNoteStore; iterateNote = iterateNote->getNext()){
			npRenderprocess::getInstance().DoDraw(*iterateNote->getKernel());
		}
	}
}

void pbNoteProcessor::CheckTargetingAbleOrUnalbe() {
	//Loop??? ????????? ?????? ???????????? ????????? ?????? ?????? ?????? Note ?????? Targeting?????????
	//???????????? Targeting??? ????????? Note?????? ?????? ????????????.

	npLinkNode<pbNoteElement*>* Iterator = m_ControlledNoteStore->getNext();
	for(;Iterator != m_ControlledNoteStore;Iterator = Iterator->getNext()){
		pbTargetingNotes* kernel = (pbTargetingNotes*)Iterator->getKernel();
		//iterator??? Targeting On Indicate ??????
		//Targeting Indicate??? True??? ?????? ?????????
		if(kernel->getPositionX() <= 700 && kernel->getNoteState() == pbNoteElement::NOTECREATE){
			if(!kernel->getTargetIndicate()){
				//Rand??? ?????? ?????? note??? Targeting ??? ????????? ??? ??? ????????? ????????? ??????.
				srand(time(NULL));
				int type = rand() % 30;
				if(type <= 20){
					kernel->SetOnTargeting();
					kernel->setNoteState(pbNoteElement::NORMAL);
				}else{
					kernel->setNoteState(pbNoteElement::NORMAL);
				}
			}
		}
	}
}

void pbNoteProcessor::ClearDataStore(){
	if( m_ControlledNoteStore != NULL) {
		npLinkNode<pbNoteElement*>::clearList(m_ControlledNoteStore);
	}

	if( m_deleteStacks != NULL) {
		npLinkNode<pbNoteElement*>::clearList(m_deleteStacks);
	}
	 if( m_NoteDropper != NULL) {
		 delete m_NoteDropper;
		 m_NoteDropper = NULL;
	 }

	 LOGI("NoteProcessor ClearDataStrore");
}

void pbNoteProcessor::Release(){
	if( SingleObject != NULL) {
		SingleObject->ClearDataStore();

		if (SingleObject->m_ControlledNoteStore != NULL) {
			delete SingleObject->m_ControlledNoteStore;
			SingleObject->m_ControlledNoteStore = NULL;
		}

		delete SingleObject;
		LOGI("pbNoteProcess Release");

		SingleObject = NULL;
	}
}

void pbNoteProcessor::AddNotes(pbTabNotes* TabNote){
	npLinkNode<pbNoteElement*>::addTail((pbNoteElement*)TabNote,m_ControlledNoteStore);
	TabNote->setNoteState(pbNoteElement::NOTECREATE);
	TouchLayer::GetInstance().RegistedObserver(TabNote);

}

void pbNoteProcessor::AddNotes(pbDTabNotes* DTabNote) {
	npLinkNode<pbNoteElement*>::addTail((pbNoteElement*)DTabNote,m_ControlledNoteStore);
	DTabNote->setNoteState(pbNoteElement::NOTECREATE);
	TouchLayer::GetInstance().RegistedObserver(DTabNote);
}

void pbNoteProcessor::AddNotes(pbLongPressNotes* LongPressNote) {
	npLinkNode<pbNoteElement*>::addTail((pbNoteElement*)LongPressNote,m_ControlledNoteStore);
	LongPressNote->setNoteState(pbNoteElement::NOTECREATE);
	TouchLayer::GetInstance().RegistedObserver(LongPressNote);
}

void pbNoteProcessor::AddNotes(pbNinjaNotes* NinjaNote) {
	npLinkNode<pbNoteElement*>::addTail((pbNoteElement*)NinjaNote,m_ControlledNoteStore);
	NinjaNote->setNoteState(pbNoteElement::NOTECREATE);
	TouchLayer::GetInstance().RegistedObserver(NinjaNote);
}

void pbNoteProcessor::ReleaseNote(pbTabNotes* TabNote) {
	m_NoteDropper->RemoveNoteAndReturningMemory(TabNote);
}

void pbNoteProcessor::ReleaseNote(pbDTabNotes* DTabNote) {
	m_NoteDropper->RemoveNoteAndReturningMemory(DTabNote);
}

void pbNoteProcessor::ReleaseNote(pbLongPressNotes* LongPressNote) {
	m_NoteDropper->RemoveNoteAndReturningMemory(LongPressNote);
}

void pbNoteProcessor::ReleaseNote(pbNinjaNotes* NinjaNote) {
	m_NoteDropper->RemoveNoteAndReturningMemory(NinjaNote);
}

void pbNoteProcessor::RequestRemoveNote(pbNoteElement* pNote) {
	npLinkNode<pbNoteElement*>::addTail(pNote,m_deleteStacks);
}
