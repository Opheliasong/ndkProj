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

void HelloWorld::npGameCreate(JNIEnv* env, int width, int height,const char* apkPath) {
	Instance->m_Width = width;
	Instance->m_Height = height;
	Instance->m_env = env;

	this->testRect = new npTextureRect();
	this->testRectCi = new npTextureRect();

	m_GameStates = GAMECREATE;
}

void HelloWorld::npShowIntro() {
	glClearColor(0.1, 0.3, 0.3, 0.4f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepthf(1.0f);
	glEnable(GL_DEPTH_TEST);		//depth test 실행
	glDepthFunc(GL_LEQUAL);		//depth test

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();									//Projection matrix초기화

	//렌더러의 해상도를 정하는 부분
	//현재 기준 렌더러의 해상도는
	//width: 800g
	//height: 480
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, 480, 800);

	glOrthof(-480/2, 480/2, -800/2, 800/2, 0, -10);

	m_GameStates = GAMEUPDATE;

	npContainerDAO::GetInstance().LoadTextureByXMLpath("running.xml");
	npContainerDAO::GetInstance().LoadTextureByXMLpath("ci.xml");

	/*
	 * Dummy용 Test Rect 설정
	 */
	this->testRect->SetTextureTAG("run");
	this->testRect->setX(-100);
	this->testRect->setY(0);
	this->testRect->SetSize(100,100);

	this->testRectCi->SetTextureTAG("ci");
	this->testRectCi->setX(100);
	this->testRectCi->setY(0);
	this->testRectCi->SetSize(50,50);
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
	delete Instance;
}

void HelloWorld::npGameUpdate() {

}

void HelloWorld::npGamePaused() {
}

void HelloWorld::npGameChanged(int width, int hegith) {
}

void HelloWorld::npGameDisplay() {
	//glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //알파값을 빼기위해
	glAlphaFunc(GL_NOTEQUAL, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glPushMatrix();

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);		//2D

	//npRenderprocess::getInstance().DoDraw(*testRect);
	//npRenderprocess renderProcess;
	//renderProcess.DoDraw(*testRect);
	testRect->PreSettingDraw();
	testRect->DrawThis();

	testRectCi->PreSettingDraw();
	testRectCi->DrawThis();


	glPopMatrix();
	glDisableClientState(GL_VERTEX_ARRAY);
}

HelloWorld* HelloWorld::GetInstance(){
	return Instance;
}

HelloWorld* HelloWorld::GetInstance(const char* apkPath){
	Instance = new HelloWorld(apkPath);
	return Instance;
}

}
