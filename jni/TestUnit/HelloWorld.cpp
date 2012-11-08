/*
 * HelloWorld.cpp
 *
 *  Created on: 2012. 11. 4.
 *      Author: Ophelia
 */

#include <string>
#include "HelloWorld.h"

namespace nitroFrame{
HelloWorld* HelloWorld::Instance = NULL;

HelloWorld::HelloWorld(){
	this->apkPath = NULL;
}

HelloWorld::HelloWorld(const char* apkPath) {
	this->apkPath = const_cast<char*>(apkPath);
}

HelloWorld::~HelloWorld() {
}

void HelloWorld::npGameCreate(JNIEnv* env,const char* apkPath) {

	LOGE("Apk Path:%s",apkPath);
	this->m_env = env;

	this->apkPath = const_cast<char*>(apkPath);
	this->testRect = new testTextureRect();
	this->testRectCi = new npTextureRect();

	m_GameStates = GAMECREATE;
}

void HelloWorld::npShowIntro() {

	int renderWidth = 800;
	int renderHeight = 480;

	glClearColor(0.1, 0.3, 0.3, 0.4f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepthf(1.0f);
	glEnable(GL_DEPTH_TEST);		//depth test 실행
	glDepthFunc(GL_LEQUAL);		//depth test

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glLoadIdentity();
	//렌더러의 해상도를 정하는 부분
	//현재 기준 렌더러의 해상도는
	//width: 800g
	//height: 480

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	//Projection matrix초기화
	glOrthof(-400.0f, 400.0f, -240.0f, +240.0f, 1.0f, -9.0f);		//
	//glOrthof(-240.f, 240.f, -400.f, 400.f, 0, -10);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glViewport(0, 0, 800, 480);
	glViewport(0,0,npRenderprocess::getInstance().getDeviceWidth(),npRenderprocess::getInstance().getDeviceHeight());

	/*
	 gluLookAt(-400.0f, -240.0f,  0.0f,
					-400.0f , -240.0f,  -1.0f,
					0.0f,1.0f,0.0f);
	 */
	//npRenderprocess::getInstance().setLookat(-400.f, -240.f, 0.f, -400.f, -240.f, -1.f, 0.f, 1.f, 0.f);

	npContainerDAO::GetInstance().LoadTextureByXMLpath("running.xml");
	npContainerDAO::GetInstance().LoadTextureByXMLpath("ci.xml");

	 // Dummy용 Test Rect 설정
	this->testRect->SetTextureTAG("run");
	this->testRect->setX(0);
	this->testRect->setY(100);
	this->testRect->SetSize(100,100);

	this->testRect->indicate->SetTextureTAG("run");
	TouchLayer::GetInstance().RegistedObserver(this->testRect);

	LOGE("Done Update");
	m_GameStates = GAMEUPDATE;
}

void HelloWorld::npMainMenuState() {
}

void HelloWorld::npGameLoop() {
	switch(m_GameStates){
	case GAMECREATE:{
		npShowIntro();
	}
	case GAMEUPDATE:{
		npGameUpdate();
	}
	}
}

void HelloWorld::npGameDestroy() {
	delete Instance->testRect;
	delete Instance;
}

void HelloWorld::npGameUpdate() {

}

void HelloWorld::npGamePaused() {
}

void HelloWorld::npGameChanged(int width, int hegith) {
}

void HelloWorld::npGameDisplay() {

	//ICS Testing code
	/*
	this->testRect->SetTextureTAG("run");
	this->testRect->setX(0);
	this->testRect->setY(100);
	this->testRect->SetSize(100,100);

	this->testRect->indicate->SetTextureTAG("run");
	TouchLayer::GetInstance().RegistedObserver(this->testRect);
	 */

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //알파값을 빼기위해
	glAlphaFunc(GL_NOTEQUAL, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glPushMatrix();

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);		//2D

	npRenderprocess::getInstance().DoDraw(*this->testRect);

	glPopMatrix();
	glDisableClientState(GL_VERTEX_ARRAY);
}

HelloWorld* HelloWorld::GetInstance(){
	if(NP_IS_EMPTY(Instance)){
		Instance = new HelloWorld;
	}
	return Instance;
}

HelloWorld* HelloWorld::GetInstance(const char* apkPath){
	if(NP_IS_EMPTY(Instance)){
		Instance = new HelloWorld(apkPath);
	}
	return Instance;
}

}
