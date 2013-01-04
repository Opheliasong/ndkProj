package nps.nitroframe.lib;

import nps.nitroframe.test.R;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.KeyEvent;
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
		this.GLSurfaceView.NativeInitialize();
	}
	
	
	public boolean onKeyDown(int keyCode, KeyEvent event){
		switch(keyCode){
		case KeyEvent.KEYCODE_BACK:
			String alertTitle = getResources().getString(R.string.alertExitTitle);
			String alertText = getResources().getString(R.string.alertExitMsg);
			String alertYesBTN = getResources().getString(R.string.alertExitYES);
			String alertNoBTN = getResources().getString(R.string.alertExitNO);
			
		    new AlertDialog.Builder(this)
		      .setTitle(alertTitle)
		      .setMessage(alertText)
		      .setPositiveButton(alertYesBTN, new DialogInterface.OnClickListener() {
		       
		       public void onClick(DialogInterface dialog, int which) {
		    	   npGameActivity.this.onDestory();
		       }
		      })
		      .setNegativeButton(alertNoBTN, null)
		      .show();
		     }
		    return true;
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
	
	public void onDestory(){
		super.onDestroy();
		this.GLSurfaceView.onDestroy();
	}
}
