/*
 * HelloWorld.h
 *
 *  Created on: 2012. 11. 4.
 *      Author: Ophelia
 */

#ifndef HELLOWORLD_H_
#define HELLOWORLD_H_

#include "../nitroFrame/npEngineFrames.h"
#include "../nitroFrame/Drawable/npTextureRect.h"
#include "../nitroFrame/Render/npRenderProcess.h"

namespace nitroFrame{
class HelloWorld :public npEngineFrame{
public:
	HelloWorld(const char* apkPath);
	~HelloWorld();

	void npGameCreate(JNIEnv* env,int width, int height,const char* apkPath);

	void npShowIntro();
	void npMainMenuState();
	void npGameLoop();
	void npGameDestroy();
	void npGameUpdate();
	void npGamePaused();

	void npGameChanged(int width, int hegith);
	void npGameDisplay();

	static HelloWorld* GetInstance();
	static HelloWorld* GetInstance(const char* apkPath);

protected:
	HelloWorld();
	static HelloWorld* Instance;

	npTextureRect* testRect;
	npTextureRect* testRectCi;

	int deviceWidth;
	int deviceHeight;

};
}


#endif /* HELLOWORLD_H_ */
