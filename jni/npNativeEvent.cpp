#include <string.h>
#include <jni.h>
#include <iostream>
#include "./TestUnit/HelloWorld.h"
#include "npNativeEvent.h"
//#include "nitroFrame/NitroFrame.h"
#include "./nitroFrame/npDebugUtill.h"
//#include "pbMainFrame.h"
//#include "pbTouchLayer.h"



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
	char* str;
	jboolean isCopy;
	str = const_cast<char*>(env->GetStringUTFChars(apkPath, &isCopy));
	nitroFrame::HelloWorld::GetInstance(str)->npGameCreate(env,w,h,str);

	npContainerDAO::SetupBaseInitialize(env, assetManager, str);
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


JNIEXPORT void JNICALL Java_nps_nitroframe_lib_npNativeEvent_npUpdateGame(JNIEnv* env, jclass thiz)
{
	/*
	//updateGameLoop();
	projectBean::pbProjectBeanFrame* gameFrame = projectBean::pbProjectBeanFrame::MainFrame;
	gameFrame->npGameLoop();
	*/
	nitroFrame::HelloWorld::GetInstance()->npGameLoop();
}

JNIEXPORT void JNICALL Java_nps_nitroframe_lib_npNativeEvent_npDestroy(JNIEnv* env, jclass thiz)
{
	/*
	projectBean::pbProjectBeanFrame* gameFrame = projectBean::pbProjectBeanFrame::MainFrame;
	gameFrame->npGameDestroy();
	delete projectBean::pbProjectBeanFrame::MainFrame;
	*/
	nitroFrame::HelloWorld::GetInstance()->npGameDestroy();
}

JNIEXPORT void JNICALL Java_nps_nitroframe_lib_npNativeEvent_npRendering(JNIEnv *env, jclass thiz){
	/*
	projectBean::pbProjectBeanFrame* gameFrame = projectBean::pbProjectBeanFrame::MainFrame;
	gameFrame->npGameDisplay();
	*/
	nitroFrame::HelloWorld::GetInstance()->npGameDisplay();
}


JNIEXPORT void JNICALL Java_nps_nitroframe_lib_npNativeEvent_npOnTouchEvent(JNIEnv* env, jclass thiz, jint x, jint y, jint touchFlag, jint pointerNumber)
{
	/*
	//onTouchEvent(x, y, touchFlag, pointerNumber);
	projectBean::pbTouchLayer::onTouchPoint(x,y,touchFlag, pointerNumber);
	*/
}
JNIEXPORT void JNICALL Java_nps_nitroframe_lib_npNativeEvent_npOnTouchPoint0(JNIEnv *env, jclass thiz, jint x, jint y, jint touchFlag){

}

/*JNIEXPORT void JNICALL Java_nps_project_bean2_npNativeEvent_npSetTextureData(JNIEnv* env, jclass thiz, jintArray pixels, jint width, jint height, jint genIndex)
{
	 jint *temp = env->GetIntArrayElements(pixels, 0);
	 char *data =(char*)temp;

	 setTextureData(data,width,height, genIndex);

	 env->ReleaseIntArrayElements(pixels,(jint*)data,JNI_ABORT);
}*/

/*JNIEXPORT jint JNICALL Java_nps_project_bean2_npNativeEvent_npTouchScale(JNIEnv *env, jclass thiz, jfloatArray scaleArray)
{
	 float* scales = env->GetFloatArrayElements(scaleArray,0);
	int renderpart = onTouchScale(scales);
	env->ReleaseFloatArrayElements((jfloatArray)scaleArray,scales,JNI_ABORT);

	return renderpart;
}*/

JNIEXPORT void JNICALL Java_nps_nitroframe_lib_npNativeEvent_npDoubleTap(JNIEnv *env, jclass thiz, jint x, jint y)
{
	//projectBean::pbTouchLayer::onDoubleTapEvent(x, y);
}

/*JNIEXPORT void JNICALL Java_nps_project_bean2_npNativeEvent_npTextureGenSize(JNIEnv *env, jclass thiz, jint size)
{
	onTextureGenSize(size);
}*/
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
