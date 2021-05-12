package nps.nitroframe.lib;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import nps.nitroframe.lib.npNativeEvent;

import android.app.NativeActivity;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView.Renderer;
import android.util.Log;
import android.view.MotionEvent;

/*! 
 * \brief     nitroFrame에서 사용되는 렌더러
 * \details	Android의 GLSurfaceView의 Renderer인터페이스를 확장한 클래스
 * 	onDrawFrame을 통해 native의 update logic과 Rendering Logic을 호출하게 한다.
 *  각 프레임간의 interval을 평균화 시키기 위해서 thread sleep을 이용한다. (자세한 사항은 onDrawFrame을 참조)
 * \author    박성민
 * \version   0.5
 */

public class npGLRenderer implements Renderer {
	private final static long NANOSECONDSPERSECOND = 1000000000L;
	private final static long NANOSECONDSPERMINISECOND = 1000000;
	private static long animationInterval = (long)(1.0 / 60 * NANOSECONDSPERSECOND);
	private long last;
	private int screenWidth;
	private int screenHeight;
	
	public void onDrawFrame(GL10 gl) {
		// TODO Frame간 Interval을 측정하여 bloking한다
		
    	long now = System.nanoTime();
    	long interval = now - last;
    	
    	npNativeEvent.npUpdateGame();
		npNativeEvent.npRendering();
    	
		if(interval < animationInterval){
    		try {
    			// because we render it before, so we should sleep twice time interval
    			Thread.sleep((animationInterval - interval) * 2 / NANOSECONDSPERMINISECOND);
    		} catch (Exception e){}
		}
    	last = now;
	}
	
	public void onDestroy(){
		npNativeEvent.npDestroy();
	}

	public void onSurfaceChanged(GL10 gl, int width, int height) {
		npNativeEvent.npSurfaceChanged(width, height);		
	}

	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		last = System.nanoTime();
	}
	
	public void setScreenWidthAndHeight(int width, int hegith){
		screenWidth = width;
		screenHeight = hegith;
	}
	
	public void onRenderingPause(){
		npNativeEvent.npOnPause();
	}
	
	public void onRenderingResume(){
		npNativeEvent.npOnResume();
	}
			
	public void sendNativeTouchEvent(int x, int y,int action, int pointerIndex){
		npNativeEvent.npOnTouchEvent(x, y, action & MotionEvent.ACTION_MASK, pointerIndex);
	}
	
	public void sendDoubleTap(int x, int y){
		npNativeEvent.npDoubleTap(x, y);
	}
	
	public void setNativeInitialize(int width, int height, AssetManager asset,String apkPath){
		npNativeEvent.npSurfaceCreate(width, height, asset, apkPath);
	}
}
