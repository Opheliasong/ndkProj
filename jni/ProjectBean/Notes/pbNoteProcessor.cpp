/*
 * pbNoteProcesser.cpp
 *
 *  Created on: 2012. 7. 16.
 *      Author: NitroPigSoft02
 */
#include "pbNoteProcessor.h"

//=======================================
//Note Process Area
//=======================================
pbNoteProcessor* pbNoteProcessor::SingleObject;

pbNoteProcessor::pbNoteProcessor():m_NoteCounts(0),m_ControlledNoteStore(NULL){
	m_NoteDropper = NULL;
	m_deleteStacks = NULL;
}

pbNoteProcessor::~pbNoteProcessor(){

}

void pbNoteProcessor::Create(){
	LOGE("pbNoteProcessor) Note Processor Create");
	if(SingleObject == NULL){
//		LOGE("pbNoteProcessor) Note Processor Not Has");
		SingleObject = new pbNoteProcessor;
//		SingleObject->m_NoteDropper = new pbNoteDropper;
		SingleObject->m_ControlledNoteStore = new npLinkNode<pbNoteElement*>;
		SingleObject->m_deleteStacks = new npLinkNode<pbNoteElement*>;
		SingleObject->m_ControlledNoteStore->setHeader();
		SingleObject->m_deleteStacks->setHeader();

		LOGI("pbNoteProcess Initialize Complete");
		return;
	}

	LOGE("pbNoteProcess Initialize Failed");
}

void pbNoteProcessor::LoadData() {
//	m_NoteDropper->SettingPatternGenerator();
}

void pbNoteProcessor::Update(float time){
//	//1)delete Stack에 올라간 Note가 존재하는지 확인.
	DeleteStackArrangement();

	//캐릭터와 충돌하는 노트가 있는지 확인
	CheckMissNote();

	npLinkNode<pbNoteElement*>* iterator;
	npLinkNode<pbNoteElement*>* head = m_ControlledNoteStore;
	iterator = head->getNext();
	while (iterator != head) {
		pbNoteElement* pNote = iterator->getKernel();
		iterator = iterator->getNext();
		pNote->Update(time);
	}
}

//딜리트 스택 정리
void pbNoteProcessor::DeleteStackArrangement() {
	if(!(npLinkNode<pbNoteElement*>::IsEmpty(m_deleteStacks))){
		//1-1) 만약 Delete StacK에 올라간 note 가 존재한다면 Loop 및 Miss Check가 시작되기 전에 ControlledList에서 삭제
		npLinkNode<pbNoteElement*>* deleteIterator = m_deleteStacks->getNext();
		int cnt=0;
		for(; deleteIterator != m_deleteStacks;){
			npLinkNode<pbNoteElement*>* deleteNodeBakup = deleteIterator->getNext();
			npLinkNode<pbNoteElement*>::findRemove(deleteIterator,m_ControlledNoteStore);
			m_NoteDropper->RemoveNoteAndReturningMemory(deleteIterator->getKernel());
			TouchLayer::GetInstance().RemovedObserver(deleteIterator->getKernel());
			npLinkNode<pbNoteElement*>::destroyNode(deleteIterator);
			deleteIterator = deleteNodeBakup;
		}
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

/*void pbNoteProcessor::RenderingAll() {
	//1) ContorledNoteStore가 Empty가 아닐 경우 Rendering 처리를 하게 한다.
	if(!npLinkNode<pbNoteElement*>::IsEmpty(m_ControlledNoteStore)){
		npLinkNode<pbNoteElement*>* iterateNote = m_ControlledNoteStore->getNext();
		for(;iterateNote != m_ControlledNoteStore; iterateNote = iterateNote->getNext()){
			npRenderprocess::getInstance().DoDraw(*iterateNote->getKernel());
		}
	}
}*/

void pbNoteProcessor::PreSettingDraw() {
	glPushMatrix();
}
void pbNoteProcessor::DrawThis() {
	if(!npLinkNode<pbNoteElement*>::IsEmpty(m_ControlledNoteStore)){
		npLinkNode<pbNoteElement*>* iterateNote = m_ControlledNoteStore->getNext();
		while(iterateNote != m_ControlledNoteStore){
			npRenderprocess::getInstance().DoDraw( *(iterateNote->getKernel()) );
			iterateNote = iterateNote->getNext();
		}

	}
	glPopMatrix();
}

void pbNoteProcessor::CheckTargetingAbleOrUnalbe() {
	//Loop를 돌면서 해당 위치보다 낮거나 같은 곳에 있는 Note 들은 Targeting여부를
	//판독하고 Targeting이 필요한 Note들은 이를 처리한다.

	npLinkNode<pbNoteElement*>* Iterator = m_ControlledNoteStore->getNext();
	for(;Iterator != m_ControlledNoteStore;Iterator = Iterator->getNext()){
		pbTargetingNotes* kernel = (pbTargetingNotes*)Iterator->getKernel();
		//iterator의 Targeting On Indicate 확인
		//Targeting Indicate가 True가 아닌 노트는
		if(kernel->getPositionX() <= 700 && kernel->getNoteState() == pbNoteElement::NOTECREATE){
			if(!kernel->getTargetIndicate()){
				//Rand를 통해 현재 note를 Targeting 할 것인지 안 할 것인지 판단후 처리.
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

void pbNoteProcessor::PushExterminateButton() {
	npLinkNode<pbNoteElement*>* iterator;
	npLinkNode<pbNoteElement*>* head = m_ControlledNoteStore;

	iterator = head->getNext();
	while (iterator != head) {
		pbNoteElement* pNote = iterator->getKernel();
		iterator = iterator->getNext();
		pNote->NoteIsDead();
	}

}

void pbNoteProcessor::ClearDataStore(){
	if( m_ControlledNoteStore != NULL) {
		npLinkNode<pbNoteElement*>::clearList(m_ControlledNoteStore);
	}

	DeleteStackArrangement();

	if( m_deleteStacks != NULL) {
		npLinkNode<pbNoteElement*>::clearList(m_deleteStacks);
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

		 if( SingleObject->m_NoteDropper != NULL) {
			 delete SingleObject->m_NoteDropper;
			 SingleObject->m_NoteDropper = NULL;
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
//	m_NoteDropper->RemoveNoteAndReturningMemory(TabNote);
}

void pbNoteProcessor::ReleaseNote(pbDTabNotes* DTabNote) {
//	m_NoteDropper->RemoveNoteAndReturningMemory(DTabNote);
}

void pbNoteProcessor::ReleaseNote(pbLongPressNotes* LongPressNote) {
//	m_NoteDropper->RemoveNoteAndReturningMemory(LongPressNote);
}

void pbNoteProcessor::ReleaseNote(pbNinjaNotes* NinjaNote) {
//	m_NoteDropper->RemoveNoteAndReturningMemory(NinjaNote);
}

void pbNoteProcessor::RequestRemoveNote(pbNoteElement* pNote) {
	npLinkNode<pbNoteElement*>::addTail(pNote,m_deleteStacks);
}
