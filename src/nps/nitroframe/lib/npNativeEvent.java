package nps.nitroframe.lib;

import android.util.Log;

public class npNativeEvent {
	public static native void npSurfaceCreate(int width, int height,Object asset, String apkPath);
	public static native void npSurfaceChanged(int width, int height);
	public static native void npUpdateGame();
	public static native void npDestroy();
	public static native void npRendering();	
	
	// 터치
	public static native void npOnTouchEvent(int x, int y, int touchFlag, int pointerNumber);
	
	//멀티터치용 터치 이벤트 함수(Native Call 함수)
	public static native void npOnTouchPoint0(int x, int y, int touchFlag);
	public static native void npOnTouchPoint1(int x, int y, int touchFlag);
	public static native void npOnTouchPoint2(int x, int y, int touchFlag);
	
	public static native void npDoubleTap(int x, int y);
	
	//상태
	public static native void npAndroidStatePause(int state);
	public static native void npOnResume();
	public static native void npOnPause();
	
	public static void staticKillProcess(){
		Log.e("","Call Kill Process");
		android.os.Process.killProcess(android.os.Process.myPid());
	}
}
