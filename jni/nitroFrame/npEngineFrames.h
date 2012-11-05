/*
 * npEngineFrames.h
 *
 *  Created on: 2012. 8. 7.
 *      Author: 박성민
 */

#ifndef NPENGINEFRAMES_H_
#define NPENGINEFRAMES_H_

#include <GLES/gl.h>
#include <GLES/glext.h>
#include <string>
#include <time.h>
#include <math.h>
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "NitroFrame.h"
#include "npNitroFrameMacro.h"
#include "Render/npContainerDAO.h"

/***
 * @author 박성민
 * @brief 게임 프레임웍(엔진)의 뼈대를 구축하는 abstract 클래스
 * @details 전체 어플리케이션의 흐름을 관리하게 될 클래스. 어플리케이션에서 사용되는 JNIenv와 같은 객체들을 소유하고 있으며
 * Logical한 부분만 관리한다.
 * @warning npGameDisplay()함수를 Define해서 사용해야 한다
 * 게임에서 사용되는 상태나 기본 변수(해상도,시간 정보 등)을 가지고 있게 된다.
 *
 * #member Variable
 * (i)Width = 현재 디바이스의 Width값
 * (i)Height = 현재 디바이스의 height 값
 * (clock_t)beginTime = update 함수가 호출되기 전의 시각 값
 * (f)FPS = 게임의 전체 FPS
 * (d)timeDelta = 이전 업데이트와 현재 업데이트간의 시간차이를 가지는 변수
 *
 * @npGameCreate : Process가 생성되었을 경우 호출되는 함수
 * @npGameDestroy : Process가 종료되었을 경우 호출되는 함수
 * @npGameLoop: 게임의 기본적인 LoopLogic
 * @npGameUpdate : 게임이 계속 진행상태인 경우 호출되는 함수
 * @npGamePaused : 게임의 루프로직이 일시 정지시에 호출되는 함수
 * @npGameChanged : 게임의 상태가 변경되었을 경우
 * @npShowIntro : 게임 프로세스가 안정화되고 Intro 화면을 보여주는 Logic
 * @npMainMenuState : 게임프로세스의 상태가 메인메뉴화면으로 넘어간 상황 일시
 *
 * ==Utiltiy 군==
 * @npUpdateTimeDelta : beginTime을 새로 갱신한다.
 * @npCheckTimeDelta : beginTime에서부터 지금까지의 시간을 측정하여 값을 리턴한다.
 * @setOrth : 직교투영방식을 사용할 경우 호출한다.
 * @setFrustum : 원근투영방식을 사용할 경우 호출한다.
 *
 */
namespace nitroFrame{

class npEngineFrame {

public:
	/***
	 * #GAMECREATE : 게임 프로세스가 프로세스가 OS의 레디큐(Ready Queue)에 올라간 즉시를 말한다.
	 *   이때에는 intro 및 각종 프로세스 초기 작업을 수행한다.
	 * #GAMEMAINMENU : 게임의 타이틀과 메인 메뉴의 분기점 UI가 주로 펼쳐지며
	    * 유저가 In-game 으로 들어갈것인지. 설정화면으로 갈것인지 등을 설정한다.
	 * #GAMEUPDATE : In-Game 에서의 update 로직을 말한다
	 * #GAMEPAUSE : In-Game에서 Pause된 상황을 말한다. 주로 메뉴화면을 출력할 경우를 말한다.
	 * #GAMEDESTROY : 게임 프로세스가 유저의 Interrupt Signal에 의해서 종료되는 상태를 말한다.
	 */
	typedef enum {GAMECREATE = 0,GAMEMAINMENU,GAMEUPDATE,GAMEPAUSE,GAMEDESTROY}GAMESTATE;

protected:

	int m_Width;
	int m_Height;

	double m_InnerWidthRatio;
	double m_InnerHeightRatio;

	clock_t m_beforeTime;
	double m_timeDelta;
	float m_FPS;

	NP_DEFINE_PROPERTY(JNIEnv*, m_env, Env)		//env 객체
	NP_DEFINE_PROPERTY(jobject, m_AssetManager, AssetManager) //android AssetManager
	NP_DEFINE_PROPERTY(AAsset*, m_Asset, Assets)	//Android Asset 객체
	char* apkPath;

	float m_bFirstLoad; // 추가 : 재로딩 & 메모리 누수 방지

public:
	GAMESTATE m_GameStates;

	npEngineFrame(const char* apkPath);
	protected:	npEngineFrame():m_env(0),m_FPS(0),m_Height(0),m_Width(0),m_beforeTime(0),m_timeDelta(0),m_GameStates(GAMECREATE), m_bFirstLoad(true),m_AssetManager(NULL),m_Asset(NULL){}
	virtual ~npEngineFrame(){};

	//기본 Frame 함수들
	virtual void npGameCreate(JNIEnv* env,int width, int height,const char* apkPath);

	virtual void npShowIntro()=0;
	virtual void npMainMenuState();
	virtual void npGameLoop();
	virtual void npGameDestroy();
	virtual void npGameUpdate();
	virtual void npGamePaused();

	virtual void npGameChanged(int width, int hegith);
	virtual void npGameDisplay();

	//Utility 함수
	double npUpdateTimeDelta();
	double npCheckTimeDelta();

/*
	int GetWidth() { return m_Width;}
	int GetHeight() { return m_Height;}

	JNIEnv* GetEnv() {return m_env;}
	jobject GetAssetManager() { return m_AssetManager;}
*/
protected:
	static npEngineFrame* Instance;
};
}

#endif /* NPENGINEFRAMES_H_ */

