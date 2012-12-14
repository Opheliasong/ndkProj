#ifndef _PBSCENEWRAPPER_H_
#define _PBSCENEWRAPPER_H_

//#include "stdafx.h"
#include "pbUI.h"
#include "pbBackground.h"
#include "pbTypes.h"
#include "Notes/pbNoteProcessor.h"

//class pbNoteProcessor;
typedef std::string sceneTag;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-------------------------------------------------------씬에 대한 상태 기계------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------pbStateElement ------------------------------------------------------------------------------///////
//pbStateElement
/*class pbSceneStateElement {
public:
	pbSceneStateElement(){m_iAction = SCENESTATE::ACTION_NONE; m_iOutGameState = SCENESTATE::GAME_NONE; m_iOutLoadState = SCENESTATE::LOAD_NONE; }
	~pbSceneStateElement(){}

public:
	SCENESTATE::ACTION m_iAction;
	SCENESTATE::GAME m_iOutGameState;
	SCENESTATE::LOAD m_iOutLoadState;
};*/
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////----------------------------------------------------- pbSceneWrapper------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef npLinkNode<npDrawable*> RenderList;

class pbSceneWrapper {
public:
	pbSceneWrapper() { m_RenderListHeader =new RenderList; m_RenderListHeader->setHeader(); }
	virtual ~pbSceneWrapper() { RenderList::destroyList(m_RenderListHeader); }

	void DrawScene() {
		RenderList* iterator;
		RenderList* head = m_RenderListHeader;
		iterator = head->getNext();
		while (iterator != head) {
			npDrawable* pkernel = iterator->getKernel();
			iterator = iterator->getNext();

			npRenderprocess::getInstance().DoDraw((*pkernel));

		}
	//	LOGE("DEBUG pbSceneWrapper : DrawScene complete");
	}

	void SetTag(sceneTag Tag) { m_Tag = Tag;}
	sceneTag& GetTag() { return m_Tag;}
	void RegistToRenderList(npDrawable* pDrawable) {
		if(pDrawable != NULL ) {
			RenderList::addTail(pDrawable, 	m_RenderListHeader);
			LOGI("pbSceneWrapper::RegistRenderList");
		}
		else
			LOGE("pbSceneWrapper::RegistRenderList Drawble is NULL");
	}

	void ClearToRenderList() {
			RenderList::clearList(m_RenderListHeader);
			LOGI("pbSceneWrapper::ClearToRenderList");
		}

	virtual void InitializeScene() = 0;
	virtual void UpdateScene(float fTime) = 0;
	virtual void ClearScene() = 0;
private:
	RenderList* m_RenderListHeader;
	sceneTag m_Tag;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////----------------------------------------------------- pbPlaySceneWrapper------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class pbTouchUI;
class pbPlaySceneWrapper : public pbSceneWrapper{
	pbPlaySceneWrapper();
public:
	pbPlaySceneWrapper(const char* SceneTag);
	~pbPlaySceneWrapper();

	virtual void InitializeScene();
	virtual void UpdateScene(float fTime);
	virtual void ClearScene();
private:
	pbTouchUI* BackWardUI;
	pbTouchUI* helpUI;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////----------------------------------------------------pbIntroSceneWrapper------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class pbTouchableBackground;
class pbIntroSceneWrapper : public pbSceneWrapper {
	pbIntroSceneWrapper();
public:
	pbIntroSceneWrapper(const char* SceneTag);
	~pbIntroSceneWrapper();

	virtual void InitializeScene();
	virtual void UpdateScene(float fTime);
	virtual void ClearScene();
private:
	pbTouchableBackground* m_IntroBG;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////----------------------------------------------------- pbSceneManager------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class pbSceneManager {
	pbSceneManager();
	~pbSceneManager();
public:
	static pbSceneManager& getInstance();

	void AddScene(sceneTag Tag, pbSceneWrapper* pScene);

	void SelectScene(sceneTag SceneTag);

	inline pbSceneWrapper* GetCurrentScene() {return m_pCurrentScene;}

	void Draw();
	void Update(float fTime);

	void RegistRenderToCurrentScene(npDrawable* pDrawable);
	void RegistRenderToScene(sceneTag Tag, npDrawable* pDrawable);
private:
	typedef std::map<sceneTag, pbSceneWrapper*> pbSceneMap;

	pbSceneMap m_SceneStore;
	pbSceneWrapper* m_pCurrentScene;
};

/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////----------------------------------------------------- pbLoadingSceneWrapper------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class pbLoadingSceneWrapper {
private:
	pbLoadingSceneWrapper();
	~pbLoadingSceneWrapper();

public:
	static void Create();

	void InitializeScene();
	void UpdateScene(float fTime);
	void ClearScene();

	void SetPercentage(float fPercent);
	float GetPercentage() { return m_fLoadingPercent;}

	static void Release();

	static  pbLoadingSceneWrapper* GetInstance() { return SingleObject;}
private:
	static pbLoadingSceneWrapper* SingleObject;

	bool m_bFirstLoading;
	float m_fLoadingPercent;
	pbInterfaceUI* m_pUI_LoadingPercent;
	pbBackground* m_pBackground;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------�ε� �Ŵ��� ------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class pbIntroSceneWrapper {
	class TouchBox : public npObserver {
	public:
		TouchBox();
		~TouchBox();

		void SetArea(float Width, float Height);
		void MoveArea(float X, float Y);
		void RegistTouchLayer();
		void RemoveTouchLayer();

		bool IsTouched() { return m_bTouched;}
		void ResetTouch() { m_bTouched = false; }

		virtual void notify(){};
		virtual void notify(int x, int y, TOUCHSTATUS::TYPE Touchstatus);
		virtual void notify(TOUCHSTATUS::TYPE Touchstatus){}
		virtual void wakeUpNotify(){}
		virtual float resetWakeTime(){return 0.0f;}
	private:
		pbTouchArea* m_pTouchArea;
		bool m_bTouched;
	};
private:
	pbIntroSceneWrapper();
	~pbIntroSceneWrapper();

public:
	static void Create();

	void InitializeScene();
	void UpdateScene(float fTime);
	void ClearScene();

	static void Release();

	static  pbIntroSceneWrapper* GetIncetance() { return SingleObject;}
private:
	static pbIntroSceneWrapper* SingleObject;

	pbBackground* m_pBackground;
	TouchBox* m_pTouchBox;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------�÷��� �Ŵ��� ------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class pbPlaySceneWrapper {
private:
	pbPlaySceneWrapper();
	~pbPlaySceneWrapper();

public:
	static void Create();

	void InitializeScene();
	void UpdateScene(float fTime);
	void ClearScene();

	static void Release();

	static  pbPlaySceneWrapper* GetIncetance() { return SingleObject;}

//	typedef enum {PLAYMODE_LAND = 0, PLAYMODE_SKY } PLAYMODE;
private:
	static pbPlaySceneWrapper* SingleObject;

//	PLAYMODE m_PlayMode;
};*/

#endif
