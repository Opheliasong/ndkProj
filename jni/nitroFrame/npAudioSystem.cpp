/*
 * npAudioSystem.cpp
 *
 *  Created on: 2012. 7. 26.
 *      Author: NitroPigSoft02
 */
#include "npDebugUtill.h"
#include "npAudioSystem.h"
namespace projectBean{

npAudioSystem* npAudioSystem::singleObject;

npAudioSystem::npAudioSystem(){

}

npAudioSystem::~npAudioSystem(){

}

void npAudioSystem::Initialize(JNIEnv* env){
	singleObject = new npAudioSystem;

	singleObject->env = env;
	singleObject->env->GetJavaVM(&singleObject->javaVm);
	//singleObject->m_audioCallbackClass = singleObject->env->FindClass("nps/project/bean2/npSoundManager");
	if(singleObject->m_audioCallbackClass == 0){
		return;
	}
}
void npAudioSystem::Release(){
	delete singleObject;
	LOGI("AudioSystem Release");
}

void npAudioSystem::stopBGM(){

}
void npAudioSystem::playBGM(){

}
void npAudioSystem::playEffect(int playkey){

	jmethodID soundMethod = 0;


	JNIEnv* env = singleObject->env;
	jclass* methodClass = &singleObject->m_audioCallbackClass;
	singleObject->javaVm->AttachCurrentThread(&env,NULL);

	if(env == 0){
		return;
	}

	//soundMethod = JNIEnv::GetStaticMethodID(singleObject->m_audioCallbackClass,"PlayEffectSound","(I)V");
	soundMethod = env->GetStaticMethodID(singleObject->m_audioCallbackClass,"PlayEffectSound","(I)V");

	if(soundMethod == NULL){
		return;
	}

	env->CallStaticVoidMethod(singleObject->m_audioCallbackClass,soundMethod,(int)playkey);
}
}
