#include <string.h>
#include <jni.h>
#include <iostream>
#include "./ProjectBean/pbMainFrame.h"
#include "npNativeEvent.h"
//#include "nitroFrame/NitroFrame.h"
#include "./nitroFrame/npDebugUtill.h"
//#include "pbMainFrame.h"
//#include "pbTouchLayer.h"

#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT void JNICALL Java_nps_nitroframe_lib_npNativeEvent_npSurfaceCreate(JNIEnv *env, jclass thiz, jint w, jint h,jobject assetManager, jstring apkPath)
{
	/*
	if( projectBean::pbProjectBeanFrame::MainFrame == NULL)
	{
		projectBean::pbProjectBeanFrame::MainFrame = new projectBean::pbProjectBeanFrame;
		projectBean::pbProjectBeanFrame* gameFrame = projectBean::pbProjectBeanFrame::MainFrame;

		const char* str;
		jboolean isCopy;
		str = env->GetStringUTFChars(apkPath, &isCopy);
		gameFrame->npGameCreate(env,w,h, assetManager, str);
	}*/
	JavaVM* javaVm;
	env->GetJavaVM(&javaVm);

	char* str;
	jboolean isCopy;
	str = const_cast<char*>(env->GetStringUTFChars(apkPath, &isCopy));
	LOGE("Create & Initialize Framework");
	nitroFrame::pbMainFrame::GetInstance()->npGameCreate(env,str);
	npRenderprocess::getInstance().setDeviceResolution(w,h);
	//npRenderprocess::getInstance().setRenderingResolution(480,800);
	npRenderprocess::getInstance().setRenderingResolution(800,480);

	npContainerDAO::SetupBaseInitialize(javaVm, assetManager, str);
	/*
	npInsertDAO* dummyInsertDAO = new npInsertDAO;
	dummyInsertDAO->InsertDummyRawSource();
	dummyInsertDAO->DeleteDummyRawSource();
	dummyInsertDAO->PrintDummyRawSource();
*/
	/*
	npLinkNode<int>* destList = new npLinkNode<int>;
	npLinkNode<int>* sourceList = new npLinkNode<int>;

	int a = 10;
	int b = 20;
	int c = 30;

	LOGI("Create Start SourceList");

	npLinkNode<int>::addTail(npLinkNode<int>::makeLinkNode(a), sourceList);
	npLinkNode<int>::addTail(npLinkNode<int>::makeLinkNode(b), sourceList);
	npLinkNode<int>::addTail(npLinkNode<int>::makeLinkNode(c), sourceList);

	LOGI("Create Done SourceList");
	npLinkNode<int>::copyList(destList, sourceList);
	npLinkNode<int>* iter = destList;

	for(iter = destList->getNext(); iter != destList; iter = iter->getNext() ){
		LOGfloatString("Dest Value: ",(int)iter->getKernel());
	}

	npLinkNode<int>::destroyList(destList);
	npLinkNode<int>::destroyList(sourceList);
	*/
}


JNIEXPORT void JNICALL Java_nps_nitroframe_lib_npNativeEvent_npSurfaceChanged(JNIEnv* env, jclass thiz, jint w , jint h)
{
	/*
	//onSurfaceChanged(w, h);
	projectBean::pbProjectBeanFrame* gameFrame = projectBean::pbProjectBeanFrame::MainFrame;
	gameFrame->npGameChanged(w,h);
	*/
}

JNIEXPORT void JNICALL Java_nps_nitroframe_lib_npNativeEvent_npUpdateGame(JNIEnv *env, jclass thiz){
	nitroFrame::pbMainFrame::GetInstance()->npGameLoop();
}

JNIEXPORT void JNICALL Java_nps_nitroframe_lib_npNativeEvent_npDestroy(JNIEnv* env, jclass thiz)
{
	/*
	projectBean::pbProjectBeanFrame* gameFrame = projectBean::pbProjectBeanFrame::MainFrame;
	gameFrame->npGameDestroy();
	delete projectBean::pbProjectBeanFrame::MainFrame;
	*/
	nitroFrame::pbMainFrame::GetInstance()->npGameDestroy();
}

JNIEXPORT void JNICALL Java_nps_nitroframe_lib_npNativeEvent_npRendering(JNIEnv *env, jclass thiz){
	/*
	projectBean::pbProjectBeanFrame* gameFrame = projectBean::pbProjectBeanFrame::MainFrame;
	gameFrame->npGameDisplay();
	*/
	nitroFrame::pbMainFrame::GetInstance()->npGameDisplay();
}

JNIEXPORT void JNICALL Java_nps_nitroframe_lib_npNativeEvent_npAndroidStatePause(JNIEnv *env, jclass thiz, jint state)
{
	/*
	projectBean::pbProjectBeanFrame* gameFrame = projectBean::pbProjectBeanFrame::MainFrame;
	if(gameFrame != 0){
		gameFrame->npGamePaused();
	}
	*/
}

JNIEXPORT void JNICALL Java_nps_nitroframe_lib_npNativeEvent_npOnResume(JNIEnv *env, jclass thiz){

}

JNIEXPORT void JNICALL Java_nps_nitroframe_lib_npNativeEvent_npOnPause(JNIEnv *env, jclass thiz){

}
#ifdef __cplusplus
}
#endif
