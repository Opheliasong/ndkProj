
#include "pbSceneWrapper.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------�� �̵� �� (�ӽ÷� Wrapper�� ���� ��Ų��, ���� ���� ���� ����)---------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////-----------------------------------------------------pbSceneMover ------------------------------------------------------------------------------///////
pbSceneMover::pbSceneMover( SCENESTATE::GAME iState) {
	m_iState = iState;
	m_pStateHeader = new SceneStateElementList();
	m_pStateHeader->setHeader();
}
pbSceneMover::~pbSceneMover() {
	if( m_pStateHeader != NULL ) {
		SceneStateElementList* iterator = m_pStateHeader;

		iterator = m_pStateHeader->getNext();
		while (iterator != m_pStateHeader) {
			pbSceneStateElement* pkernel = iterator->getKernel();
			iterator = iterator->getNext();

			delete pkernel;
		}


		SceneStateElementList::destroyList(m_pStateHeader);

		LOGI("pbSceneMover Destroy Complete");
		return ;
	}

	LOGE("pbSceneMover Destructor : m_pStateHeader is NULL");
}

void pbSceneMover::AddStateElement(SCENESTATE::ACTION iAction, SCENESTATE::GAME iNextGameState, sceneTag NextSceneTag) {
	pbSceneStateElement* newState = new pbSceneStateElement();
	newState->m_iAction = iAction;
	newState->m_iNextGameState = iNextGameState;
	newState->m_Tag = NextSceneTag;

	SceneStateElementList* pTargetNode =  SceneStateElementList::makeLinkNode(newState);
	SceneStateElementList::addTail(pTargetNode, m_pStateHeader);

}
void pbSceneMover::DeleteStateElement(SCENESTATE::ACTION iAction) {
	SceneStateElementList* iterator;
	SceneStateElementList* head = m_pStateHeader;

	iterator = head->getNext();
	while (iterator != head) {
		pbSceneStateElement* pkernel = iterator->getKernel();
		iterator = iterator->getNext();

		if( pkernel->m_iAction == iAction ) {
			SceneStateElementList::destroyNode(iterator);
			delete pkernel;

			break;
		}

	}
}

pbSceneStateElement* pbSceneMover::FindOutStateByAction(SCENESTATE::ACTION iAction) {
	SceneStateElementList* iterator;
	SceneStateElementList* head = m_pStateHeader;

	iterator = head->getNext();
	while (iterator != head) {
		pbSceneStateElement* pkernel = iterator->getKernel();
		iterator = iterator->getNext();

		if( pkernel->m_iAction == iAction ) {
			return pkernel;
		}
	}

	return NULL;
}

////-----------------------------------------------------pbSceneNavigator ------------------------------------------------------------------------------///////
pbSceneNavigator& pbSceneNavigator::GetInstance() {
	static pbSceneNavigator Singleton;

	return Singleton;
}

pbSceneNavigator::pbSceneNavigator() {
	m_pSceneMoverHeader = new SceneMoverList;
	m_pSceneMoverHeader->setHeader();
	m_iCurrentGameState = SCENESTATE::GAME_NONE;
	m_iNextGameState = SCENESTATE::GAME_NONE;
	m_bReadyToNextScene = false;
//	m_iCurrentLoadState = SCENESTATE::LOAD_NONE;
//	m_iClearGameState = SCENESTATE::GAME_NONE;

	LOGI("pbSceneNavigator::Constructor() Complete");
}
pbSceneNavigator::~pbSceneNavigator() {
	ClearSceneState();
	SceneMoverList::destroyList(m_pSceneMoverHeader);
	m_pSceneMoverHeader = NULL;

	LOGI("pbSceneNavigator::Destructor() Complete");
}

void pbSceneNavigator::LoadSceneState() {
	m_iCurrentGameState = SCENESTATE::GAME_CREATE;
// m_iCurrentLoadState = SCENESTATE::LOAD_NONE;
//	m_iClearGameState = SCENESTATE::GAME_NONE;

/*	pbSceneMover* newMover = new pbSceneMover(SCENESTATE::GAME_CREATE);
	//newMover->AddStateElement(SCENESTATE::ACTION_FOWARD, SCENESTATE::GAME_INTRO, SCENESTATE::LOAD_INTRO);
	newMover->AddStateElement(SCENESTATE::ACTION_FOWARD, SCENESTATE::GAME_PLAY, SCENESTATE::LOAD_PLAY);
	AddSceneMover(newMover);*/
//	newMover = new pbSceneMover(SCENESTATE::GAME_INTRO);
//	newMover->AddStateElement(SCENESTATE::ACTION_FOWARD, SCENESTATE::GAME_PLAY, SCENESTATE::LOAD_PLAY);
//	newMover->AddStateElement(SCENESTATE::ACTION_BACKWARD, SCENESTATE::GAME_DESTROY, SCENESTATE::LOAD_NONE);
//	AddSceneMover(newMover);

//	newMover = new pbSceneMover(SCENESTATE::GAME_PLAY);
//	newMover->AddStateElement(SCENESTATE::ACTION_BACKWARD, SCENESTATE::GAME_INTRO, SCENESTATE::LOAD_INTRO);
//	AddSceneMover(newMover);


	LOGI("pbSceneNavigator::LoadSceneState() Complete ");
}

void pbSceneNavigator::AddSceneMover(pbSceneMover* pMover) {
	SceneMoverList::addTail(pMover, m_pSceneMoverHeader);
}

//액션에 해당하는 씬이 없으면 false 반환
void pbSceneNavigator::SearchAndReadyToMoveScene(SCENESTATE::ACTION Action) {

	if(m_pSceneMoverHeader != NULL) {
		SceneMoverList* iterator;
		SceneMoverList* head = m_pSceneMoverHeader;

		iterator = head->getNext();
		while (iterator != head) {
			pbSceneMover* pkernel = iterator->getKernel();
			iterator = iterator->getNext();

			if( pkernel->GetState() == m_iCurrentGameState) {
				pbSceneStateElement* pElement = pkernel->FindOutStateByAction(Action);
				if(  pElement != NULL) {
					m_iNextGameState = pElement->m_iNextGameState;
					m_bReadyToNextScene = true;
					m_NextSceneTag = pElement->m_Tag;
					LOGE("[DEBUG]pbSceneNavigator::SearchAndReadyToMoveScene() success");
					return;
				}//end if NULL check
			}// end if State Check
		}//end while

	}// end if ListHeader Check

	return;
}

void pbSceneNavigator::MoveScene() {
	//현재 씬 클리어
	if( pbSceneManager::getInstance().GetCurrentScene() != NULL ) {
		pbSceneManager::getInstance().GetCurrentScene()->ClearScene();
	}

	//다음씬 교체후 초기화
	pbSceneManager::getInstance().SelectScene(m_NextSceneTag);
	pbSceneManager::getInstance().GetCurrentScene()->InitializeScene();

	//네비게이터 변수 초기화
	m_iCurrentGameState = m_iNextGameState;
	m_bReadyToNextScene = false;

	std::string DebugString("[DEBUG]pbSceneNavigator::MoveScene : ");
	DebugString.append(m_NextSceneTag);
	LOGE(DebugString.c_str() );

	//LOGE("[DEBUG]pbSceneNavigator::MoveScene() Complete" );
}

void pbSceneNavigator::ClearSceneState() {
	if( m_pSceneMoverHeader != NULL) {
		LinkNodeDeleteAllKernel(pbSceneMover*, m_pSceneMoverHeader);
		SceneMoverList::clearList(m_pSceneMoverHeader);
	}
}

////-----------------------------------------------------------------------------------------------------------------------------------------------------------///////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------pbSceneWrapper  Base Class------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GLfloat pbSceneWrapper::m_FadeVertex[12] = {0,};

pbSceneWrapper::pbSceneWrapper() {
	m_RenderListHeader =new RenderList;
	m_RenderListHeader->setHeader();

	m_StageTrigger = new pbStageTrigger();

	SetVertexByCenter(m_FadeVertex, 800, 480);
	m_FadeValue = 0.0f;
	m_FadeControl = 0.0f;
	m_FadeOn = false;
}

pbSceneWrapper::~pbSceneWrapper() {
	RenderList::destroyList(m_RenderListHeader);
	delete m_StageTrigger;
}

void pbSceneWrapper::DrawScene() {
	RenderList* iterator;
	RenderList* head = m_RenderListHeader;
	iterator = head->getNext();
	while (iterator != head) {
		npDrawable* pkernel = iterator->getKernel();
		iterator = iterator->getNext();

		npRenderprocess::getInstance().DoDraw((*pkernel));

	}

	if(m_FadeOn ) {
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glPushMatrix();
		glTranslatef(400, 240, 0);
		glColor4f(0.0f, 0.0f, 0.0f, m_FadeValue);
		glVertexPointer(3, GL_FLOAT, 0, m_FadeVertex);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glPopMatrix();
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}
//	LOGE("DEBUG pbSceneWrapper : DrawScene complete");
}

void pbSceneWrapper::RegistToRenderList(npDrawable* pDrawable) {
	if(pDrawable != NULL ) {
		RenderList::addTail(pDrawable, 	m_RenderListHeader);
		LOGI("pbSceneWrapper::RegistRenderList");
	}
	else
		LOGE("pbSceneWrapper::RegistRenderList Drawble is NULL");
}

void pbSceneWrapper::RemoveToRenderList(npDrawable* pDrawable) {
	if(pDrawable != NULL ) {
		RenderList::findDelete(pDrawable, 	m_RenderListHeader);
		LOGI("pbSceneWrapper::RemoveToRenderList");
	}
	else
		LOGE("pbSceneWrapper::RemoveToRenderList Drawble is NULL");
}

void pbSceneWrapper::ResetFadeValues() {
	m_FadeValue = 1.0f;
	m_FadeControl = 0.0f;
}

void pbSceneWrapper::StartFadeIn(float fFadeTime) {
	m_FadeOn = true;
	m_FadeValue = 1.0f;
	m_FadeControl = -(1/fFadeTime);

	LOGI(" pbSceneWrapper::StartFadeIn");
}
void pbSceneWrapper::StartFadeOut(float fFadeTime) {
	m_FadeOn = true;
	m_FadeValue = 0.0f;
	m_FadeControl = 1/fFadeTime;
	LOGI(" pbSceneWrapper::StartFadeOut");
}

void pbSceneWrapper::FadeUpdate(float fTime) {
	if( m_FadeControl != 0.0f) {
		m_FadeValue += m_FadeControl*fTime;

		if( m_FadeValue < 0.0f ) {
			m_FadeValue =  0.0f;
			m_FadeControl =  0.0f;
			m_FadeOn = false;
		}
		else 	if( m_FadeValue > 1.0f ) {
			m_FadeValue = 1.0;
			m_FadeControl =  0.0f;
		}
	}

}

void pbSceneWrapper::ClearToRenderList() {
	RenderList::clearList(m_RenderListHeader);
	LOGI("pbSceneWrapper::ClearToRenderList");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------pbPlaySceneWrapper------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pbPlaySceneWrapper::pbPlaySceneWrapper(const char* SceneTag) {
	SetTag(SceneTag);	//이 씬의 기본 태그를 지정
}

pbPlaySceneWrapper::~pbPlaySceneWrapper() {
}

void pbPlaySceneWrapper::InitializeScene() {
	StartFadeIn(0.5f);
	pbEffectManager::GetInstance()->SetSceneTag(GetTag());
	GetStageTrigger()->Initialize();
	//background
/*	pbBackground* pCreateBG = pbBackgroundProcessor::GetInstance().AddScrollBackGround(800, 480, 400, 240, 0.1f, "ci");
	RegistToRenderList(pCreateBG);

	pCreateBG = pbBackgroundProcessor::GetInstance().AddMoveBackGround(400, 240, 0, 220, 0.5f, "ci");
	RegistToRenderList(pCreateBG);

	pCreateBG = pbBackgroundProcessor::GetInstance().AddScrollBackGround(800, 100, 400, 50, 1.0f, "ci");
	RegistToRenderList(pCreateBG);*/


	//----------------UI------------------------------//
	pbUI* createUI = pbUIProcessor::GetInstance()->AddBackPanelUI(400, 450, "ci", 800, 50);
	this->RegistToRenderList(createUI);
	//라이프 UI 추가
	createUI = pbUIProcessor::GetInstance()->AddNumberUI(100, 450, "ci", 90,  30, "run", 25, 35, 1, &(pbStageValue::GetLifeData));
	this->RegistToRenderList(createUI);
	//스코어 UI 추가
	createUI = pbUIProcessor::GetInstance()->AddNumberUI(125 + 110, 450, "ci", 100, 30, "run", 25, 35, 9, &(pbStageValue::GetScoreData));
	this->RegistToRenderList(createUI);
	// 메뉴버튼
	createUI = pbUIProcessor::GetInstance()->AddButtonUI(760, 450, "ci", 70, 40, &(pbPlaySceneWrapper::MenuTouch));
	this->RegistToRenderList(createUI);
	// 맵 네비게이트
	pbStageValue::m_fMaxStageLength = 40000;
	createUI = pbUIProcessor::GetInstance()->AddGaugeUI(590, 450, "run", "ci", 250, 25, pbStageValue::m_fMaxStageLength, &(pbStageValue::GetStageX));
	this->RegistToRenderList(createUI);

/*
	pbGuideLineGenerator::GetInstance()->LoadGuideLine("GuideLine.xml");
	pbNoteProcessor::GetInstance()->LoadData("NoteData.xml");
*/
	pbComboManager::GetInstance()->LoadData();
	RegistToRenderList(pbComboManager::GetInstance());

	// 캐릭터
	pbCharacter::GetInstance()->LoadData(GetTag());
	pbCharacter::GetInstance()->SetPos(-200, 240);
	pbCharacter::GetInstance()->SetConditionPos(72.f, 240.f);
	pbCharacter::GetInstance()->SetTouchFunction(&(pbCharacter::PlayGame_TouchFunc));
	RegistToRenderList(pbCharacter::GetInstance());
	GetStageTrigger()->AddPosState(0, &(pbCharacter::Appeared));
	GetStageTrigger()->AddIDState(pbCharacter::WALKOUT, &(pbCharacter::WalkOut));

	// Fever게이지
	createUI = pbUIProcessor::GetInstance()->AddGaugeUI_RelativePos(pbCharacter::GetMarionette()->GetV2Pos(), 0, -((pbCharacter::GetInstance()->GetHeight()/2) + 15), "run", "ci", 150, 20, pbStageValue::MAX_FEVERGAUGE, &(pbStageValue::GetFeverGauge));
	this->RegistToRenderList(createUI);

	// 보스
	pbBoss::GetInstance()->LoadData();
	pbBoss::GetMarionette()->SetPosX(1200);pbBoss::GetMarionette()->SetPosY(240);
	pbBoss::GetInstance()->SetConditionPos(690.0f, 240.0f);
	RegistToRenderList(pbBoss::GetInstance());
	GetStageTrigger()->AddPosState(400, &(pbBoss::Approaching));
	GetStageTrigger()->AddIDState(pbBoss::DIE, &(pbBoss::PostDieProcess));

	LOGI("pbPlaySceneWrapper InitializeScene Complete");
}

void pbPlaySceneWrapper::UpdateScene(float fTime) {
	FadeUpdate(fTime);
	if(!GetStageTrigger()->IsPaused())
	{
		GetStageTrigger()->Update(fTime);

		pbBackgroundProcessor::GetInstance().Update(fTime);
		pbUIProcessor::GetInstance()->Update(fTime);
		pbCharacter::GetInstance()->Update(fTime);
		pbBoss::GetInstance()->Update(fTime);
		pbEffectManager::GetInstance()->Update(fTime);

/*		nitroFrame::npTimer::updateTime(mesc);
		pbNoteProcessor::GetInstance()->Update(fTime);
		pbGuideLineGenerator::GetInstance()->Update(fTime);
		*/

	}
}

void pbPlaySceneWrapper::MenuTouch() {
/*	pbSceneNavigator::GetInstance().SearchAndReadyToMoveScene(SCENESTATE::ACTION_BACKWARD);*/
	//pbEffectManager::GetInstance()->AddMissEffect();
	pbStageValue::IncreaseFeverGauge(100);
}

void pbPlaySceneWrapper::ClearScene() {
	ResetFadeValues();
	ClearToRenderList();
	TouchLayer::GetInstance().ClearRegistedList();
	pbBackgroundProcessor::GetInstance().ClearDataStore();
	pbUIProcessor::GetInstance()->ClearDataStore();
	pbCharacter::GetInstance()->ClearDataStore();
	pbBoss::GetInstance()->ClearDataStore();
	pbEffectManager::GetInstance()->ClearDataStore();
//	pbGlobalInGameVariable::ResetGlobalVariable();
	pbComboManager::GetInstance()->ClearDataStore();

/*	pbNoteProcessor::GetInstance()->ClearDataStore();

	pbGuideLineGenerator::GetInstance()->ClearDataStore();
	*/
	LOGI("pbPlaySceneWrapper ClearScene Complete");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------pbIntroSceneWrapper------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pbIntroSceneWrapper::pbIntroSceneWrapper(const char* SceneTag) {
	m_IntroBG = NULL;
	SetTag(SceneTag);	//이 씬의 기본 태그를 지정
}

pbIntroSceneWrapper::~pbIntroSceneWrapper() {

}

void pbIntroSceneWrapper::InitializeScene() {
	StartFadeIn(0.5f);
	m_IntroBG = pbBackgroundProcessor::GetInstance().AddTouchableBackGround(800, 480, 400, 240, "run");
	RegistToRenderList(m_IntroBG);

	LOGI("pbIntroSceneWrapper InitializeScene Complete");
}

void pbIntroSceneWrapper::UpdateScene(float fTime) {
	FadeUpdate(fTime);
	pbBackgroundProcessor::GetInstance().Update(fTime);

	if( m_IntroBG != NULL ) {
		if( m_IntroBG->IsTouched() ) {
			StartFadeOut(0.5f);
			m_IntroBG->ResetTouched();
		 }
	}

	if( IsFadeOutEnd()) {
		pbSceneNavigator::GetInstance().SearchAndReadyToMoveScene(SCENESTATE::ACTION_FOWARD);
		ResetFadeValues();
	}


}

void pbIntroSceneWrapper::ClearScene() {
	ClearToRenderList();
	TouchLayer::GetInstance().ClearRegistedList();
	pbBackgroundProcessor::GetInstance().ClearDataStore();
	m_IntroBG = NULL;
	LOGI("pbIntroSceneWrapper ClearScene Complete");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------pbScoreSceneWrapper------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pbScoreSceneWrapper::pbScoreSceneWrapper(const char* SceneTag) {
	SetTag(SceneTag);	//이 씬의 기본 태그를 지정
	m_ResultViewer = new pbResultViewer();
}

pbScoreSceneWrapper::~pbScoreSceneWrapper() {
	delete m_ResultViewer;
}

void pbScoreSceneWrapper::InitializeScene() {
	StartFadeIn(0.3f);
	GetStageTrigger()->Initialize();
	RegistToRenderList(m_ResultViewer);

	m_ResultViewer->SetPos(200, 0);
	m_ResultViewer->PushBackScoreView(0, 430, "ci", 150, 50, "run", 50, 50, pbStageValue::m_TotalScore);
	m_ResultViewer->PushBackScoreView(0, 330, "ci", 150, 50, "run", 50, 50, pbStageValue::m_iNumLife);

	// 캐릭터
	pbCharacter::GetInstance()->LoadData(GetTag());
	pbCharacter::GetInstance()->SetTouchFunction(&(pbCharacter::Result_TouchFunc));
	pbCharacter::GetInstance()->SetPos(600.0f, 100.0f);
	pbCharacter::GetInstance()->SetConditionPos(600.f, 100.f);
	pbCharacter::GetMarionette()->SelectMoveState(pbCharacter::WEAVING_UP);
	GetStageTrigger()->AddIDState(pbCharacter::WALKOUT, &(pbCharacter::WalkOut));
	RegistToRenderList(pbCharacter::GetInstance());

	LOGI("pbScoreSceneWrapper InitializeScene Complete");
}

void pbScoreSceneWrapper::UpdateScene(float fTime) {
	FadeUpdate(fTime);

	m_ResultViewer->Update(fTime);
	pbCharacter::GetInstance()->Update(fTime);
}

void pbScoreSceneWrapper::ClearScene() {
	ResetFadeValues();
	ClearToRenderList();
	pbCharacter::GetInstance()->ClearDataStore();
	m_ResultViewer->ClearDataStore();
	TouchLayer::GetInstance().ClearRegistedList();
	//pbBackgroundProcessor::GetInstance().ClearDataStore();
	LOGI("pbScoreSceneWrapper ClearScene Complete");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------pbSceneManager------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pbSceneManager::pbSceneManager() {
	m_pCurrentScene = NULL;

}

pbSceneManager::~pbSceneManager() {
	pbSceneMap::iterator Iter;
	for( Iter = m_SceneStore.begin(); Iter != m_SceneStore.end(); ++Iter) {
		pbSceneWrapper* pSecond = Iter->second;
		m_SceneStore.erase(Iter);
		delete pSecond;
	}
}

pbSceneManager& pbSceneManager::getInstance(){
	static pbSceneManager singletone;
	return singletone;
}

void pbSceneManager::AddScene(sceneTag Tag, pbSceneWrapper* pScene) {
	m_SceneStore.insert(std::pair<sceneTag, pbSceneWrapper*>(Tag, pScene));
}

void pbSceneManager::SelectScene(sceneTag SceneTag) {
	pbSceneMap::iterator Iter = m_SceneStore.find(SceneTag);
	if( Iter != m_SceneStore.end() ) {
		m_pCurrentScene = Iter->second;
		LOGE("[DEBUG]pbSceneManager::SelectScene success" );
	}
	else {
		m_pCurrentScene = NULL;
		LOGE("[DEBUG]pbSceneManager::SelectScene FAIL" );
	}

}

void pbSceneManager::Draw() {
	if( m_pCurrentScene != NULL)
		m_pCurrentScene->DrawScene();
}
void pbSceneManager::Update(float fTime) {
	if( m_pCurrentScene != NULL)
		m_pCurrentScene->UpdateScene(fTime);
}

void pbSceneManager::RegistRenderToCurrentScene(npDrawable* pDrawable) {
	if( m_pCurrentScene != NULL)
		m_pCurrentScene->RegistToRenderList(pDrawable);
}

void pbSceneManager::RegistRenderToScene(sceneTag Tag, npDrawable* pDrawable) {
	pbSceneMap::iterator Iter = m_SceneStore.find(Tag);
	if( Iter != m_SceneStore.end() ) {
		Iter->second->RegistToRenderList(pDrawable);
	}
}

void pbSceneManager::RemoveRenderToCurrentScene(npDrawable* pDrawable) {
	if( m_pCurrentScene != NULL)
		m_pCurrentScene->RemoveToRenderList(pDrawable);
}

void pbSceneManager::RemoveRenderToScene(sceneTag Tag, npDrawable* pDrawable) {
	pbSceneMap::iterator Iter = m_SceneStore.find(Tag);
	if( Iter != m_SceneStore.end() ) {
		Iter->second->RemoveToRenderList(pDrawable);
	}
}

/*pbLoadingSceneWrapper* pbLoadingSceneWrapper::SingleObject = NULL;

pbLoadingSceneWrapper::pbLoadingSceneWrapper() {
	m_pUI_LoadingPercent = NULL;
	m_pBackground = NULL;
	m_fLoadingPercent = 0.0f;
	m_bFirstLoading = true;
}
pbLoadingSceneWrapper::~pbLoadingSceneWrapper() {

}

void pbLoadingSceneWrapper::Create() {
	if( SingleObject == NULL) {
		SingleObject = new pbLoadingSceneWrapper;

		LOGI("pbLoadingManager Create Complete");
	}
}

void pbLoadingSceneWrapper::InitializeScene() {
	SingleObject->m_pBackground = pbBackgroundProcessor::GetInstance()->AddStaticBackGround(800, 480, 400, 240, 11);
//	SingleObject->m_pUI_LoadingPercent = pbUIProcessor::GetInstance()->AddLoadingPercentUI(730, 40, 0, 1, 0, 1);

	LOGI("pbLoadingManager InitializeScene Complete");
}

void pbLoadingSceneWrapper::UpdateScene(float fTime) {
	if( m_bFirstLoading ) {
		// �ε� �Ҷ� ���� �ؽ��� ��
		if( !pbDataStorage::DataStorageBackPacketCreate("LoadingTexture.xml") )
			LOGE("npGameLoading BackPacketCreate FAIL");
		pbDataStorage::DataStoragePacketSwapAndBackPacketDelete();

		// �ʱ�ȭ
		InitializeScene();

		LOGE("LOADING FirstLoading");
	}//end if LoadingPercent

	SetPercentage(m_fLoadingPercent);

	switch(pbSceneNavigator::GetInstance()->GetCurrentLoadState()){
		case SCENESTATE::LOAD_INTRO:{
			if( m_bFirstLoading) {

				if( !pbDataStorage::DataStorageBackPacketCreate("IntroTexture.xml") )
					LOGE("npGameLoading BackPacketCreate FAIL");

				m_bFirstLoading = false;
			}
			else if( m_fLoadingPercent < 3.0f) {
				m_fLoadingPercent += fTime;
			}
			else if( m_fLoadingPercent >= 3.0f) {
				pbDataStorage::DataStoragePacketSwapAndBackPacketDelete();

				ClearScene();

				pbIntroSceneWrapper::GetIncetance()->InitializeScene();

				pbSceneNavigator::GetInstance()->ResetCurrentLoadState();

				LOGE("LOADING Intro Complete");
			}//end if LoadingPercent
			break;
		}
		case SCENESTATE::LOAD_MAINMENU:{
			LOGE("MAIN LOADING");
			m_fLoadingPercent += (float)m_timeDelta;
			 if( m_fLoadingPercent >= 3.0f) {
				//�׽�Ʈ�� �ٷ� �÷��̷� �ѱ��
				m_LoadingStates = GAME_PLAY;
				m_GameStates = GAME_LOADING;
				LOGE("MAIN LOADING 2");
				pbLoadingSceneManager::DeleteScene();

				m_fLoadingPercent = 0.0f;
			 }
		}
		break;
		case SCENESTATE::LOAD_PLAY:{
//					LOGE("PLAY LOADING");
			if( m_bFirstLoading) {
				if( !pbDataStorage::DataStorageBackPacketCreate("PlayTexture.xml") )
					LOGE("npGameLoading BackPacketCreate FAIL");

				m_bFirstLoading = false;
			}
			else if( m_fLoadingPercent < 3.0f) {
				m_fLoadingPercent += fTime;
			}
			else if( m_fLoadingPercent >= 3.0f) {
				pbDataStorage::DataStoragePacketSwapAndBackPacketDelete();
				ClearScene();
				pbPlaySceneWrapper::GetIncetance()->InitializeScene();
				pbSceneNavigator::GetInstance()->ResetCurrentLoadState();
				LOGE("LOADING Play Complete");
			}//end if LoadingPercent

			break;
		}
	}//end switch
}

void pbLoadingSceneWrapper::SetPercentage(float fPercent) {
	if( m_pUI_LoadingPercent != NULL) {
		m_fLoadingPercent = fPercent;
//		((pbLoadingPercent_UI*)m_pUI_LoadingPercent)->SetLoadingPercentage(m_fLoadingPercent);

	}
}

void pbLoadingSceneWrapper::ClearScene() {
	if( SingleObject->m_pBackground != NULL) {
		pbBackgroundProcessor::GetInstance()->DeleteBackground(SingleObject->m_pBackground);
		pbRenderProcess::RemoveRenderBackground(SingleObject->m_pBackground);
		SingleObject->m_pBackground = NULL;
	}
	if( SingleObject->m_pUI_LoadingPercent != NULL) {
		pbUIProcessor::GetInstance()->DeleteUI(SingleObject->m_pUI_LoadingPercent);
		pbRenderProcess::RemoveRenderUI(SingleObject->m_pUI_LoadingPercent);
		SingleObject->m_pUI_LoadingPercent = NULL;
	}

	m_bFirstLoading = true;
	m_fLoadingPercent = 0.0f;


	LOGI("pbLoadingManager DeleteScene Complete");
}

void pbLoadingSceneWrapper::Release() {
	if( SingleObject != NULL) {
		SingleObject->ClearScene();

		delete SingleObject;

		SingleObject = NULL;

		LOGI("pbLoadingManager Release Complete");
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------��Ʈ�� �Ŵ��� ------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////-----------------------------------------------------��Ʈ�� ���� ��ġ �ڽ� Ŭ����------------------------------------------------------------------------------///////
pbIntroSceneWrapper::TouchBox::TouchBox() {
	m_pTouchArea = NULL;
	m_bTouched = false;
	m_pTouchArea = new pbTouchArea;
}
pbIntroSceneWrapper::TouchBox::~TouchBox() {
	delete m_pTouchArea;
}

void pbIntroSceneWrapper::TouchBox::SetArea(float Width, float Height) {
	m_pTouchArea->setTouchArea(Width, Height);
}

void pbIntroSceneWrapper::TouchBox::MoveArea(float X, float Y) {
	m_pTouchArea->movePositon(X, Y);
}

void pbIntroSceneWrapper::TouchBox::RegistTouchLayer() {
	pbTouchLayer::registerObserver(this);
}
void pbIntroSceneWrapper::TouchBox::RemoveTouchLayer() {
	pbTouchLayer::removeObserver(this);
}

void pbIntroSceneWrapper::TouchBox::notify(int x, int y, TOUCHSTATUS::TYPE Touchstatus) {
	if( Touchstatus == TOUCHSTATUS::TAPDOWN) {

		if( m_bTouched == false)
			if( m_pTouchArea->InRect(x, y) ) {
				m_bTouched = true;
			}
	}
}

////-----------------------------------------------------��Ʈ�� �Ŵ���  ����------------------------------------------------------------------------------///////

pbIntroSceneWrapper* pbIntroSceneWrapper::SingleObject = NULL;

pbIntroSceneWrapper::pbIntroSceneWrapper() {
	m_pBackground = NULL;
	m_pTouchBox = NULL;
}
pbIntroSceneWrapper::~pbIntroSceneWrapper() {

}

void pbIntroSceneWrapper::Create() {
	if( SingleObject == NULL) {
		SingleObject = new pbIntroSceneWrapper;

		SingleObject->m_pTouchBox = new TouchBox;
		LOGI("pbIntroSceneManager Create Complete");
	}
}

void pbIntroSceneWrapper::InitializeScene() {

	m_pBackground = pbBackgroundProcessor::GetInstance()->AddStaticBackGround(800, 480, 400, 240, 0);
	if( m_pBackground != NULL) {
		m_pTouchBox->SetArea(800, 480);
		m_pTouchBox->MoveArea(400, 240);
		m_pTouchBox->RegistTouchLayer();
		LOGI("pbIntroSceneManager InitializeScene Complete");

		return ;
	}

	LOGE("pbIntroSceneManager InitializeScene Failed");
}

void pbIntroSceneWrapper::UpdateScene(float fTime) {
	if( m_pBackground != NULL) {

		if(m_pTouchBox->IsTouched() ) {
			pbSceneNavigator::GetInstance()->MoveScene(SCENESTATE::ACTION_FOWARD);
//			ClearScene();
			m_pTouchBox->ResetTouch();
		}
	}
}

void pbIntroSceneWrapper::ClearScene() {
	if( m_pBackground != NULL) {
		pbRenderProcess::RemoveRenderBackground(m_pBackground);
		pbBackgroundProcessor::GetInstance()->DeleteBackground(m_pBackground);
		m_pTouchBox->RemoveTouchLayer();
		m_pBackground = NULL;

		LOGI("pbIntroSceneManager ClearScene Complete");
	}
}

void pbIntroSceneWrapper::Release() {
	if( SingleObject != NULL) {
		SingleObject->ClearScene();

		delete SingleObject->m_pTouchBox;
		delete SingleObject;

		SingleObject = NULL;

		LOGI("pbIntroSceneManager Release Complete");
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------�ε� �Ŵ��� ------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pbPlaySceneWrapper* pbPlaySceneWrapper::SingleObject = NULL;

pbPlaySceneWrapper::pbPlaySceneWrapper() {

}
pbPlaySceneWrapper::~pbPlaySceneWrapper() {

}

void pbPlaySceneWrapper::Create() {
	if( SingleObject == NULL) {
		SingleObject = new pbPlaySceneWrapper;

		LOGI("pbPlaySceneWrapper Create Complete");
	}
}

void pbPlaySceneWrapper::InitializeScene() {
	pbUIProcessor::GetInstance()->LoadData("UI_Placement.xml");
	pbComboManager::GetInstance()->LoadData();
	pbCharacter::GetInstance()->LoadData();
	pbGuideLineGenerator::GetInstance()->LoadGuideLine("GuideLine.xml");
	pbBoss::GetInstance()->LoadData();
	pbNoteProcessor::GetInstance()->LoadData("NoteData.xml");

	//���
	pbBackgroundProcessor::GetInstance()->AddScrollBackGround(800, 480, 400, 240, 0.1f, 23);
	pbBackgroundProcessor::GetInstance()->AddMoveBackGround(400, 240, 0, 220, 0.5f, 76);
	pbBackgroundProcessor::GetInstance()->AddScrollBackGround(800, 100, 400, 50, 1.0f, 77);


	LOGI("pbPlaySceneWrapper InitializeScene Complete");
}

void pbPlaySceneWrapper::UpdateScene(float fTime) {
	if(!pbGlobalInGameVariable::bGamePause)
	{
		pbGlobalInGameVariable::fWorldMoveX = pbGlobalInGameVariable::fWorldMoveDir*pbGlobalInGameVariable::fWorldMoveSpeed*fTime;
		pbGlobalInGameVariable::fWorldX += pbGlobalInGameVariable::fWorldMoveX;

		float mesc = 1000.f * fTime;
		nitroFrame::npTimer::updateTime(mesc);
		pbNoteProcessor::GetInstance()->Update(fTime);
		pbUIProcessor::GetInstance()->Update(fTime);
		pbEffectProcess::GetInstance()->Update(fTime);
		pbGuideLineGenerator::GetInstance()->Update(fTime);
		pbCharacter::GetInstance()->Update(fTime);
		pbBackgroundProcessor::GetInstance()->Update(fTime);
		pbBoss::GetInstance()->Update(fTime);
	}
}

void pbPlaySceneWrapper::ClearScene() {

	pbNoteProcessor::GetInstance()->ClearDataStore();
	pbComboManager::GetInstance()->ClearDataStore();
	pbEffectProcess::GetInstance()->ClearDataStore();
	pbGuideLineGenerator::GetInstance()->ClearDataStore();
	pbBackgroundProcessor::GetInstance()->ClearDataStore();
	pbCharacter::GetInstance()->ClearDataStore();
	pbBoss::GetInstance()->ClearDataStore();
	pbUIProcessor::GetInstance()->ClearDataStore();
	pbGlobalInGameVariable::ResetGlobalVariable();
	pbRenderProcess::ClearDataStore();


	pbTouchLayer::ClearDataStore();
	LOGI("pbPlaySceneWrapper ClearScene Complete");
}

void pbPlaySceneWrapper::Release() {
	if( SingleObject != NULL) {
		SingleObject->ClearScene();

		delete SingleObject;
		SingleObject = NULL;

		LOGI("pbPlaySceneWrapper Release Complete");
	}
}*/


