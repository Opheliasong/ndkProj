package nps.nitroframe.lib;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.WindowManager;

/**
 * @author 박성민
 * 
 *
 */
public class npGameActivity extends Activity {
	
	protected npGLSurfaceView GLSurfaceView;
	private npSoundManager soundManager;
	private static AssetManager assetManager;
	private static String packageName;
	
	protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		
		//SoundManager 생성
		this.soundManager = new npSoundManager();
		this.soundManager.Initialize(getBaseContext());
		
		//AssetManager 가져오기
		this.assetManager = getAssets();
		
		//SurfaceView 생성 및 Content적용
		this.GLSurfaceView = new npGLSurfaceView(this, this.assetManager);
		this.setContentView(this.GLSurfaceView);
	}
	
	//onResume 이벤트 처리
	protected void onResume(){
		super.onResume();
		this.GLSurfaceView.onResume();
	}
	
	protected void onPause(){
		super.onPause();
		this.GLSurfaceView.onPause();
	}
}
