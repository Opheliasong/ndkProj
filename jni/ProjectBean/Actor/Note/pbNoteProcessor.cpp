/*
 * pbNoteProcesser.cpp
 *
 *  Created on: 2012. 7. 16.
 *      Author: NitroPigSoft02
 */
#include "pbNoteProcessor.h"
#include "pbTouchLayer.h"
#include "pbRenderProcess.h"
#include "npTimer.h"
#include "pbDataStorage.h"
#include "pbGlobalVariable.h"
#include "pbMainFrame.h"

namespace projectBean{
//--------------------------------�Ľ� �ؿ��� �淮ȭ�� ������ Ŭ���� : ���� ��Ʈ------------------------------------------------------------------------------------//
void pbDriedNoteArray::CreateArray(int size)
{
	m_ArraySize = size;
	m_AddCount = 0;
	m_CallCount = 0;
	m_ppDriedNote = (pbDriedNote**)calloc(size, sizeof(pbDriedNote*) );

	for( int i = 0; i <m_ArraySize; i++)
		m_ppDriedNote[i] = NULL;

}

void pbDriedNoteArray::AddDriedNote(pbDriedNote* pDriedNote){
	if(m_AddCount < m_ArraySize)
	{
		m_ppDriedNote[m_AddCount] = pDriedNote;
		m_AddCount++;
	}
}

void pbDriedNoteArray::ReleaseArray(){
	if( m_ppDriedNote != NULL)
	{
		for(int i = 0; i < m_ArraySize; i++)
		{
			if( m_ppDriedNote[i] != NULL)
				delete m_ppDriedNote[i];
		}

		free(m_ppDriedNote);
	}

	m_ppDriedNote = NULL;

	LOGI("pbDriedNoteArray Release");
}

//======================================================================================//


//=======================================
//Note Process Area
//=======================================
pbNoteDropper::pbNoteDropper() {
	m_pDriedNoteArray = new pbDriedNoteArray;
	m_fGenerateTime = 0.0f;
	m_bGenerateMode = false;
}
pbNoteDropper::~pbNoteDropper() {
	if( m_pDriedNoteArray != NULL ) {
		m_pDriedNoteArray->ReleaseArray();
		delete m_pDriedNoteArray;

		LOGI("pbNoteDropper Destructor");
	}
}

bool pbNoteDropper::DriedDataParsing(const char* fileName) {
	//------------XML �Ľ�--------//
	char* buffer = pbProjectBeanFrame::npGetAssetBuffer(fileName);
	if( buffer == NULL) return false;

	TiXmlDocument xmlDoc;
	xmlDoc.Parse(buffer);

	if(xmlDoc.Error()){
		LOGE("XmlParser error");
		return false;
	}

		//-----------------�ؽ��ĵ����� ��� �Ľ�----------------------//
		TiXmlNode* DataHeadNode = xmlDoc.FirstChild("NoteData")->FirstChild("NoteDataHeader");
		int count;
		DataHeadNode->ToElement()->Attribute("DataCount", &count);

		if( m_pDriedNoteArray == NULL) return false;

		m_pDriedNoteArray->CreateArray(count);

		TiXmlNode* DriedDataNode = DataHeadNode->NextSibling("DriedData");

		//-----------------DriedData ����---------------------//
		while( DriedDataNode != NULL) {
			CreateDriedNoteByType(DriedDataNode);

			DriedDataNode = DriedDataNode->NextSibling();
		}

	LOGI("Done NoteData Xml parsing");

	xmlDoc.Clear();
	pbProjectBeanFrame::npCloseAssetBuffer(buffer);

	return true;
}

void pbNoteDropper::CreateDriedNote_Targeting(float PosX, float PosY) {
	pbDriedNote* pTempDriedNote = new pbDriedNote();
	pTempDriedNote->NoteType = NOTETYPE::TARGETING;
	pTempDriedNote->fPosX = PosX;
	pTempDriedNote->fPosY = PosY;

	m_pDriedNoteArray->AddDriedNote(pTempDriedNote);

}

void pbNoteDropper::CreateDriedNote_Swipe(float PosX, float PosY, int numChain, float fChainVectorX, float fChainVectorY) {
	pbDriedNote_Chain* pTempDriedNote = new pbDriedNote_Chain();
	pTempDriedNote->NoteType = NOTETYPE::SWIPE;
	pTempDriedNote->fPosX = PosX;
	pTempDriedNote->fPosY = PosY;

	pTempDriedNote->numChain = numChain;
	pTempDriedNote->fChainVectorX =fChainVectorX;
	pTempDriedNote->fChainVectorY =fChainVectorY;

	m_pDriedNoteArray->AddDriedNote(pTempDriedNote);

}

void pbNoteDropper::CreateDriedNote_Long(float PosX, float PosY, float fSecondDistanceX, float fSecondDistanceY)  {
	pbDriedNote_DoublePos* pTempDriedNote = new pbDriedNote_DoublePos();
	pTempDriedNote->NoteType = NOTETYPE::LPRESS;
	pTempDriedNote->fPosX = PosX;
	pTempDriedNote->fPosY = PosY;

	pTempDriedNote->fSecondDistanceX =fSecondDistanceX;
	pTempDriedNote->fSecondDistanceY =fSecondDistanceY;

	m_pDriedNoteArray->AddDriedNote(pTempDriedNote);

}

void  pbNoteDropper::CreateDriedNote_DoubleTap(float PosX, float PosY, float fSecondDistanceX, float fSecondDistanceY) {
	pbDriedNote_DoublePos* pTempDriedNote = new pbDriedNote_DoublePos();
	pTempDriedNote->NoteType = NOTETYPE::DTAP;
	pTempDriedNote->fPosX = PosX;
	pTempDriedNote->fPosY = PosY;

	pTempDriedNote->fSecondDistanceX =fSecondDistanceX;
	pTempDriedNote->fSecondDistanceY =fSecondDistanceY;

	m_pDriedNoteArray->AddDriedNote(pTempDriedNote);
}

void pbNoteDropper::CreateDriedNoteByType(TiXmlNode* DriedDataNode) {
	float fStartPos = 400.f;
	const char* pcType = NULL;
	double PosX, PosY, fSecondValueX, fSecondValueY;

	pcType = DriedDataNode->ToElement()->Attribute("Type");

	if( strcmp(pcType, "SwipeNote") == 0 ) {
		//-----------------------------����������Ʈ --------------------------------------//
//		DriedDataNode->ToElement()->Attribute("ID", &ID);

		DriedDataNode = DriedDataNode->FirstChild("Position");
		DriedDataNode->ToElement()->Attribute("X", &PosX);
		DriedDataNode->ToElement()->Attribute("Y", &PosY);

		if( DriedDataNode->NextSibling("TouchArea") != NULL )
			LOGE("TouchArea  is Not Used ");

		DriedDataNode = DriedDataNode->NextSibling("ChainData");

		int numChain;
		DriedDataNode->ToElement()->Attribute("Chain", &numChain);
		DriedDataNode->ToElement()->Attribute("VectorX", &fSecondValueX);
		DriedDataNode->ToElement()->Attribute("VectorY", &fSecondValueY);

		CreateDriedNote_Swipe(PosX + fStartPos, PosY, numChain, fSecondValueX, fSecondValueY);
		//========================================================================//
	}
	else if( strcmp(pcType, "LongNote") == 0 ) {
		//-----------------------------�ճ�Ʈ --------------------------------------//
//		DriedDataNode->ToElement()->Attribute("ID", &pTempDriedNote->ID);

		DriedDataNode = DriedDataNode->FirstChild("Position");
		DriedDataNode->ToElement()->Attribute("X", &PosX);
		DriedDataNode->ToElement()->Attribute("Y", &PosY);

		if( DriedDataNode->NextSibling("TouchArea") != NULL )
			LOGE("TouchArea  is Not Used ");

		DriedDataNode = DriedDataNode->NextSibling("SecondDistance");
		DriedDataNode->ToElement()->Attribute("X", &fSecondValueX);
		DriedDataNode->ToElement()->Attribute("Y", &fSecondValueY);

		CreateDriedNote_Long(PosX + fStartPos, PosY, fSecondValueX, fSecondValueY);
		//========================================================================//
	}
	else if( strcmp(pcType, "DoubleTapNote") == 0 ) {
		//-----------------------------�����ǳ�Ʈ --------------------------------------//
//		DriedDataNode->ToElement()->Attribute("ID", &pTempDriedNote->ID);

		DriedDataNode = DriedDataNode->FirstChild("Position");
		DriedDataNode->ToElement()->Attribute("X", &PosX);
		DriedDataNode->ToElement()->Attribute("Y", &PosY);

		if( DriedDataNode->NextSibling("TouchArea") != NULL )
			LOGE("TouchArea is Not Used ");

		DriedDataNode = DriedDataNode->NextSibling("SecondDistance");
		DriedDataNode->ToElement()->Attribute("X", &fSecondValueX);
		DriedDataNode->ToElement()->Attribute("Y", &fSecondValueY);

		CreateDriedNote_DoubleTap(PosX + fStartPos, PosY, fSecondValueX, fSecondValueY);
		//========================================================================//
	}
	else if( strcmp(pcType, "TargetingNote") == 0 ) {
		//-----------------------------Ÿ���� ��Ʈ --------------------------------------//
		//DriedDataNode->ToElement()->Attribute("ID", &pTempDriedNote->ID);

		DriedDataNode = DriedDataNode->FirstChild("Position");
		DriedDataNode->ToElement()->Attribute("X", &PosX);
		DriedDataNode->ToElement()->Attribute("Y", &PosY);

		if( DriedDataNode->NextSibling("TouchArea") != NULL )
			LOGE("TouchArea  is Not Used ");

		CreateDriedNote_Targeting(PosX + fStartPos, PosY);
		//========================================================================//
	}
	else
		LOGE("pbNoteProcess :: CreateDriedNoteByType : Can't Find Note Type");

}

void pbNoteDropper::NoteDropCheck(){
	if( !pbBoss::GetInstance()->IsBattlePhase() ) {
		while( m_pDriedNoteArray->IsExistUsableDriedNote())
		{
			pbDriedNote* pDriedNote = m_pDriedNoteArray->GetDriedNote_ByCallCount();
			if( pDriedNote == NULL)		{
				m_pDriedNoteArray->IncreaseCallCount();
				LOGE("PRODUCT POINTER IS NULL");
				continue;
			}

			float fProductPosX = pDriedNote->fPosX  + pbGlobalInGameVariable::fWorldX;

			if( fProductPosX < 900.f)		//üũ �����ϸ� ���� Ż��
			{
				m_pDriedNoteArray->IncreaseCallCount();//������ üũ�ϰ� �ѱ�
				LOGfloatString("ProductX", fProductPosX);
				LOGE("PRODUCT CHECK : PASS");

				pbNoteProcessor::GetInstance()->CreateNote(pDriedNote, fProductPosX, pDriedNote->fPosY);

			}//end if Distance
			else
				break;
		}//end while
	}

}

void pbNoteDropper::SetGenerateNote(bool On) {
	if( On ) {
		nitroFrame::npTimer::registerObserver(this,nitroFrame::npTimer::CHANGEDLOOP,500);
		LOGE("pbNoteDropper::SetGenerateNote ON");
	}
	else {
		m_bNotifyPause = true;
		nitroFrame::npTimer::findRemove(this);
		LOGE("pbNoteDropper::SetGenerateNote OFF");
	}
}

void pbNoteDropper::wakeUpNotify() {
	float PosY, fSecondValueX, fSecondValueY;
	srand(time(NULL));
	int type = rand() % 3;
	int Stair = rand() % 10;
	PosY = Stair *30 + 100;

	if(type == 0 ) {
		int numChain = (int)(rand() % 3 + 2);
		m_fGenerateTime =  numChain*100 + 800;
		fSecondValueX = 0.8f;
		fSecondValueY = (rand() % 800 - 400)/1000.f;
		pbNoteProcessor::GetInstance()->AddSwipeNote(900, PosY, numChain, fSecondValueX, fSecondValueY);
	}
	else if(type == 1 ) {
		fSecondValueX = (rand() % 300 + 200);
		m_fGenerateTime =  fSecondValueX + 800;
		pbNoteProcessor::GetInstance()->AddLongNote(900, PosY, fSecondValueX, fSecondValueY);
	}
	else if(type == 2 ) {
		m_fGenerateTime =  800;
		pbNoteProcessor::GetInstance()->AddTargetingNote(900, PosY);
	}
/*	else if(type == 3 ) {
		PosY = rand() % 250 + 150;
		fSecondValueX = (rand() % 200 + 150);
		fSecondValueY = (rand() % 300 - 150);
		pbNoteProcessor::GetInstance()->AddDoubTapNote(900, PosY, fSecondValueX, fSecondValueY);
	}*/


}

float pbNoteDropper::resetWakeTime() {
	return m_fGenerateTime;
}


//======================================================================================//

//=======================================
//Note Process Area
//=======================================
pbNoteProcessor* pbNoteProcessor::SingleObject;


pbNoteProcessor::pbNoteProcessor():m_NoteCounts(0),m_ControledNoteStore(NULL){
	m_pSwipeNoteRentalUnit = NULL;
	m_pDoubleTapNoteRentalUnit =  NULL;
	m_pLongNoteRentalUnit = NULL;
	m_pTargetNoteRentalUnit = NULL;
	m_NoteDropper = NULL;
}

pbNoteProcessor::~pbNoteProcessor(){

}

void pbNoteProcessor::Create(){
	if(SingleObject == NULL){
		SingleObject = new pbNoteProcessor;

		//��Ʈ ���μ����� Store�� List�� Head�� �����Ѵ�.
		SingleObject->m_ControledNoteStore = new pbLinkNode<pbNote>();
		SingleObject->m_ControledNoteStore->setHeader();

		//-------------------------------------��Ż ���� �ʱ�ȭ------------------------------------------------------------------//
		SingleObject->m_pSwipeNoteRentalUnit = new pbMemoryRentalUnit<pbSwipeNote>;
		SingleObject->m_pDoubleTapNoteRentalUnit =  new pbMemoryRentalUnit<pbDoubleTapNote>;
		SingleObject->m_pLongNoteRentalUnit = new pbMemoryRentalUnit<pbLongPressNote>;
		SingleObject->m_pTargetNoteRentalUnit = new pbMemoryRentalUnit<pbTargetingNote>;

		SingleObject->m_pSwipeNoteRentalUnit->Initialize(20);
		SingleObject->m_pDoubleTapNoteRentalUnit->Initialize(5);
		SingleObject->m_pLongNoteRentalUnit->Initialize(5);
		SingleObject->m_pTargetNoteRentalUnit->Initialize(20);



		LOGI("pbNoteProcess Initialize Complete");

		return ;
	}
	LOGE("pbNoteProcess Initialize Failed");
}

bool pbNoteProcessor::LoadData(const char* fileName) {
	if( m_NoteDropper == NULL ) {
		m_NoteDropper = new pbNoteDropper;
		if( m_NoteDropper->DriedDataParsing(fileName) ) {
			LOGI("pbNoteProcess::LoadData Complete");
			return true;
		}
	}

	LOGE("pbNoteProcess::LoadData Failed");
	return false;
}


void pbNoteProcessor::AddTargetingNote(float X, float Y) {
	pbTargetingNote* newOne = m_pTargetNoteRentalUnit->RentalMemory();
	if( newOne == NULL) return;
	newOne->ValueInitializer();
	newOne->setTypes(NOTETYPE::TARGETING);
//	newOne->m_ID = pDriedNote->ID;
	newOne->setVertexIndices(0, 1);
	newOne->setUVIndices( 9, 19);

	float fTouchWidth = pbDataStorage::GetVertexWidth(0);
	float fTouchHeight = pbDataStorage::GetVertexHeight(0);
	newOne->setTouchWidthAndHeight(fTouchWidth, fTouchHeight);

	newOne->setNotePosition(X, Y);

	newOne->randOnSwitch();

	if(newOne->getTargetOnSwitch()){
		nitroFrame::npTimer::registerObserver(newOne,nitroFrame::npTimer::CHANGEDLOOP,0);
	}
	registControled(newOne);
	pbTouchLayer::registerObserver(newOne);
	pbRenderProcess::RegistRenderNote(newOne);
}

void pbNoteProcessor::AddSwipeNote(float X, float Y, int numChain, float fChainVectorX, float fChainVectorY) {
		for( int i = 0 ; i < numChain; i ++)
		{
			pbSwipeNote* newSwipeNote = m_pSwipeNoteRentalUnit->RentalMemory();
			if( newSwipeNote == NULL) return;
			newSwipeNote->ValueInitializer();
			newSwipeNote->setTypes(NOTETYPE::SWIPE);
//			newSwipeNote->m_ID = pDriedNote->ID;
			newSwipeNote->setVertexIndices(3, 1);
			newSwipeNote->setUVIndices(0, 19);

			float fTouchWidth = pbDataStorage::GetVertexWidth(3);
			float fTouchHeight = pbDataStorage::GetVertexHeight(3);
			newSwipeNote->setTouchWidthAndHeight(fTouchWidth,fTouchHeight);

			float Width = i*fChainVectorX*pbDataStorage::GetVertexWidth(0);
			float Height = i*fChainVectorY*pbDataStorage::GetVertexHeight(0);
			newSwipeNote->setNotePosition(X + Width, Y + Height  +8.0f);
			newSwipeNote->ExtraInitialize();

			newSwipeNote->SetTargetOnSwitch(true);

			nitroFrame::npTimer::registerObserver(newSwipeNote,nitroFrame::npTimer::CHANGEDLOOP,0);

			registControled(newSwipeNote);
			pbTouchLayer::registerObserver(newSwipeNote);
			pbRenderProcess::RegistRenderNote(newSwipeNote);
		}
		LOGW("PRODUCT SWIPE : Create");
}

void pbNoteProcessor::AddDoubTapNote(float X, float Y, float fSecondDistanceX, float fSecondDistanceY) {
	pbDoubleTapNote* newDoubleTapNote = m_pDoubleTapNoteRentalUnit->RentalMemory();
	if( newDoubleTapNote == NULL) { LOGE("PRODUCT DTAP : Error"); return; }
	newDoubleTapNote->ValueInitializer();
	newDoubleTapNote->setTypes(NOTETYPE::DTAP);
//	newDoubleTapNote->m_ID = pDriedNote->ID;
	newDoubleTapNote->setVertexIndices(0, 1);
	newDoubleTapNote->setUVIndices( 9, 19);

	float fTouchWidth = pbDataStorage::GetVertexWidth(0);
	float fTouchHeight = pbDataStorage::GetVertexHeight(0);
	newDoubleTapNote->setTouchWidthAndHeight(fTouchWidth,fTouchHeight);

	newDoubleTapNote->setNotePosition(X, Y);
	newDoubleTapNote->ExtraInitialize();
	newDoubleTapNote->SetSecondNotePosDelta(fSecondDistanceX, fSecondDistanceY);		//�����ǳ�Ʈ �ɼ�

	newDoubleTapNote->randOnSwitch();

	if(newDoubleTapNote->getTargetOnSwitch()){
		nitroFrame::npTimer::registerObserver(newDoubleTapNote,nitroFrame::npTimer::CHANGEDLOOP,0);
	}


	registControled(newDoubleTapNote);
	pbTouchLayer::registerObserver(newDoubleTapNote);
	pbRenderProcess::RegistRenderNote(newDoubleTapNote);

	LOGW("PRODUCT DTAP : Create");
}

void pbNoteProcessor::AddLongNote(float X, float Y, float fSecondDistanceX, float fSecondDistanceY) {
	pbLongPressNote* newLongPressNote = m_pLongNoteRentalUnit->RentalMemory();
	if( newLongPressNote == NULL)  { LOGE("PRODUCT LPRESS : Error"); return; }
	newLongPressNote->ValueInitializer();
	newLongPressNote->setTypes(NOTETYPE::LPRESS);
//	newLongPressNote->m_ID = pDriedNote->ID;
	newLongPressNote->setVertexIndices(2);
	newLongPressNote->setUVIndices( 17, 19, 18);

	float fTouchWidth = pbDataStorage::GetVertexWidth(2);
	float fTouchHeight = pbDataStorage::GetVertexHeight(2);
	newLongPressNote->setTouchWidthAndHeight(fTouchWidth,fTouchHeight);

	newLongPressNote->setNotePosition(X, Y);
	newLongPressNote->ExtraInitialize();
	newLongPressNote->SetDestination(fSecondDistanceX, fSecondDistanceY);		//�����ǳ�Ʈ �ɼ�

	newLongPressNote->randOnSwitch();

	if(newLongPressNote->getTargetOnSwitch()){
		nitroFrame::npTimer::registerObserver(newLongPressNote,nitroFrame::npTimer::CHANGEDLOOP,0);
	}

	registControled(newLongPressNote);
	pbTouchLayer::registerObserver(newLongPressNote);
	pbRenderProcess::RegistRenderNote(newLongPressNote);

	LOGW("PRODUCT LPRESS : Create");
}


void pbNoteProcessor::CreateNote(pbDriedNote* pDriedNote, float PosX, float PosY)
{
	switch(pDriedNote->NoteType)		{
			case NOTETYPE::TARGETING:{
				AddTargetingNote(PosX, PosY);
			}
			break;
			case NOTETYPE::SWIPE:
			{	//-----------------------------����������Ʈ --------------------------------------//
				pbDriedNote_Chain* pDriedNote_Chain = (pbDriedNote_Chain*)pDriedNote;
				AddSwipeNote(PosX, PosY, pDriedNote_Chain->fChainVectorX, pDriedNote_Chain->fChainVectorY, pDriedNote_Chain->numChain);
			}
			break;
			case NOTETYPE::DTAP:
			{	//-----------------------------�����ǳ�Ʈ --------------------------------------//
				pbDriedNote_DoublePos* pDriedNoteDPos = (pbDriedNote_DoublePos*)pDriedNote;
				AddDoubTapNote(PosX, PosY, pDriedNoteDPos->fSecondDistanceX, pDriedNoteDPos->fSecondDistanceY);
			}
			break;
			case NOTETYPE::LPRESS:
			{	//-----------------------------�ճ�Ʈ --------------------------------------//
				pbDriedNote_DoublePos* pDriedNoteDPos = (pbDriedNote_DoublePos*)pDriedNote;
				AddLongNote(PosX, PosY, 	pDriedNoteDPos->fSecondDistanceX, pDriedNoteDPos->fSecondDistanceY);
			}
			break;
		}//end switch
}


void pbNoteProcessor::CheckMissNote(){
	pbLinkNode<pbNote>* pFirstNoteNode = SingleObject->m_ControledNoteStore->getNext();
	pbNote* pFirstNote = pFirstNoteNode->getKernel();
	if (pFirstNote != NULL) {

		if( pFirstNote->GetPosX()  < pbCharacter::GetInstance()->GetPosX() ) {//���ɻ� ����

			 if( pFirstNote->getTargetOnSwitch() ) {
				 if( !pFirstNote->GetEndPhase()) {
					 pFirstNote->JudgeMissNote();
					pbCharacter::GetInstance()->DecreaseLife();
					pbComboManager::GetInstance()->ResetCombo();
				 }
			 }
			 else {
				pbTouchLayer::removeObserver(pFirstNote);
				nitroFrame::npTimer::findRemove(pFirstNote);

					//��� ���� �ڵ�
				pFirstNote->SetEndPhase(true);
				pFirstNote->SetNoteState(NoteState::END);
				pFirstNote->SetNoteJudge(NOTEJUDGEMENT::MISS);
			 }

/*
			float X = pFirstNote->GetPosX() - pbCharacter::GetInstance()->GetPosX();
			float Y = pFirstNote->GetPosY() - pbCharacter::GetInstance()->GetPosY();
			if(	 sqrt( (X*X) + (Y*Y) )  < 100.0f  ){
			}
*/

		}
	}
}

void pbNoteProcessor::Update(float time){
	pbLinkNode<pbNote>* iterator;
	pbLinkNode<pbNote>* head = m_ControledNoteStore;

	CheckMissNote();

	if( m_NoteDropper != NULL)
		m_NoteDropper->NoteDropCheck();

	iterator = head->getNext();
	while (iterator != head) {
		pbNote* pNote = iterator->getKernel();
		iterator = iterator->getNext();

		pNote->Update(time);
	}


}

void pbNoteProcessor::ClearDataStore(){
	if( m_ControledNoteStore != NULL) {
		 pbLinkNode<pbNote>::ClearList(m_ControledNoteStore);
	}

	 if( m_NoteDropper != NULL) {
		 delete m_NoteDropper;
		 m_NoteDropper = NULL;
	 }

	 LOGI("NoteProcessor ClearDataStrore");
}
/*NoteProcess�� ������ �����ϱ� ���� �Լ�. ���α׷��� ����Ǵ� �ñ⿡ ȣ��Ǹ� �ȴ�.
 * �޸� Heap�� �÷��� ��� ��ü���� �����ϹǷ� �߸��� ȣ��� ������ �߻��� �� �ִ�.
 */
void pbNoteProcessor::Release(){
	if( SingleObject != NULL) {
		SingleObject->ClearDataStore();

		if (SingleObject->m_ControledNoteStore != NULL) {
			delete SingleObject->m_ControledNoteStore;
			SingleObject->m_ControledNoteStore = NULL;
		}

		if (SingleObject->m_pSwipeNoteRentalUnit != NULL) {
			SingleObject->m_pSwipeNoteRentalUnit->Release();
			delete SingleObject->m_pSwipeNoteRentalUnit;
			SingleObject->m_pSwipeNoteRentalUnit = NULL;
		}

		if (SingleObject->m_pDoubleTapNoteRentalUnit != NULL) {
			SingleObject->m_pDoubleTapNoteRentalUnit->Release();
			delete SingleObject->m_pDoubleTapNoteRentalUnit;
			SingleObject->m_pDoubleTapNoteRentalUnit = NULL;
		}

		if (SingleObject->m_pLongNoteRentalUnit != NULL) {
			SingleObject->m_pLongNoteRentalUnit->Release();
			delete SingleObject->m_pLongNoteRentalUnit;
			SingleObject->m_pLongNoteRentalUnit = NULL;
		}

		if (SingleObject->m_pTargetNoteRentalUnit != NULL) {
			SingleObject->m_pTargetNoteRentalUnit->Release();
			delete SingleObject->m_pTargetNoteRentalUnit;
			SingleObject->m_pTargetNoteRentalUnit = NULL;
		}

		delete SingleObject;
		LOGI("pbNoteProcess Release");

		SingleObject = NULL;
	}
}

/*
 *
 */
void pbNoteProcessor::registControled(pbNote* pNote){
	 pbLinkNode<pbNote>* pTargetNode =  pbLinkNode<pbNote>::makeLinkNode(pNote);
	 pbLinkNode<pbNote>::addTail(pTargetNode,SingleObject->m_ControledNoteStore);
}

/* ��Ʈ�� ��Ȳ���� �����ϴ� �޼��� pTargetNode�� Controled ����Ʈ���� ���ŵǰ� NodeStore����Ʈ�� ���Եȴ�.
 * �̿� ���� NodeStore����Ʈ���� �������� �� ����� ID�� ������ �ĺ��Ͽ� �ٽ� �ҷ����� �ְ� �ȴ�.
 * �̴� ������ Retry�ÿ� Loading�� ������ �ּ�ȭ �ϰ� ������ �ٽ� �÷��̸� �ϰ� ����� ���� �����̴�.
 *
 * @pTargetNode: Controled ����Ʈ���� ���ŵǾ��� ��Ʈ�� ��ũ����
 */
//[FIXLOG:8-29]������ : ��ũ���� �� ���ó���� �޸𸮸� ���� �Ҵ��ϹǷ� ã�Ƽ� ������ �Ѵ�
void pbNoteProcessor::removeControled(pbNote* pNote){
	 pbLinkNode<pbNote>* pTargetNode =  pbLinkNode<pbNote>::makeLinkNode(pNote);
	 pbLinkNode<pbNote>::findDelete(pTargetNode,SingleObject->m_ControledNoteStore);

//	pbNoteLinkNode::addTail(pTargetNode,NoteProcesser->m_NoteStore);
}

// ����Ʈ���� �����ϰ� ��Ż�޸𸮸� ��ȯ�Ѵ�
void pbNoteProcessor::RemoveNoteAndReturningMemory(pbTargetingNote* pNote) {
	if( pNote != NULL) {
		removeControled(pNote);
		m_pTargetNoteRentalUnit->ReturningRentalMemory(pNote);
	}
}

void pbNoteProcessor::RemoveNoteAndReturningMemory(pbSwipeNote* pNote) {
	if( pNote != NULL) {
		removeControled(pNote);
		m_pSwipeNoteRentalUnit->ReturningRentalMemory(pNote);
	}
}

void pbNoteProcessor::RemoveNoteAndReturningMemory(pbDoubleTapNote* pNote) {
	if( pNote != NULL) {
		removeControled(pNote);
		m_pDoubleTapNoteRentalUnit->ReturningRentalMemory(pNote);
	}
}

void pbNoteProcessor::RemoveNoteAndReturningMemory(pbLongPressNote* pNote) {
	if( pNote != NULL) {
		removeControled(pNote);
		m_pLongNoteRentalUnit->ReturningRentalMemory(pNote);
	}
}

}
