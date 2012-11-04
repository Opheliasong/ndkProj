/*
 * npEngineFrames.cpp
 *
 *  Created on: 2012. 8. 7.
 *      Author: NitroPigSoft02
 */

#include "npEngineFrames.h"

namespace nitroFrame{
void npEngineFrame::npGameCreate(JNIEnv* env,int width, int height){
	//기본적으로 해상도값을 가지고 있는다
	m_Width = width;
	m_Height = height;

	//Env를 넣어둔다
	m_env = env;

	npContainerDAO::SetupTheAPKPath(apkPath);
}

void npEngineFrame::npGameDestroy(){

}

void npEngineFrame::npMainMenuState(){

}

void npEngineFrame::npGameLoop(){
	switch(m_GameStates){
		case GAMECREATE:{
			npShowIntro();
			break;
		}
		case GAMEMAINMENU:{
			npMainMenuState();
			break;
		}
		case GAMEUPDATE:{
			npGameUpdate();
			break;
		}
		case GAMEPAUSE:{
			npGamePaused();
			break;
		}
		case GAMEDESTROY:{
			npGameDestroy();
			break;
		}
	}
}

void npEngineFrame::npGameUpdate(){
	clock_t beginTime = clock();
	m_timeDelta= ((double)(beginTime - m_beforeTime))/CLOCKS_PER_SEC;
	m_FPS = 1/m_timeDelta;

}

void npEngineFrame::npGamePaused(){

}

void npEngineFrame::npGameDisplay(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void npEngineFrame::npGameChanged(int width, int hegith){

}

double npEngineFrame::npUpdateTimeDelta(){
	clock_t beginTime = clock();
	m_timeDelta= ((double)(beginTime - m_beforeTime))/CLOCKS_PER_SEC;
	return m_timeDelta;

}

double npEngineFrame::npCheckTimeDelta(){
	clock_t beginTime = clock();
	return	((double)(beginTime - m_beforeTime))/CLOCKS_PER_SEC;

}

npEngineFrame::npEngineFrame(const char* apkPath):m_env(0),m_FPS(0),m_Height(0),m_Width(0),m_beforeTime(0),m_timeDelta(0),m_GameStates(GAMECREATE), m_bFirstLoad(true){
	this->apkPath = const_cast<char*>(apkPath);
}


}


