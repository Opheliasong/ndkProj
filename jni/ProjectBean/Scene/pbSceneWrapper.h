#ifndef _PBSCENEWRAPPER_H_
#define _PBSCENEWRAPPER_H_

#include "../stdafx.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////----------------------------------------------------- pbSceneWrapper------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef npLinkNode<npDrawable*> RenderList;
class pbStageTrigger;
class pbSceneWrapper {
public:
	pbSceneWrapper();
	virtual ~pbSceneWrapper();

	void DrawScene();

	inline void SetTag(sceneTag Tag) { m_Tag = Tag;}
	inline sceneTag& GetTag() { return m_Tag;}

	void RegistToRenderList(npDrawable* pDrawable);
	void RemoveToRenderList(npDrawable* pDrawable);
	void ClearToRenderList();

	void ResetFadeValues();
	void StartFadeIn(float fFadeTime);
	void StartFadeOut(float fFadeTime);
	void FadeUpdate(float fTime);

	pbStageTrigger* GetStageTrigger() {return m_StageTrigger;}
	bool IsFadeInStart() { return m_FadeControl < 0;}
	bool IsFadeInEnd() { return (m_FadeValue == 0.0f && m_FadeControl == 0.0f );}
	bool IsFadeOutStart() { return m_FadeControl > 0;}
	bool IsFadeOutEnd() { return (m_FadeValue == 1.0f && m_FadeControl == 0.0f );}


	virtual void InitializeScene() = 0;
	virtual void UpdateScene(float fTime) = 0;
	virtual void ClearScene() = 0;
private:
	RenderList* m_RenderListHeader;
	sceneTag m_Tag;
	pbStageTrigger* m_StageTrigger;

	static GLfloat m_FadeVertex[12];
	float m_FadeControl;
	float m_FadeValue;
	bool m_FadeOn;
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

	static void MenuTouch();
private:
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////----------------------------------------------------pbScoreSceneWrapper------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class pbResultViewer;
class pbScoreSceneWrapper : public pbSceneWrapper {
	pbScoreSceneWrapper();
public:
	pbScoreSceneWrapper(const char* SceneTag);
	~pbScoreSceneWrapper();

	virtual void InitializeScene();
	virtual void UpdateScene(float fTime);
	virtual void ClearScene();
private:
	pbResultViewer* m_ResultViewer;
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

	void RemoveRenderToCurrentScene(npDrawable* pDrawable);
	void RemoveRenderToScene(sceneTag Tag, npDrawable* pDrawable);
private:
	typedef std::map<sceneTag, pbSceneWrapper*> pbSceneMap;

	pbSceneMap m_SceneStore;
	pbSceneWrapper* m_pCurrentScene;
};


#endif
