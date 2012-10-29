/*
 * npAudioSystem.h
 *
 *  Created on: 2012. 7. 24.
 *      Author: NitroPigSoft02
 */

#ifndef NPAUDIOSYSTEM_H_
#define NPAUDIOSYSTEM_H_
#include "jni.h"

namespace projectBean{

class npAudioSystem{
private:
	npAudioSystem();
	npAudioSystem(JNIEnv* env);		//JNIEnv를 기반으로 동작하는 클래스
	~npAudioSystem();

	JavaVM* javaVm;
	JNIEnv* env;
	jclass m_audioCallbackClass;
	static	npAudioSystem* singleObject;
public:

	static void Initialize(JNIEnv* env);
	static void Release();

	static void stopBGM();
	static void playBGM();

	static void playEffect(int playkey);
};

}
#endif /* NPAUDIOSYSTEM_H_ */
