/*
 * pbSceneNavigator.cpp
 *
 *  Created on: 2012. 12. 5.
 *      Author: NitroSoft
 */

#include "pbSceneNavigator.h"

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
