/*
 * pbSceneNavigator.h
 *
 *  Created on: 2012. 12. 5.
 *      Author: NitroSoft
 */

#ifndef PBSCENENAVIGATOR_H_
#define PBSCENENAVIGATOR_H_

#include "../stdafx.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-------------------------------------------------------씬에 대한 상태 기계------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------pbStateElement ------------------------------------------------------------------------------///////
class pbSceneStateElement {
public:
	pbSceneStateElement(){m_iAction = SCENESTATE::ACTION_NONE; m_iNextGameState =  SCENESTATE::GAME_NONE; }
	~pbSceneStateElement(){}

public:
	SCENESTATE::ACTION m_iAction;
	SCENESTATE::GAME m_iNextGameState;
	sceneTag m_Tag;
};

typedef npLinkNode<pbSceneStateElement*> SceneStateElementList;
////-----------------------------------------------------pbSceneMover ------------------------------------------------------------------------------///////
// SceneMover
class pbSceneMover {
public:
	pbSceneMover(){m_iState =  SCENESTATE::GAME_NONE; m_pStateHeader = NULL; }
	pbSceneMover(SCENESTATE::GAME iState);
	~pbSceneMover();

	void AddStateElement(SCENESTATE::ACTION iAction, SCENESTATE::GAME iNextGameState, sceneTag NextSceneTag);
	void DeleteStateElement(SCENESTATE::ACTION iAction);

	pbSceneStateElement* FindOutStateByAction(SCENESTATE::ACTION iAction);

	 SCENESTATE::GAME GetState() { return m_iState; }
private:
	SCENESTATE::GAME m_iState;
	SceneStateElementList* m_pStateHeader;
};

////-----------------------------------------------------pbSceneNavigator ------------------------------------------------------------------------------///////
class pbSceneNavigator {
public:
	pbSceneNavigator();
	~pbSceneNavigator();

	void LoadSceneState();

	void AddSceneMover(pbSceneMover* pMover);

	void SearchAndReadyToMoveScene(SCENESTATE::ACTION Action);

	void ClearSceneState();

	inline  SCENESTATE::GAME GetCurrentGameState() { return m_iCurrentGameState;}

	inline bool IsReadyToNextScene() { return m_bReadyToNextScene; }

	void MoveScene();

	inline void SetCurrentState(SCENESTATE::GAME iCurrentGameState ) { m_iCurrentGameState  = iCurrentGameState;}

	static pbSceneNavigator& GetInstance();
private:
	typedef npLinkNode<pbSceneMover*> SceneMoverList;
	SceneMoverList* m_pSceneMoverHeader;
	SCENESTATE::GAME m_iCurrentGameState;
	SCENESTATE::GAME m_iNextGameState;

	bool m_bReadyToNextScene;
	sceneTag m_NextSceneTag;
};


#endif /* PBSCENENAVIGATOR_H_ */
