#include "pbGuideLine.h"
#include "pbMainFrame.h"

//------------------------------------------------------가이드라인 드로워------------------------------------------------------------//
pbGuideLineDrawer::pbGuideLineDrawer() {
	m_vPos[0] = 0; m_vPos[1] = 0;
	m_pV2FirstPos = NULL;
	m_pDriedLineDataListHeader = NULL;
	m_DriedLineDataList_Iterator = NULL;
	for( int i = 0 ; i < MAX_VERTEX; i++) {
		m_VertexBuffer[i*2] = 0;
		m_VertexBuffer[i*2+1] = 0;
	}

	m_AddIndex = 1;
}

pbGuideLineDrawer::~pbGuideLineDrawer() {

}

void pbGuideLineDrawer::SetFirstVertexBuffer(float* pV2Pos) {
	m_pV2FirstPos = pV2Pos;
}

void pbGuideLineDrawer::SetVertexBuffer(pbDriedGuideLineList_Iterator pStarIterator) {
	m_DriedLineDataList_Iterator = pStarIterator;

	m_AddIndex = 1;
	bool lastLine = false;
	while(m_DriedLineDataList_Iterator != NULL) {
		pbDriedGuideLineData* pDriedData = m_DriedLineDataList_Iterator->getKernel();
		if( pDriedData != NULL) {
			if( pDriedData->PosX > 2400 - m_vPos[0])
				lastLine = true;

			//버퍼 추가
			m_VertexBuffer[m_AddIndex*2] = pDriedData->PosX;
			m_VertexBuffer[m_AddIndex*2+1] = pDriedData->PosY;

			m_AddIndex++;

			if( lastLine == false ) {
				m_DriedLineDataList_Iterator = m_DriedLineDataList_Iterator->getNext();
//				LOGI("pbGuideLineDrawer::SetVertexBuffer Added Buffer");
			}
			else {
				m_DriedLineDataList_Iterator = NULL;
	//			LOGI("pbGuideLineDrawer::SetVertexBuffer Added Last Buffer");
				break;
			}
		}
		else
			break;
	}
}


void pbGuideLineDrawer::Draw() {
	if( m_pV2FirstPos != NULL) {
		m_VertexBuffer[0] = m_pV2FirstPos[0] - m_vPos[0];
		m_VertexBuffer[1] = m_pV2FirstPos[1];
	}
	else
		LOGE("pbGuideLineDrawer::Draw FirstPos Lost");


	glPushMatrix();

	glLineWidth(2.0f );
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glVertexPointer(2, GL_FLOAT, 0, m_VertexBuffer);
	glTranslatef(m_vPos[0] , m_vPos[1], 0.f);
	glDrawArrays(GL_LINE_STRIP, 0, m_AddIndex);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glPopMatrix();

}

//------------------------------------------------------가이드라인 로더------------------------------------------------------------//
pbGuideLineLoader::pbGuideLineLoader() {
	m_TotalGuideLineData = 0;
	m_pSecondLineIter = NULL;
	m_pDriedLineDataListHeader = new pbDriedGuideLineList;
	m_pDriedLineDataListHeader->setHeader();

	m_vPos_Point1[0] = 0;
	m_vPos_Point1[1] = 0;
	m_vPos_Point2[0] = 0;
	m_vPos_Point2[1] = 0;

	m_fMovedPosX = 0.0f;

	m_bUpdateList = false;
}

pbGuideLineLoader::~pbGuideLineLoader() {
	if( m_pDriedLineDataListHeader != NULL) {
		pbDriedGuideLineList::destroyListAndDeleteKernel(m_pDriedLineDataListHeader);
		m_pDriedLineDataListHeader = NULL;
		LOGI("pbGuideLineLoader::Destructor");
	}

}

void pbGuideLineLoader::registLinkNode(pbDriedGuideLineData* pLinedata){
	 pbDriedGuideLineList* pTargetNode =  pbDriedGuideLineList::makeLinkNode(pLinedata);
	 pbDriedGuideLineList::addTail(pTargetNode, m_pDriedLineDataListHeader);
}

void pbGuideLineLoader::removeLinkNode(pbDriedGuideLineData* pLinedata){
	 pbDriedGuideLineList* pTargetNode =  pbDriedGuideLineList::makeLinkNode(pLinedata);
	 pbDriedGuideLineList::findDelete(pTargetNode, m_pDriedLineDataListHeader);

}

void pbGuideLineLoader::Clear() {
	pbDriedGuideLineList::ClearListAndDeleteKernel(m_pDriedLineDataListHeader);
}

bool pbGuideLineLoader::GuideLineDataParsing(const char* fileName){
	const float fStartPos = 400.0f;

	//------------XML 파싱--------//
	char* buffer = pbProjectBeanFrame::npGetAssetBuffer(fileName);
	if( buffer == NULL) return false;

	TiXmlDocument xmlDoc;
	xmlDoc.Parse(buffer);

	if(xmlDoc.Error()){
		LOGE("XmlParser error");

		return false;
	}

	//----------------가이드라인데이터 헤더 파싱----------------------//
	if( xmlDoc.FirstChild("GuideLine")->FirstChild("GuideLineHeader") != NULL )
		LOGE("GuideLineHeader Element is not used ");

	TiXmlNode* LineDataNode = xmlDoc.FirstChild("GuideLine")->FirstChild("LineData");

	int count = 0;
	double dParseData = 0.0;
	while(LineDataNode != NULL ) {
		pbDriedGuideLineData* pDriedLineData = new pbDriedGuideLineData();
		LineDataNode->ToElement()->Attribute("X", &dParseData);
		pDriedLineData->PosX = dParseData + fStartPos;
		LineDataNode->ToElement()->Attribute("Y", &dParseData);
		pDriedLineData->PosY = dParseData;

		registLinkNode(pDriedLineData);

		LineDataNode = LineDataNode->NextSibling();
		count++;
	}

	m_TotalGuideLineData = count;
	LOGfloatString("GuideLine Xml LoadDataCount", m_TotalGuideLineData);
	LOGI("Done GuideLine Xml parsing");

	xmlDoc.Clear();
	pbProjectBeanFrame::npCloseAssetBuffer(buffer);

	return true;
}

void pbGuideLineLoader::SetFirstLineForLerpPoint() {
	if( !pbDriedGuideLineList::IsEmpty(m_pDriedLineDataListHeader) ) {
		pbDriedGuideLineData* pDriedLineData = m_pDriedLineDataListHeader->getNext()->getKernel();
//		if( pDriedLineData != NULL) {
			m_vPos_Point1[0] = pDriedLineData->PosX;
			m_vPos_Point1[1] = pDriedLineData->PosY;
//		}

		m_pSecondLineIter = m_pDriedLineDataListHeader->getNext()->getNext();
		pDriedLineData = m_pSecondLineIter->getKernel();
		if( pDriedLineData != NULL) {
			m_vPos_Point2[0] = pDriedLineData->PosX;
			m_vPos_Point2[1] = pDriedLineData->PosY;
		}

		LOGI("pbGuideLineLoader::SetFirstLineForLerpPoint");
	}
}

void pbGuideLineLoader::FirstNodeDelete() {
	if( !pbDriedGuideLineList::IsEmpty(m_pDriedLineDataListHeader) ) {
		pbDriedGuideLineData* pDriedLineData = m_pDriedLineDataListHeader->getNext()->getKernel();
		removeLinkNode(pDriedLineData);
		delete pDriedLineData;
		m_TotalGuideLineData--;

		LOGI("pbGuideLineLoader::FirstNodeDelete");
	}

}

void pbGuideLineLoader::SettingPosYFromFirstLineLerp(float* pV2Pos) {
	static float fLineX_1, fLineX_2, fLerp;
	fLineX_1 = m_vPos_Point2[0] - m_vPos_Point1[0];
	fLineX_2 = (pV2Pos[0] - m_fMovedPosX) - m_vPos_Point1[0];
	fLerp = fLineX_2/fLineX_1;

	if( fLerp >= 1.0f) {
		FirstNodeDelete();
		SetFirstLineForLerpPoint();
		fLerp = 0.0f;
		m_bUpdateList = true;
	}

	pV2Pos[1] = Lerp(m_vPos_Point1[1], m_vPos_Point2[1], fLerp);
/*	LOGfloatString("pbGuideLineLoader::fLineX_1", fLineX_1);
	LOGfloatString("pbGuideLineLoader::fLineX_2", fLineX_2);
	LOGfloatString("pbGuideLineLoader::fLerp", fLerp);
	LOGfloatString("pbGuideLineLoader::PosX", pV2Pos[0]);
	LOGfloatString("pbGuideLineLoader::PosY", pV2Pos[1]);*/
}

//------------------------------------------------------가이드라인 제너레이터------------------------------------------------------------//

pbGuideLineGenerator* pbGuideLineGenerator::m_pSingleObject = NULL;

pbGuideLineGenerator::pbGuideLineGenerator() {
	m_pGuideLineLoader = NULL;
	m_pGuideLineDrawer = NULL;
}
pbGuideLineGenerator::~pbGuideLineGenerator(){
	LOGI("Called ~pbGuideLineGenerator");

}

void pbGuideLineGenerator::Create(){
	//---------------------제너레이터 생성과 등록-----------------------//
	if( m_pSingleObject == NULL){
		m_pSingleObject = new pbGuideLineGenerator();

		m_pSingleObject->m_pGuideLineLoader =  new pbGuideLineLoader;

		m_pSingleObject->m_pGuideLineDrawer = new pbGuideLineDrawer;

		LOGI("pbGuideLineGenerator::Create Complete");
		return ;
	}

	LOGE("pbGuideLineGenerator::Create Failed");
}

void pbGuideLineGenerator::LoadGuideLine(const char* fileName) {
	if( m_pGuideLineLoader->GuideLineDataParsing(fileName) ) {
		pbRenderProcess::RegistRenderGuideLine(this);
		m_pGuideLineLoader->SetFirstLineForLerpPoint();
		m_pGuideLineLoader->SetUpdateList(true);

		m_pGuideLineDrawer->SetFirstVertexBuffer(pbCharacter::GetInstance()->GetPos());

		LOGI("pbGuideLineGenerator::LoadGuideLine Complete");
		return ;
	}
	LOGE("pbGuideLineGenerator::LoadGuideLine Failed");
}

void pbGuideLineGenerator::Draw(){
	if( m_pGuideLineDrawer != NULL) {
		m_pGuideLineDrawer->Draw();
	}

}

void pbGuideLineGenerator::Update(float fTime) {
	// 검사 & 지우기
	m_pGuideLineLoader->SettingPosYFromFirstLineLerp( pbCharacter::GetInstance()->GetPos() );
	m_pGuideLineLoader->SetMovedPosX(pbGlobalInGameVariable::fWorldX);

	//버텍스 세팅
	if( m_pGuideLineLoader->GetUpdateList() ) {
		m_pGuideLineDrawer->SetVertexBuffer( m_pGuideLineLoader->GetSecondLineIterator() );
		m_pGuideLineLoader->SetUpdateList(false);
	}

	//라인 드로워 이동
	m_pGuideLineDrawer->SetPosX(pbGlobalInGameVariable::fWorldX);
}


void pbGuideLineGenerator::ClearDataStore(){
	m_pGuideLineLoader->Clear();

	LOGI("pbGuideLineGenerator ClearDataStore");
}

void pbGuideLineGenerator::Release(){
	if( m_pSingleObject != NULL) {
		m_pSingleObject->ClearDataStore();

		if( m_pSingleObject->m_pGuideLineLoader != NULL) {
			delete m_pSingleObject->m_pGuideLineLoader;
			m_pSingleObject->m_pGuideLineLoader = NULL;
		}

		if( m_pSingleObject->m_pGuideLineDrawer != NULL ) {
			delete m_pSingleObject->m_pGuideLineDrawer;
			m_pSingleObject->m_pGuideLineDrawer = NULL;
		}

		delete m_pSingleObject;
		m_pSingleObject = NULL;

		LOGI("pbGuideLineGenerator:: Release");
	}


}

/*pbGuideLineGenerator* pbGuideLineGenerator::m_pSingleObject = NULL;

pbGuideLineGenerator::pbGuideLineGenerator() : m_pDriedLineData(NULL), m_TotalGuideLineData(0),m_CurrentDriedLineDataIndex(0),
		m_CurrentStoreIndex(0), m_bEndGenerating(false){

	m_pVertexDataRentalUnit = NULL;
	m_iFollowLineCheckCount = 0;

}
pbGuideLineGenerator::~pbGuideLineGenerator(){
	LOGI("Called ~pbGuideLineGenerator");

}

void pbGuideLineGenerator::FirstDataOnLoad(){
	//첫 FollowLine을 위해 두 점을 기본 로드
	AddNextVertexData();
	AddNextVertexData();

	m_pSingleObject->m_FollowLineVertexPointer[0] = &(m_pSingleObject->m_GuideLineVertexBuffer[0]);
	m_pSingleObject->m_FollowLineVertexPointer[1] = &(m_pSingleObject->m_GuideLineVertexBuffer[3]);

	m_iFollowLineCheckCount = 0;

	LOGI("First Data OnLoad Complete");
}

bool pbGuideLineGenerator::IsInScreen(float x, float y) {
	if (y <= 480.0f && y >= 0) {
		if (x >= 0 && x <= WIDTH+200.0f) {
			return true;
		}
	}
	return false;
}

void pbGuideLineGenerator::Create(){
	//---------------------제너레이터 생성과 등록-----------------------//
	if( m_pSingleObject == NULL){

		m_pSingleObject = new pbGuideLineGenerator();
		//---------------------버텍스 데이터 초기화-----------------------//
		for (int i = 0; i < MAX_VERTEXBUFFER; ++i) {
			m_pSingleObject->m_GuideLineVertexBuffer[i] = 0.0f;
		}

		m_pSingleObject->m_pVertexDataRentalUnit = new pbMemoryRentalUnit<pbGuideLineVertexData>;
		m_pSingleObject->m_pVertexDataRentalUnit->Initialize(MAX_VERTEX);
		LOGI("pbGuideLineGenerator::Create Complete");

		return ;
	}

	LOGE("pbGuideLineGenerator::Create Failed");
}

void pbGuideLineGenerator::LoadGuideLine(const char* fileName) {
	if( GuideLineDataParsing(fileName) ) {
		FirstDataOnLoad();
		pbRenderProcess::RegistRenderGuideLine(this);

		LOGI("pbGuideLineGenerator::LoadGuideLine Complete");
		return ;
	}
	LOGE("pbGuideLineGenerator::LoadGuideLine Failed");
}

bool pbGuideLineGenerator::GuideLineDataParsing(const char* fileName){
	const float fStartPos = 400.0f;

	//------------XML 파싱--------//
	char* buffer = pbProjectBeanFrame::npGetAssetBuffer(fileName);
	if( buffer == NULL) return false;

	TiXmlDocument xmlDoc;
	xmlDoc.Parse(buffer);

	if(xmlDoc.Error()){
		LOGE("XmlParser error");

		return false;
	}

	//----------------가이드라인데이터 헤더 파싱----------------------//
	TiXmlNode* DataHeadNode = xmlDoc.FirstChild("GuideLine")->FirstChild("GuideLineHeader");
	int count;
	DataHeadNode->ToElement()->Attribute("DataCount", &count);
	m_pSingleObject->m_TotalGuideLineData = count;

	if( m_pSingleObject->m_pDriedLineData != NULL){
		delete[] m_pSingleObject->m_pDriedLineData;
	}
	m_pSingleObject->m_pDriedLineData = new pbDriedGuideLineData[m_pSingleObject->m_TotalGuideLineData];

	TiXmlNode* LineDataNode = DataHeadNode->NextSibling("LineData");
	double dParseData;
	for(int i = 0; i < count; ++i) {
		LineDataNode->ToElement()->Attribute("X", &dParseData);
		m_pSingleObject->m_pDriedLineData[i].PosX = dParseData + fStartPos;
		LineDataNode->ToElement()->Attribute("Y", &dParseData);
		m_pSingleObject->m_pDriedLineData[i].PosY = dParseData;

		LineDataNode = LineDataNode->NextSibling();
	}

	LOGI("Done GuideLine Xml parsing");

	xmlDoc.Clear();
	pbProjectBeanFrame::npCloseAssetBuffer(buffer);

	return true;
}

void pbGuideLineGenerator::AddNextVertexData(){
	pbGuideLineVertexData* pVertexData = m_pSingleObject->m_pVertexDataRentalUnit->RentalMemory();

	if( pVertexData == NULL ) {
		LOGE("pbGuideLineGenerator::AddNextVertexData : NULL Pointer Error");
		m_bEndGenerating = true;
		return;
	}

	m_pVertexDataStore[m_CurrentStoreIndex] = pVertexData;

	 //---버퍼와 연결
	pVertexData->LinkedPosX = &(m_GuideLineVertexBuffer[m_CurrentStoreIndex*3]);
	pVertexData->LinkedPosY = &(m_GuideLineVertexBuffer[m_CurrentStoreIndex*3+1]);

	m_CurrentStoreIndex++;

	 //--DriedData 얹기
	//						 *메인 프레임 루프상 fWorldX가 이동한 후에 MoveUpdate로 또 이동하므로 중복이동을 막기위해 이동치를 상쇄한다*(보류 : 시작시 WordMove수치가 비정상)
	*(pVertexData->LinkedPosX) =  m_pDriedLineData[m_CurrentDriedLineDataIndex].PosX + pbGlobalInGameVariable::fWorldX ;
	*(pVertexData->LinkedPosY) =  m_pDriedLineData[m_CurrentDriedLineDataIndex].PosY;

	m_CurrentDriedLineDataIndex++;

//	LOGE("pbGuideLineGenerator::ADD Data");
}

// 계약사항 : 외부 작동은 가장 앞의 점을 없애는 것처럼 보이지만
// 				  실제로는 힙처럼 데이터를 앞으로 밀기 때문에 가장 마지막 데이터를 없앤다
//				  버퍼와 연결된 포인터가 꼬이는것을 막기 위해서이다
void pbGuideLineGenerator::DeleteFirstGuideLineVertex(){
	if( m_CurrentStoreIndex > 0)
	{
		 //---데이터 앞으로 당기기
		for(int i = 0; i < m_CurrentStoreIndex-1; i++)	{
			*(m_pVertexDataStore[i]->LinkedPosX) = *(m_pVertexDataStore[i+1]->LinkedPosX);
			*(m_pVertexDataStore[i]->LinkedPosY) = *(m_pVertexDataStore[i+1]->LinkedPosY);
//			LOGE("DATA MOVE");
		}

		//메모리 반환
		pbGuideLineVertexData* pVertexData = m_pVertexDataStore[m_CurrentStoreIndex-1];
		if( pVertexData == NULL ) {
			LOGE("pbGuideLineGenerator::DeleteFirstGuideLineVertex : NULL Pointer Error");
			m_bEndGenerating = true;
			return;
		}

		m_pVertexDataRentalUnit->ReturningRentalMemory(pVertexData);

		m_CurrentStoreIndex--;

///		LOGE("pbGuideLineGenerator :: Delete Data");
	}

}

void pbGuideLineGenerator::InScreenCheckAndAddData(){
	if( !m_bEndGenerating ){
		if( m_CurrentDriedLineDataIndex < m_TotalGuideLineData)
		{
			if( IsInScreen( m_pDriedLineData[m_CurrentDriedLineDataIndex-1].PosX + pbGlobalInGameVariable::fWorldX, m_pDriedLineData[m_CurrentDriedLineDataIndex-1].PosY)  )
			{
				AddNextVertexData();
			}
		}
	}
}

void pbGuideLineGenerator::FollowLineControl() {
	if( *(m_pVertexDataStore[m_iFollowLineCheckCount]->LinkedPosX) <= CHARACTER_POSITION ){
			m_FollowLineVertexPointer[0] = &(m_GuideLineVertexBuffer[m_iFollowLineCheckCount*3]);
			m_FollowLineVertexPointer[1] = &(m_GuideLineVertexBuffer[m_iFollowLineCheckCount*3 + 3]);

			m_iFollowLineCheckCount++;
	}

	if( *(m_pVertexDataStore[1]->LinkedPosX) <= 0.0f ){
		m_pSingleObject->DeleteFirstGuideLineVertex();

		m_iFollowLineCheckCount--;
		m_FollowLineVertexPointer[0] = &(m_GuideLineVertexBuffer[m_iFollowLineCheckCount*3-3]);
		m_FollowLineVertexPointer[1] = &(m_GuideLineVertexBuffer[m_iFollowLineCheckCount*3]);

	}
}

void pbGuideLineGenerator::MoveGuideLine(float fMoveX, float fMoveY){
	InScreenCheckAndAddData();

	//버텍스 이동
	for(int i = 0; i < m_CurrentStoreIndex; i++) {
		*(m_pVertexDataStore[i]->LinkedPosX) += fMoveX;
		*(m_pVertexDataStore[i]->LinkedPosY) += fMoveY;
	}

	FollowLineControl();
}


void pbGuideLineGenerator::Draw(){
	glPushMatrix();

	glLineWidth(2.0f );
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glVertexPointer(3, GL_FLOAT, 0, m_GuideLineVertexBuffer);

	glDrawArrays(GL_LINE_STRIP, 0, m_CurrentStoreIndex );

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glPopMatrix();
}


void pbGuideLineGenerator::ClearDataStore(){
	if( m_pDriedLineData != NULL) {
//		pbRenderProcess::RemoveRenderGuideLine();
		delete[] m_pDriedLineData;
		m_pDriedLineData = NULL;
	}

}

void pbGuideLineGenerator::Release(){
	if( m_pSingleObject != NULL) {
		m_pSingleObject->ClearDataStore();

		if( m_pSingleObject->m_pVertexDataRentalUnit != NULL) {
			m_pSingleObject->m_pVertexDataRentalUnit->Release();
			m_pSingleObject->m_pVertexDataRentalUnit = NULL;
		}


		delete m_pSingleObject;
		m_pSingleObject = NULL;

		LOGI("pbGuideLineGenerator:: Release");
	}


}*/

