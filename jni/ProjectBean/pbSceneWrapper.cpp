
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

void pbSceneMover::AddStateElement(SCENESTATE::ACTION iAction, SCENESTATE::GAME iOutGameState, SCENESTATE::LOAD iOutLoadState) {
	pbSceneStateElement* newState = new pbSceneStateElement();
	newState->m_iAction = iAction;
	newState->m_iOutGameState = iOutGameState;
	newState->m_iOutLoadState = iOutLoadState;

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
pbSceneNavigator* pbSceneNavigator::SingleObject = NULL;

pbSceneNavigator::pbSceneNavigator() {
	m_pSceneMoverHeader = NULL;
	m_iCurrentGameState =SCENESTATE::GAME_NONE;
	m_iCurrentLoadState = SCENESTATE::LOAD_NONE;
	m_iClearGameState = SCENESTATE::GAME_NONE;
}
pbSceneNavigator::~pbSceneNavigator() {

}
void pbSceneNavigator::Create() {
	if( SingleObject == NULL) {
		SingleObject = new pbSceneNavigator();
		SingleObject->m_pSceneMoverHeader = new SceneMoverList;
		SingleObject->m_pSceneMoverHeader->setHeader();

		LOGI("pbSceneNavigator::Create() Complete");
		return;
	}

	LOGE("pbSceneNavigator::Create() Failed");
}

void pbSceneNavigator::LoadSceneState() {
	m_iCurrentGameState = SCENESTATE::GAME_CREATE;
	m_iCurrentLoadState = SCENESTATE::LOAD_NONE;
	m_iClearGameState = SCENESTATE::GAME_NONE;

	pbSceneMover* newMover = new pbSceneMover(SCENESTATE::GAME_CREATE);
	//newMover->AddStateElement(SCENESTATE::ACTION_FOWARD, SCENESTATE::GAME_INTRO, SCENESTATE::LOAD_INTRO);
	newMover->AddStateElement(SCENESTATE::ACTION_FOWARD, SCENESTATE::GAME_PLAY, SCENESTATE::LOAD_PLAY);
	AddSceneMover(newMover);
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
bool pbSceneNavigator::MoveScene(SCENESTATE::ACTION Action) {

	//Debug Log
	if( Action == SCENESTATE::ACTION_FOWARD)
		LOGE("ACTION_FOWARD");
	else if(Action ==SCENESTATE::ACTION_BACKWARD)
		LOGE("ACTION_BACKWARD");
	else if(Action ==SCENESTATE::ACTION_SELECT_1)
		LOGE("ACTION_SELECT_1");
	else if(Action ==SCENESTATE::ACTION_SELECT_2)
		LOGE("ACTION_SELECT_2");
	else if(Action ==SCENESTATE::ACTION_SELECT_3)
		LOGE("ACTION_SELECT_3");
	else if(Action ==SCENESTATE::ACTION_NONE)
		LOGE("ACTION_NONE");

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
					m_iClearGameState = m_iCurrentGameState;
					m_iCurrentGameState = pElement->m_iOutGameState;
					m_iCurrentLoadState = pElement->m_iOutLoadState;

					//------------------------------------����׿�------------------------------//
					if( m_iClearGameState == SCENESTATE::GAME_CREATE)
						LOGE("GAME_CREATE -> ");
					else if(m_iClearGameState ==SCENESTATE::GAME_DESTROY)
						LOGE("GAME_DESTROY -> ");
					else if(m_iClearGameState ==SCENESTATE::GAME_INTRO)
						LOGE("GAME_INTRO -> ");
					else if(m_iClearGameState ==SCENESTATE::GAME_MAINMENU)
						LOGE("GAME_MAINMENU -> ");
					else if(m_iClearGameState ==SCENESTATE::GAME_PAUSE)
						LOGE("GAME_PAUSE -> ");
					else if(m_iClearGameState ==SCENESTATE::GAME_PLAY)
						LOGE("GAME_PLAY -> ");
					else if(m_iClearGameState ==SCENESTATE::GAME_NONE)
						LOGE("GAME_NONE -> ");

					if( m_iCurrentGameState == SCENESTATE::GAME_CREATE)
						LOGE("GAME_CREATE");
					else if(m_iCurrentGameState ==SCENESTATE::GAME_DESTROY)
						LOGE("GAME_DESTROY");
					else if(m_iCurrentGameState ==SCENESTATE::GAME_INTRO)
						LOGE("GAME_INTRO");
					else if(m_iCurrentGameState ==SCENESTATE::GAME_MAINMENU)
						LOGE("GAME_MAINMENU");
					else if(m_iCurrentGameState ==SCENESTATE::GAME_PAUSE)
						LOGE("GAME_PAUSE");
					else if(m_iCurrentGameState ==SCENESTATE::GAME_PLAY)
						LOGE("GAME_PLAY");
					else if(m_iCurrentGameState ==SCENESTATE::GAME_NONE)
						LOGE("GAME_NONE");
					//-----------------------------------------------------------------------------//

					return true;
				}//end if NULL check
			}// end if State Check
		}//end while

	}// end if ListHeader Check

	return false;
}

void pbSceneNavigator::ClearSceneState() {
	if( m_pSceneMoverHeader != NULL) {
		LinkNodeDeleteAllKernel(pbSceneMover*, m_pSceneMoverHeader);
		SceneMoverList::clearList(m_pSceneMoverHeader);
	}
}

void pbSceneNavigator::Release() {
	if( SingleObject != NULL) {
		if( SingleObject->m_pSceneMoverHeader != NULL) {
			SingleObject->ClearSceneState();
			SceneMoverList::destroyList(SingleObject->m_pSceneMoverHeader);
			SingleObject->m_pSceneMoverHeader = NULL;
		}

		SingleObject = NULL;

		LOGI("pbSceneNavigator::Release() Complete");

		return ;
	}
	LOGE("pbSceneNavigator::Release() Failed");
}

////-----------------------------------------------------------------------------------------------------------------------------------------------------------///////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------pbPlaySceneWrapper------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pbPlaySceneWrapper::pbPlaySceneWrapper() {
	SetTag("PLAYSCENE");
}
pbPlaySceneWrapper::~pbPlaySceneWrapper() {

}

void pbPlaySceneWrapper::InitializeScene() {
/*	pbUIProcessor::GetInstance()->LoadData("UI_Placement.xml");
	pbComboManager::GetInstance()->LoadData();
	pbCharacter::GetInstance()->LoadData();
	pbGuideLineGenerator::GetInstance()->LoadGuideLine("GuideLine.xml");
	pbBoss::GetInstance()->LoadData();
	pbNoteProcessor::GetInstance()->LoadData("NoteData.xml");
*/
	//background
	pbBackground* pCreateBG = pbBackgroundProcessor::GetInstance().AddScrollBackGround(800, 480, 400, 240, 0.1f, "ci");
	pbSceneManager::getInstance().RegistRenderToScene(GetTag(), pCreateBG);

	pCreateBG = pbBackgroundProcessor::GetInstance().AddMoveBackGround(400, 240, 0, 220, 0.5f, "ci");
	pbSceneManager::getInstance().RegistRenderToScene(GetTag(), pCreateBG);

	pCreateBG = pbBackgroundProcessor::GetInstance().AddScrollBackGround(800, 100, 400, 50, 1.0f, "ci");
	pbSceneManager::getInstance().RegistRenderToScene(GetTag(), pCreateBG);


	LOGI("pbPlaySceneWrapper InitializeScene Complete");
}

void pbPlaySceneWrapper::UpdateScene(float fTime) {
	if(!pbGlobalInGameVariable::bGamePause)
	{
		pbGlobalInGameVariable::fWorldMoveX = pbGlobalInGameVariable::fWorldMoveDir*pbGlobalInGameVariable::fWorldMoveSpeed*fTime;
		pbGlobalInGameVariable::fWorldX += pbGlobalInGameVariable::fWorldMoveX;

		float mesc = 1000.f * fTime;
		pbBackgroundProcessor::GetInstance().Update(fTime);
/*		nitroFrame::npTimer::updateTime(mesc);
		pbNoteProcessor::GetInstance()->Update(fTime);
		pbUIProcessor::GetInstance()->Update(fTime);
		pbEffectProcess::GetInstance()->Update(fTime);
		pbGuideLineGenerator::GetInstance()->Update(fTime);
		pbCharacter::GetInstance()->Update(fTime);
		pbBoss::GetInstance()->Update(fTime);*/
	}
}

void pbPlaySceneWrapper::ClearScene() {
	pbBackgroundProcessor::GetInstance().ClearDataStore();
/*	pbNoteProcessor::GetInstance()->ClearDataStore();
	pbComboManager::GetInstance()->ClearDataStore();
	pbEffectProcess::GetInstance()->ClearDataStore();
	pbGuideLineGenerator::GetInstance()->ClearDataStore();
	pbCharacter::GetInstance()->ClearDataStore();
	pbBoss::GetInstance()->ClearDataStore();
	pbUIProcessor::GetInstance()->ClearDataStore();
	pbGlobalInGameVariable::ResetGlobalVariable();
	pbRenderProcess::ClearDataStore();

	pbTouchLayer::ClearDataStore();*/
	LOGI("pbPlaySceneWrapper ClearScene Complete");
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

	//TODO :: 임시 코드. 테스트를 위해 바로 셀렉트 한다
	delete m_pCurrentScene;

}

pbSceneManager& pbSceneManager::getInstance(){
	static pbSceneManager singletone;
	return singletone;
}

void pbSceneManager::AddScene(sceneTag Tag, pbSceneWrapper* pScene) {
	m_SceneStore.insert(Tag, pScene);
	//TODO :: 임시 코드. 테스트를 위해 바로 셀렉트 한다
	m_pCurrentScene = pScene;
}

void pbSceneManager::SelectScene(sceneTag SceneTag) {
	//TODO :: 씬을 고른다
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


