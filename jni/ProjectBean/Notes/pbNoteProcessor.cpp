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
	m_pLongNoteRentalUnit->Initialize(5);
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
//	if( !pbBoss::GetInstance()->IsBattlePhase() ) {
//		while( m_pDriedNoteArray->IsExistUsableDriedNote())
//		{
//			pbDriedNote* pDriedNote = m_pDriedNoteArray->GetDriedNote_ByCallCount();
//			if( pDriedNote == NULL)		{
//				m_pDriedNoteArray->IncreaseCallCount();
//				LOGE("PRODUCT POINTER IS NULL");
//				continue;
//			}
//
//			float fProductPosX = pDriedNote->fPosX  + pbGlobalInGameVariable::fWorldX;
//
//			if( fProductPosX < 900.f){
//				//m_pDriedNoteArray->IncreaseCallCount();
//				LOGfloatString("ProductX", fProductPosX);
//				LOGE("PRODUCT CHECK : PASS");
//
//				pbNoteProcessor::GetInstance()->CreateNote(pDriedNote, fProductPosX, pDriedNote->fPosY);
//			}	//end if Distance
//			else
//				break;
//		}//end while
//	}
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

void pbNoteDropper::SetGenerateNote(bool On) {
	m_bGenerateState = On;
}

/***
 * @details NoteDropper의 onTimerAlerts는 Note의 생성 타이밍일 경우 작동하게 된다.
 * 현 project Bean의 기획 사항에서는 이 타이밍일 경우 Note를 생성하여 Note Processer에게 넘겨주면 된다.
 * 전체 note의 instance관리는 note processor에서 관리하게 되므로, 이를 넘겨주는 작업만 하게 되면 된다.
 */
//		for( int i = 0 ; i < numChain; i ++)
//		{
//			pbSwipeNote* newSwipeNote = m_pSwipeNoteRentalUnit->RentalMemory();
//			if( newSwipeNote == NULL) return;
//			newSwipeNote->ValueInitializer();
//			newSwipeNote->setTypes(NOTETYPE::SWIPE);
////			newSwipeNote->m_ID = pDriedNote->ID;
//			newSwipeNote->setVertexIndices(3, 1);
//			newSwipeNote->setUVIndices(0, 19);
//
//			float fTouchWidth = pbDataStorage::GetVertexWidth(3);
//			float fTouchHeight = pbDataStorage::GetVertexHeight(3);
//			newSwipeNote->setTouchWidthAndHeight(fTouchWidth,fTouchHeight);
//
//			float Width = i*fChainVectorX*pbDataStorage::GetVertexWidth(0);
//			float Height = i*fChainVectorY*pbDataStorage::GetVertexHeight(0);
//			newSwipeNote->setNotePosition(X + Width, Y + Height  +8.0f);
//			newSwipeNote->ExtraInitialize();
//
//			newSwipeNote->SetTargetOnSwitch(true);
//
//			nitroFrame::npTimer::registerObserver(newSwipeNote,nitroFrame::npTimer::CHANGEDLOOP,0);
//
//			registControled(newSwipeNote);
//			pbTouchLayer::registerObserver(newSwipeNote);
//			pbRenderProcess::RegistRenderNote(newSwipeNote);
//		}
//		LOGW("PRODUCT SWIPE : Create");

void pbNoteDropper::onTimeAlerts() {
	//LOGE("NoteDropper)Call on Time Alerts");

	float PosY, fSecondValueX, fSecondValueY;
	srand(time(NULL));
	int type = rand() % 4;
//	int type = 0;
	int Stair = rand() % 10;
	PosY = Stair *30 + 100;

	//TODO 객체를 추가 할 때에는 반드시 Note의 Data 초기화를 해주어야 한다.
	if(type == 0 ) {
//		int numChain = (int)(rand() % 3 + 2);
//		m_fGenerateTime =  numChain*100 + 800;
//		fSecondValueX = 0.8f;
//		fSecondValueY = (rand() % 800 - 400)/1000.f;
//		pbNoteProcessor::GetInstance()->AddSwipeNote(900, PosY, numChain, fSecondValueX, fSecondValueY);
		//pbNoteProcessor::GetInstance()->AddNotes();

//		pbTabNotes* newOne = m_pTargetNoteRentalUnit->RentalMemory();
//		newOne->ResetNoteState();
//		newOne->setNotePosition(600,PosY);
//		newOne->setTargetMarkSize(65.f,65.f);
//		newOne->SetOnTargeting();

//		//LOGE("newOne Rental Index : %d",newOne->m_RentalIndex);
//		pbNoteProcessor::GetInstance()->AddNotes(newOne);
	}
	else if(type == 1 ) {
//		fSecondValueX = (rand() % 300 + 200);
//		m_fGenerateTime =  fSecondValueX + 800;
//		pbNoteProcessor::GetInstance()->AddLongNote(900, PosY, fSecondValueX, fSecondValueY);
		//LOGE("Add Long Notes");

		pbDTabNotes* newOne = m_pDoubleTapNoteRentalUnit->RentalMemory();
		newOne->ResetNoteState();
		newOne->setNotePosition(400, PosY);
		newOne->setTargetMarkSize(65.f,65.f);
		newOne->SetOnTargeting();

		pbNoteProcessor::GetInstance()->AddNotes(newOne);
	}
	else if(type == 2 ) {
//		m_fGenerateTime =  800;
//		pbNoteProcessor::GetInstance()->AddTargetingNote(900, PosY);
		//LOGE("Add Dtab Note");
	}
	else if(type == 3 ) {
	//		m_fGenerateTime =  800;
	//		pbNoteProcessor::GetInstance()->AddTargetingNote(900, PosY);
//			LOGE("Add NinJa Note");
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

pbNoteProcessor::pbNoteProcessor():m_NoteCounts(0),m_ControledNoteStore(NULL){
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
		SingleObject->m_ControledNoteStore = new npLinkNode<pbNoteElement*>;
		SingleObject->m_deleteStacks = new npLinkNode<pbNoteElement*>;
		SingleObject->m_ControledNoteStore->setHeader();
		SingleObject->m_deleteStacks->setHeader();

		LOGI("pbNoteProcess Initialize Complete");
		//NoteDropper 타이머 등록
		nitroFrame::npTimer::getInstance().registerObserver(SingleObject->m_NoteDropper,nitroFrame::npTimer::REPEATLOOP,1000.f);

//		//Test Code ) Dummy note를 생성하고 이를 추가한다.
//		//1) TabNote Dummy를 하나 추가시키기 위해 위치와, 크기를 설정하고, 이를 Controled Store에 등록한다.
//		pbTabNotes* DummyTabNote = new pbTabNotes;
//		DummyTabNote->ResetNoteState();
//		//DummyTabNote->setNotePosition(500,480);
//		DummyTabNote->setNotePosition(500,100);
//		DummyTabNote->setTargetMarkSize(75.f,75.f);
//		TouchLayer::GetInstance().RegistedObserver(DummyTabNote);
//		npLinkNode<pbNoteElement*>::addTail(DummyTabNote, SingleObject->m_ControledNoteStore);
//		DummyTabNote->SetOnTargeting();
//
//		pbTabNotes* DummyTabNote2 = new pbTabNotes;
//		DummyTabNote2->ResetNoteState();
//		DummyTabNote2->setNotePosition(500,300);
//		DummyTabNote2->setTargetMarkSize(75.f,75.f);
//		TouchLayer::GetInstance().RegistedObserver(DummyTabNote2);
//		npLinkNode<pbNoteElement*>::addTail(DummyTabNote2, SingleObject->m_ControledNoteStore);
//		DummyTabNote2->SetOnTargeting();
//
//		pbTabNotes* DummyTabNote3 = new pbTabNotes;
//		DummyTabNote3->ResetNoteState();
//		DummyTabNote3->setNotePosition(100,300);
//		TouchLayer::GetInstance().RegistedObserver(DummyTabNote3);
//		npLinkNode<pbNoteElement*>::addTail(DummyTabNote3, SingleObject->m_ControledNoteStore);
//
//		pbTabNotes* DummyTabNote4 = new pbTabNotes;
//		DummyTabNote4->ResetNoteState();
//		DummyTabNote4->setNotePosition(100,100);
//		TouchLayer::GetInstance().RegistedObserver(DummyTabNote4);
//		npLinkNode<pbNoteElement*>::addTail(DummyTabNote4, SingleObject->m_ControledNoteStore);
		return;
	}

	LOGE("pbNoteProcess Initialize Failed");
}

//void pbNoteProcessor::CreateNote(pbNoteElement* pDriedNote, float PosX, float PosY)
//{
//	switch(pDriedNote->NoteType)		{
//			case NOTETYPE::TARGETING:{
//				AddTargetingNote(PosX, PosY);
//			}
//			break;
//			case NOTETYPE::SWIPE:
//			{	//-----------------------------����������Ʈ --------------------------------------//
//				pbDriedNote_Chain* pDriedNote_Chain = (pbDriedNote_Chain*)pDriedNote;
//				AddSwipeNote(PosX, PosY, pDriedNote_Chain->fChainVectorX, pDriedNote_Chain->fChainVectorY, pDriedNote_Chain->numChain);
//			}
//			break;
//			case NOTETYPE::DTAP:
//			{	//-----------------------------�����ǳ�Ʈ --------------------------------------//
//				pbDriedNote_DoublePos* pDriedNoteDPos = (pbDriedNote_DoublePos*)pDriedNote;
//				AddDoubTapNote(PosX, PosY, pDriedNoteDPos->fSecondDistanceX, pDriedNoteDPos->fSecondDistanceY);
//			}
//			break;
//			case NOTETYPE::LPRESS:
//			{	//-----------------------------�ճ�Ʈ --------------------------------------//
//				pbDriedNote_DoublePos* pDriedNoteDPos = (pbDriedNote_DoublePos*)pDriedNote;
//				AddLongNote(PosX, PosY, 	pDriedNoteDPos->fSecondDistanceX, pDriedNoteDPos->fSecondDistanceY);
//			}
//			break;
//		}//end switch
//}

void pbNoteProcessor::Update(float time){
	npLinkNode<pbNoteElement*>* iterator;
	npLinkNode<pbNoteElement*>* head = m_ControledNoteStore;

	//1)delete Stack에 올라간 Note가 존재하는지 확인.
	if(!(npLinkNode<pbNoteElement*>::IsEmpty(m_deleteStacks))){
		//1-1) 만약 Delete StacK에 올라간 note 가 존재한다면 Loop 및 Miss Check가 시작되기 전에 ControlledList에서 삭제
		npLinkNode<pbNoteElement*>* deleteIterator = m_deleteStacks->getNext();
		int cnt=0;
		for(; deleteIterator != m_deleteStacks;){
			cnt++;
			npLinkNode<pbNoteElement*>* deleteNodeBakup = deleteIterator->getNext();
			npLinkNode<pbNoteElement*>::findRemove(deleteIterator,m_ControledNoteStore);
			m_NoteDropper->RemoveNoteAndReturningMemory(deleteIterator->getKernel());
			npLinkNode<pbNoteElement*>::destroyNode(deleteIterator);
			deleteIterator = deleteNodeBakup;
		}
		LOGE("Delete Stack Cnt:%d",cnt);
	}
	//CheckMissNote();
	//CheckTargetingAbleOrUnalbe();
//	if(NP_IS_NOT_EMPTY(m_NoteDropper)){
//	}
	iterator = head->getNext();
	while (iterator != head) {
		pbNoteElement* pNote = iterator->getKernel();
		iterator = iterator->getNext();
		pNote->Update(time);
	}

}

void pbNoteProcessor::CheckMissNote(){
	//pbLinkNode<pbNote>* pFirstNoteNode = SingleObject->m_ControledNoteStore->getNext();
	//pbNote* pFirstNote = pFirstNoteNode->getKernel();
	//npLinkNode<pbNoteElement>* HeadNext = m_ControledNoteStore->getNext();
	//pbNoteElement* FirstNote = HeadNext->getKernel();
	//pbTargetingNotes* FirstNote = HeadNext->getKernel();

	//Loop를 돌면서 TargetIndicate가 On(True)된 note들은 위치 판독을 시작한다.
//	if (NP_IS_NOT_EMPTY(FirstNote)) {
//		if (FirstNote->getTargetIndicate()) {
//			//if( FirstNote->GetPosX()  < pbCharacter::GetInstance()->GetPosX() ) {//���ɻ� ����
//			//캐릭터의 X 축 위치와 Note의 X 축 위치를 비교한다.
//			if (FirstNote->getPositionX() < pbCharacter::GetInstance()->GetPosX() ) {
//				// if( pFirstNote->getTargetOnSwitch() ) {
//				//if( !pFirstNote->GetEndPhase()) {
//				//if(){
//				//pFirstNote->JudgeMissNote();
//				pbCharacter::GetInstance()->DecreaseLife();
//				//pbComboManager::GetInstance()->ResetCombo();
//				TouchLayer::GetInstance().RemovedObserver(FirstNote);
//			//캐릭터의 X 축 위치로 오지 않은 Note들은 처리 하지 않는다.
//			} else {
//				//pbTouchLayer::removeObserver(pFirstNote);
//				//TouchLayer::GetInstance().RemovedObserver(FirstNote);
//				//nitroFrame::npTimer::findRemove(pFirstNote);
//				//pFirstNote->SetEndPhase(true);
//				//pFirstNote->SetNoteState(NoteState::END);
//				//pFirstNote->SetNoteJudge(NOTEJUDGEMENT::MISS);
//			}
//		}
//	}

	npLinkNode<pbNoteElement*>* Iterator = m_ControledNoteStore->getNext();
	pbTargetingNotes* kernel;

	for(;Iterator == m_ControledNoteStore;Iterator = Iterator->getNext()){
		kernel = (pbTargetingNotes*)Iterator->getKernel();
		//If note states is 'NORMAL' and lower character's X-axis position than Note's X-axis position
		if(kernel->getNoteState() == pbNoteElement::NORMAL){
//		&& kernel->getPositionX() < pbCharacter::GetInstance()->GetPosX()){
		//this Note was judged miss.
			//Step.1 ) DecreaseLife
			//pbCharacter::GetInstance()->DecreaseLife();

			//Step.2 ) Reset Combo

			//Step.3 ) Releasing Note
			//Step.3-1) That note has removed that Touchlayer
			TouchLayer::GetInstance().RemovedObserver(kernel);
			//Step.3-2) Release note on memory
			m_NoteDropper->RemoveNoteAndReturningMemory(kernel);
		}
	}

}

void pbNoteProcessor::RenderingAll() {
	//1) ContorledNoteStore가 Empty가 아닐 경우 Rendering 처리를 하게 한다.
	if(!npLinkNode<pbNoteElement*>::IsEmpty(m_ControledNoteStore)){
		npLinkNode<pbNoteElement*>* iterateNote = m_ControledNoteStore->getNext();
		for(;iterateNote != m_ControledNoteStore; iterateNote = iterateNote->getNext()){
			npRenderprocess::getInstance().DoDraw(*iterateNote->getKernel());
		}
	}
}

void pbNoteProcessor::CheckTargetingAbleOrUnalbe() {
	//Loop를 돌면서 해당 위치보다 낮거나 같은 곳에 있는 Note 들은 Targeting여부를
	//판독하고 Targeting이 필요한 Note들은 이를 처리한다.

	npLinkNode<pbNoteElement*>* Iterator = m_ControledNoteStore->getNext();
	for(;Iterator != m_ControledNoteStore;Iterator = Iterator->getNext()){
		pbTargetingNotes* kernel = (pbTargetingNotes*)Iterator->getKernel();
		//iterator의 Targeting On Indicate 확인
		//Targeting Indicate가 True가 아닌 노트는
		if(kernel->getNoteState() == pbNoteElement::NOTECREATE){
			if(!kernel->getTargetIndicate()){
				//Rand를 통해 현재 note를 Targeting 할 것인지 안 할 것인지 판단후 처리.
			}
		}
	}
}

void pbNoteProcessor::ClearDataStore(){
	if( m_ControledNoteStore != NULL) {
		npLinkNode<pbNoteElement*>::clearList(m_ControledNoteStore);
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

		if (SingleObject->m_ControledNoteStore != NULL) {
			delete SingleObject->m_ControledNoteStore;
			SingleObject->m_ControledNoteStore = NULL;
		}

		delete SingleObject;
		LOGI("pbNoteProcess Release");

		SingleObject = NULL;
	}
}
/*
void pbNoteProcessor::registControled(pbNote* pNote){
	 pbLinkNode<pbNote>* pTargetNode =  pbLinkNode<pbNote>::makeLinkNode(pNote);
	 pbLinkNode<pbNote>::addTail(pTargetNode,SingleObject->m_ControledNoteStore);
}

void pbNoteProcessor::removeControled(pbNote* pNote){
	 pbLinkNode<pbNote>* pTargetNode =  pbLinkNode<pbNote>::makeLinkNode(pNote);
	 pbLinkNode<pbNote>::findDelete(pTargetNode,SingleObject->m_ControledNoteStore);

}
*/

//void pbNoteProcessor::AddNotes(pbTabNotes* TabNote) {
//	/*
//		pbTargetingNote* newOne = m_pTargetNoteRentalUnit->RentalMemory();
//
//		if( newOne == NULL) return;
//		newOne->ValueInitializer();
//		newOne->setTypes(NOTETYPE::TARGETING);
//	//	newOne->m_ID = pDriedNote->ID;
//		newOne->setVertexIndices(0, 1);
//		newOne->setUVIndices( 9, 19);
//
//		float fTouchWidth = pbDataStorage::GetVertexWidth(0);
//		float fTouchHeight = pbDataStorage::GetVertexHeight(0);
//
//		newOne->setTouchWidthAndHeight(fTouchWidth, fTouchHeight);
//		newOne->setNotePosition(X, Y);
//		newOne->randOnSwitch();
//
//		if(newOne->getTargetOnSwitch()){
//			//nitroFrame::npTimer::registerObserver(newOne,nitroFrame::npTimer::CHANGEDLOOP,0);
//		}
//
//		//registControled(newOne);
//		//pbTouchLayer::registerObserver(newOne);
//		//pbRenderProcess::RegistRenderNote(newOne);
// */
//}
void pbNoteProcessor::AddNotes(pbTabNotes* TabNote){
	npLinkNode<pbNoteElement*>::addTail((pbNoteElement*)TabNote,m_ControledNoteStore);
	TouchLayer::GetInstance().RegistedObserver(TabNote);

}

void pbNoteProcessor::AddNotes(pbDTabNotes* DTabNote) {
	npLinkNode<pbNoteElement*>::addTail((pbNoteElement*)DTabNote,m_ControledNoteStore);
	TouchLayer::GetInstance().RegistedObserver(DTabNote);
}

void pbNoteProcessor::AddNotes(pbLongPressNotes* LongPressNote) {
	npLinkNode<pbNoteElement*>::addTail((pbNoteElement*)LongPressNote,m_ControledNoteStore);
	TouchLayer::GetInstance().RegistedObserver(LongPressNote);
}

void pbNoteProcessor::AddNotes(pbNinjaNotes* NinjaNote) {
	npLinkNode<pbNoteElement*>::addTail((pbNoteElement*)NinjaNote,m_ControledNoteStore);
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


//void pbNoteProcessor::RemoveNoteAndReturningMemory(pbLongPressNote* pNote) {
//	if( pNote != NULL) {
//		removeControled(pNote);
//		m_pLongNoteRentalUnit->ReturningRentalMemory(pNote);
//	}
//}

//
//void pbNoteProcessor::AddTargetingNote(float X, float Y) {
//	pbTargetingNote* newOne = m_pTargetNoteRentalUnit->RentalMemory();
//	if( newOne == NULL) return;
//	newOne->ValueInitializer();
//	newOne->setTypes(NOTETYPE::TARGETING);
////	newOne->m_ID = pDriedNote->ID;
//	newOne->setVertexIndices(0, 1);
//	newOne->setUVIndices( 9, 19);
//
//	float fTouchWidth = pbDataStorage::GetVertexWidth(0);
//	float fTouchHeight = pbDataStorage::GetVertexHeight(0);
//	newOne->setTouchWidthAndHeight(fTouchWidth, fTouchHeight);
//
//	newOne->setNotePosition(X, Y);
//
//	newOne->randOnSwitch();
//
//	if(newOne->getTargetOnSwitch()){
//		nitroFrame::npTimer::registerObserver(newOne,nitroFrame::npTimer::CHANGEDLOOP,0);
//	}
//	registControled(newOne);
//	pbTouchLayer::registerObserver(newOne);
//	pbRenderProcess::RegistRenderNote(newOne);
//}
//
//void pbNoteProcessor::AddSwipeNote(float X, float Y, int numChain, float fChainVectorX, float fChainVectorY) {
//		for( int i = 0 ; i < numChain; i ++)
//		{
//			pbSwipeNote* newSwipeNote = m_pSwipeNoteRentalUnit->RentalMemory();
//			if( newSwipeNote == NULL) return;
//			newSwipeNote->ValueInitializer();
//			newSwipeNote->setTypes(NOTETYPE::SWIPE);
////			newSwipeNote->m_ID = pDriedNote->ID;
//			newSwipeNote->setVertexIndices(3, 1);
//			newSwipeNote->setUVIndices(0, 19);
//
//			float fTouchWidth = pbDataStorage::GetVertexWidth(3);
//			float fTouchHeight = pbDataStorage::GetVertexHeight(3);
//			newSwipeNote->setTouchWidthAndHeight(fTouchWidth,fTouchHeight);
//
//			float Width = i*fChainVectorX*pbDataStorage::GetVertexWidth(0);
//			float Height = i*fChainVectorY*pbDataStorage::GetVertexHeight(0);
//			newSwipeNote->setNotePosition(X + Width, Y + Height  +8.0f);
//			newSwipeNote->ExtraInitialize();
//
//			newSwipeNote->SetTargetOnSwitch(true);
//
//			nitroFrame::npTimer::registerObserver(newSwipeNote,nitroFrame::npTimer::CHANGEDLOOP,0);
//
//			registControled(newSwipeNote);
//			pbTouchLayer::registerObserver(newSwipeNote);
//			pbRenderProcess::RegistRenderNote(newSwipeNote);
//		}
//		LOGW("PRODUCT SWIPE : Create");
//}
//
//void pbNoteProcessor::AddDoubTapNote(float X, float Y, float fSecondDistanceX, float fSecondDistanceY) {
//	pbDoubleTapNote* newDoubleTapNote = m_pDoubleTapNoteRentalUnit->RentalMemory();
//	if( newDoubleTapNote == NULL) { LOGE("PRODUCT DTAP : Error"); return; }
//	newDoubleTapNote->ValueInitializer();
//	newDoubleTapNote->setTypes(NOTETYPE::DTAP);
////	newDoubleTapNote->m_ID = pDriedNote->ID;
//	newDoubleTapNote->setVertexIndices(0, 1);
//	newDoubleTapNote->setUVIndices( 9, 19);
//
//	float fTouchWidth = pbDataStorage::GetVertexWidth(0);
//	float fTouchHeight = pbDataStorage::GetVertexHeight(0);
//	newDoubleTapNote->setTouchWidthAndHeight(fTouchWidth,fTouchHeight);
//
//	newDoubleTapNote->setNotePosition(X, Y);
//	newDoubleTapNote->ExtraInitialize();
//	newDoubleTapNote->SetSecondNotePosDelta(fSecondDistanceX, fSecondDistanceY);		//�����ǳ�Ʈ �ɼ�
//
//	newDoubleTapNote->randOnSwitch();
//
//	if(newDoubleTapNote->getTargetOnSwitch()){
//		nitroFrame::npTimer::registerObserver(newDoubleTapNote,nitroFrame::npTimer::CHANGEDLOOP,0);
//	}
//
//
//	registControled(newDoubleTapNote);
//	pbTouchLayer::registerObserver(newDoubleTapNote);
//	pbRenderProcess::RegistRenderNote(newDoubleTapNote);
//
//	LOGW("PRODUCT DTAP : Create");
//}
//
//void pbNoteProcessor::AddLongNote(float X, float Y, float fSecondDistanceX, float fSecondDistanceY) {
//	pbLongPressNote* newLongPressNote = m_pLongNoteRentalUnit->RentalMemory();
//	if( newLongPressNote == NULL)  { LOGE("PRODUCT LPRESS : Error"); return; }
//	newLongPressNote->ValueInitializer();
//	newLongPressNote->setTypes(NOTETYPE::LPRESS);
////	newLongPressNote->m_ID = pDriedNote->ID;
//	newLongPressNote->setVertexIndices(2);
//	newLongPressNote->setUVIndices( 17, 19, 18);
//
//	float fTouchWidth = pbDataStorage::GetVertexWidth(2);
//	float fTouchHeight = pbDataStorage::GetVertexHeight(2);
//	newLongPressNote->setTouchWidthAndHeight(fTouchWidth,fTouchHeight);
//
//	newLongPressNote->setNotePosition(X, Y);
//	newLongPressNote->ExtraInitialize();
//	newLongPressNote->SetDestination(fSecondDistanceX, fSecondDistanceY);		//�����ǳ�Ʈ �ɼ�
//
//	newLongPressNote->randOnSwitch();
//
//	if(newLongPressNote->getTargetOnSwitch()){
//		nitroFrame::npTimer::registerObserver(newLongPressNote,nitroFrame::npTimer::CHANGEDLOOP,0);
//	}
//
//	registControled(newLongPressNote);
//	pbTouchLayer::registerObserver(newLongPressNote);
//	pbRenderProcess::RegistRenderNote(newLongPressNote);
//
//	LOGW("PRODUCT LPRESS : Create");
//}

//void pbDriedNoteArray::CreateArray(int size)
//{
//	m_ArraySize = size;
//	m_AddCount = 0;
//	m_CallCount = 0;
//	m_ppDriedNote = (pbDriedNote**)calloc(size, sizeof(pbDriedNote*) );
//
//	for( int i = 0; i <m_ArraySize; i++)
//		m_ppDriedNote[i] = NULL;
//
//}
//
//void pbDriedNoteArray::AddDriedNote(pbDriedNote* pDriedNote){
//	if(m_AddCount < m_ArraySize)
//	{
//		m_ppDriedNote[m_AddCount] = pDriedNote;
//		m_AddCount++;
//	}
//}
//
//void pbDriedNoteArray::ReleaseArray(){
//	if( m_ppDriedNote != NULL)
//	{
//		for(int i = 0; i < m_ArraySize; i++)
//		{
//			if( m_ppDriedNote[i] != NULL)
//				delete m_ppDriedNote[i];
//		}
//
//		free(m_ppDriedNote);
//	}
//
//	m_ppDriedNote = NULL;
//
//	LOGI("pbDriedNoteArray Release");
//}
