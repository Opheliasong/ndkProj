/*
 * pbMainFrame.h
 *
 *  Created on: 2012. 8. 7.
 *      Author: NitroPigSoft02
 */

#ifndef PBMAINFRAME_H_
#define PBMAINFRAME_H_

#include "../stdafx.h"

namespace nitroFrame{
class pbMainFrame :public npEngineFrame{
public:
	pbMainFrame(const char* apkPath);
	~pbMainFrame();

	void npGameCreate(JNIEnv* env,const char* apkPath);

	void npShowIntro();
	void npMainMenuState();
	void npGameLoop();
	void npGameDestroy();
	void npGameUpdate();
	void npGamePaused();

	void npGameChanged(int width, int hegith);
	void npGameDisplay();

	static pbMainFrame* GetInstance();
	static pbMainFrame* GetInstance(const char* apkPath);

	void npOnBackPressed();

protected:
	pbMainFrame();
	static pbMainFrame* Instance;

	bool m_bFirstLoad;
};
}
#endif /* PBMAINFRAME_H_ */
