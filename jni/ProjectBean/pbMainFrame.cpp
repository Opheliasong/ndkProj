/*
 * pbMainFrame.cpp
 *
 *  Created on: 2012. 8. 8.
 *      Author: NitroPigSoft02
 */
#include "pbMainFrame.h"
#include "Notes/pbNoteProcessor.h"
namespace nitroFrame{
pbMainFrame* pbMainFrame::Instance = NULL;

pbMainFrame::pbMainFrame(){
	this->apkPath = NULL;
	m_bFirstLoad = false;
}

pbMainFrame::pbMainFrame(const char* apkPath) {
	this->apkPath = const_cast<char*>(apkPath);
	m_bFirstLoad = false;
}

pbMainFrame::~pbMainFrame() {
}

void pbMainFrame::npGameCreate(JNIEnv* env,const char* apkPath) {

	LOGE("Apk Path:%s",apkPath);
	this->m_env = env;

	this->apkPath = const_cast<char*>(apkPath);

	m_GameStates = GAMECREATE;
}

void pbMainFrame::npShowIntro() {

	if( !m_bFirstLoad  ) {
		int renderWidth = 800;
		int renderHeight = 480;

		glClearColor(0.1, 0.3, 0.3, 0.4f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearDepthf(1.0f);
		glEnable(GL_DEPTH_TEST);		//depth test �ㅽ뻾
		glDepthFunc(GL_LEQUAL);		//depth test

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		glLoadIdentity();
		//�뚮뜑�ъ쓽 �댁긽�꾨� �뺥븯��遺�텇
		//�꾩옱 湲곗� �뚮뜑�ъ쓽 �댁긽�꾨뒗
		//width: 800g
		//height: 480

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();	//Projection matrix珥덇린��
		glOrthof(-400.0f, 400.0f, -240.0f, +240.0f, 1.0f, -9.0f);		//
		//glOrthof(-240.f, 240.f, -400.f, 400.f, 0, -10);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//glViewport(0, 0, 800, 480);
		glViewport(0,0,npRenderprocess::getInstance().getDeviceWidth(),npRenderprocess::getInstance().getDeviceHeight());
		npRenderprocess::getInstance().setLookat(-400.0f, -240.0f,  0.0f,
																	-400.0f , -240.0f,  -1.0f,
																	0.0f,1.0f,0.0f);
		//XMLParsingBufferCreater::GetInstance().SetArchive(apkPath);
		pbUIProcessor::Create();
//		pbCharacter::Create();
/*
		pbEffectProcess::Create();
		pbGuideLineGenerator::Create();
		pbComboManager::Create();

		pbBoss::Create();

		pbNoteProcessor::Create();*/

		//FIXLOG[10-10] : 초기화는 공용 오브젝트만 초기화, 다른 초기화는 게임스테이트에 따라 다르게 초기화 한다
//		npContainerDAO::GetInstance().LoadTextureByXMLpath("ScriptTexture.xml");
		npContainerDAO::GetInstance().LoadTextureByXMLpath("ci.xml");
		npContainerDAO::GetInstance().LoadTextureByXMLpath("running.xml");
		npContainerDAO::GetInstance().LoadTextureByXMLpath("texture.xml");

		//노트 Process 관련 시작
		pbNoteProcessor::Create();
		LOGE("Craete Done Note Processor");

		//-------------------------------------------------씬 관련 초기화-------------------------------------------------------------//
		//---------씬 래퍼 세팅-----------------//
		pbPlaySceneWrapper* pPlayScene = new pbPlaySceneWrapper("PLAY_SCENE");
		pbSceneManager::getInstance().AddScene(pPlayScene->GetTag(), pPlayScene);

		pbIntroSceneWrapper* pIntroScene = new pbIntroSceneWrapper("INTRO_SCENE");
		pbSceneManager::getInstance().AddScene(pIntroScene->GetTag(), pIntroScene);

		//---------네비게이터 세팅-----------------//
		//pbSceneNavigator::GetInstance().LoadSceneState();
		pbSceneNavigator::GetInstance().SetCurrentState(SCENESTATE::GAME_CREATE);

		//---------인트로 네비게이트 무버
		pbSceneMover* newMover = new pbSceneMover(SCENESTATE::GAME_CREATE);
		//																										 인트로로 변경 해줘야함
		newMover->AddStateElement(SCENESTATE::ACTION_FOWARD, SCENESTATE::GAME_INTRO, pIntroScene->GetTag() );
		pbSceneNavigator::GetInstance().AddSceneMover(newMover);

		//---------플레이 네비게이트 무버
		newMover = new pbSceneMover(SCENESTATE::GAME_INTRO);
		newMover->AddStateElement(SCENESTATE::ACTION_FOWARD, SCENESTATE::GAME_PLAY, pPlayScene->GetTag() );
		pbSceneNavigator::GetInstance().AddSceneMover(newMover);

		pbSceneNavigator::GetInstance().SearchAndReadyToMoveScene(SCENESTATE::ACTION_FOWARD);
		//-/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////-//
		//---------되돌리기 테스트
			newMover = new pbSceneMover(SCENESTATE::GAME_PLAY);
			newMover->AddStateElement(SCENESTATE::ACTION_BACKWARD, SCENESTATE::GAME_INTRO, pIntroScene->GetTag() );
			pbSceneNavigator::GetInstance().AddSceneMover(newMover);
		//
	/*
		nitroFrame::npTimer::initialize();
*/

		m_bFirstLoad = true;
		LOGI("pbMainFrame::npShowIntro() Complete");

		m_GameStates = GAMEUPDATE;

		m_beforeTime = clock();
	}
}

void pbMainFrame::npMainMenuState() {
}

void pbMainFrame::npGameLoop() {
	//시간 관련 처리를 기본 EngineFrame 방식을 따른다.
	static int count = 0;
	clock_t beginTime = clock();
	m_timeDelta= ((double)(beginTime - m_beforeTime))/CLOCKS_PER_SEC;

//#define FPS_LOG
#ifdef FPS_LOG
	m_FPS += 1/m_timeDelta;
	count++;
	LOGfloatString("FPS:", m_FPS/(float)count);
#endif
	switch(m_GameStates){
		case GAMECREATE:{
			npShowIntro();
			break;
		}
		case GAMEUPDATE:{
			//nitroFrame::npTimer::getInstance().updateTime(m_timeDelta);
			npGameUpdate();
			break;
		}
	}

	m_beforeTime = beginTime;
}

void pbMainFrame::npGameDestroy() {
	pbUIProcessor::Release();
	pbNoteProcessor::Release();
//	pbCharacter::Release();
/*	pbNoteProcessor::Release();
	pbBoss::Release();

	pbGuideLineGenerator::Release();

	pbEffectProcess::Release();
	pbComboManager::Release();
*/

	delete Instance;

	LOGI(" pbMainFrame::npGameDestroy Complete");
}

void pbMainFrame::npGameUpdate() {

	if( pbSceneNavigator::GetInstance().IsReadyToNextScene()) {
		pbSceneNavigator::GetInstance().MoveScene();
	}

	pbSceneManager::getInstance().Update(m_timeDelta);
	//pbNoteProcessor::GetInstance()->Update(m_timeDelta);
}

void pbMainFrame::npGamePaused() {
}

void pbMainFrame::npGameChanged(int width, int hegith) {
}

void pbMainFrame::npGameDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_NOTEQUAL, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);		//2D


	glPushMatrix();
	pbSceneManager::getInstance().Draw();
	pbNoteProcessor::GetInstance()->RenderingAll();
	glPopMatrix();


	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);		//2D
}

pbMainFrame* pbMainFrame::GetInstance(){
	if(NP_IS_EMPTY(Instance)){
		Instance = new pbMainFrame;
	}
	return Instance;
}

pbMainFrame* pbMainFrame::GetInstance(const char* apkPath){
	if(NP_IS_EMPTY(Instance)){
		Instance = new pbMainFrame(apkPath);
	}
	return Instance;
}

void pbMainFrame::npOnBackPressed() {
	//pbSceneNavigator::GetInstance()->MoveScene(SCENESTATE::ACTION_BACKWARD);
	//LOGE("NP ONBACKPRESSED END");
}

}

