package nps.nitroframe.lib;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.Display;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.WindowManager;
import nps.nitroframe.lib.npNativeEvent;

/**
 * @author 박성민
 *  npGLSurfaceView
 * birf: GLSurfaceView를 상속받아 확장한 클래스.
 * nitroFrame을 사용하기 위해 제작되어져있으며 npRenderer를 이용하여 rendering한다.
 * native단의 Create를 호출하여 native에서의 Initialize를 수행하게 한다.
 * TouchEvent를 판독하여 Renderer의 Event Queue에 등록하는 역할을 수행한다.
 * 
 */

public class npGLSurfaceView extends GLSurfaceView {
	private npGLRenderer m_renderer;
	private AssetManager m_assetManager;
	
	private int m_Width;
	private int m_Height;
	
	private Context m_Context;
	private GestureDetector m_GestureDetector;
    private int m_touchCount;
	private boolean m_OnInitilized=false;
    private touchPointer m_arrTouchPointer[] = new touchPointer[3];

    /**
     * @author 박성민
     *  *touchPointer
     * 멀티 터치를 위하여 터치 좌표를 가지고 있는 객체를 말한다.
     * 멀티터치의 id와 index값을 가지며, 이를 이용하여 멀티터치에 필요한 작업을 수행한다.
     */
    class touchPointer{
    public float m_x;
    public float m_y;
    public int m_id;
    public int m_Index;
    public touchPointer(){
    	m_x = 0;
    	m_y = 0;
    	m_id = -1;
    	m_Index = -1;
    }
    public touchPointer(int id){
    	m_x = 0;
    	m_y = 0;
    	m_id = id;
    	m_Index = -1;
    }
    
    }

	public npGLSurfaceView(Context context,AssetManager mgr) {
		super(context);
		m_Context = context;
		setDebugFlags(DEBUG_CHECK_GL_ERROR | DEBUG_LOG_GL_CALLS);
		
		m_assetManager = mgr;
		//renderer 생성
		m_renderer = new npGLRenderer();
		setRenderer(m_renderer);
		setRenderMode(RENDERMODE_CONTINUOUSLY);		
		requestFocus();
		
	}
	
	public void NativeInitialize(){
		setFocusableInTouchMode(true);
		
		try{
		Display display = ((WindowManager)m_Context.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
		m_Width = display.getWidth();
		m_Height = display.getHeight();
		
		/*
		Log.i("GLview", "width:"+m_Width);
		Log.i("GLview", "height:"+m_Height);
		*/
		
		}catch(Exception e){
			Log.e("GLView", "GLView CTOR Exception: "+e.toString());
		}
		String apkFilePath = null;
		ApplicationInfo appInfo = null;
		PackageManager packMgmr = m_Context.getPackageManager();
		try {
			appInfo = packMgmr.getApplicationInfo("nps.nitroframe.test", 0);
		} catch (NameNotFoundException e) {
			e.printStackTrace();
			throw new RuntimeException("Unable to locate assets, aborting...");
		}
		
		apkFilePath = appInfo.sourceDir;
		
		npNativeEvent.npSurfaceCreate(m_Width, m_Height, m_assetManager, apkFilePath);
		m_OnInitilized = true;
	}
	
	public void onPause(){
		queueEvent(new Runnable() {
			
			public void run() {
				// TODO Auto-generated method stub
				m_renderer.onRenderingPause();
			}
		});
	}
	
	public void onResume(){
		queueEvent(new Runnable() {
			
			public void run() {
				// TODO Auto-generated method stub
				m_renderer.onRenderingResume();
			}
		});		
	}
	

	/**
	 * @author 박성민
	 * Touch Events
	 * brif : 터치 이벤트에 관련된 로직 수행
	 * runalbe을 이용하여 EventQueue에 등록하여 수행하게 한다.
	*/
	@Override
	public boolean onTouchEvent(MotionEvent event) 
	{
		if(m_OnInitilized == true){
			if(m_GestureDetector == null){
				m_GestureDetector = new GestureDetector(m_Context, new npGestureLinstener());
			}
			m_GestureDetector.onTouchEvent(event);
		    m_touchCount = event.getPointerCount();
		    
	    	switch(m_touchCount){
	    	case 1:{
	    		// 싱글 터치 로직으로 들어간다.
	    		if(singleTouchEventProcess(event)){
	    			return true;
	    		}
	    		return false;
	    	}
	    	case 2:{
	    		//더블 터치 로직으로 들어간다.
	    		if(dualTouchEventProcess(event)){
	    			return true;
	    		}
	    		return false;
	    	}
	    	default:{
	    		//듀얼로직으로 우회한다.
	    		if(detourDualProcess(event)){
	    			return true;
	    		}
	    		return false;
	    	}
		}
		}
		return false;
	}
	
	private void sendNativeTouchEvent(MotionEvent event,final int action, final int pointerIndex){
		final float x = event.getX(pointerIndex);
		final float y = event.getY(pointerIndex);
		
		queueEvent(new Runnable() {
			
			public void run() {
				// TODO Auto-generated method stub
				m_renderer.sendNativeTouchEvent((int)x, (int)y, action, pointerIndex);
			}
		});		
	}
    
    public boolean singleTouchEventProcess(MotionEvent event){
    	int action = event.getAction() & MotionEvent.ACTION_MASK;
    	
    	switch(action){
    	case MotionEvent.ACTION_DOWN:{
    		sendNativeTouchEvent(event, MotionEvent.ACTION_DOWN, 0);
    		return true;
    	}
    		
    	case MotionEvent.ACTION_MOVE:{
    		sendNativeTouchEvent(event, MotionEvent.ACTION_MOVE, 0);
    		return true;
    		}
    	case MotionEvent.ACTION_UP:{
    		sendNativeTouchEvent(event, MotionEvent.ACTION_UP, 0);
    		return true;
    	}
    	}
    	return false;
    }
    
    public boolean dualTouchEventProcess(MotionEvent event){
    	int action = event.getAction();
    	
    	switch(action & MotionEvent.ACTION_MASK){
	    	case MotionEvent.ACTION_POINTER_1_DOWN:{
	    		sendNativeTouchEvent(event, MotionEvent.ACTION_DOWN, 1);
	    		return true;
	    	}
	    		
	    	case MotionEvent.ACTION_MOVE:{
	    		if(m_arrTouchPointer[0].m_id == 0){
	    			int i = 0;
	    			try{
	    				for(i = 0; i < 2; i++){
	    					m_arrTouchPointer[i].m_x = event.getX(i);
	    					m_arrTouchPointer[i].m_y = event.getY(i);
	    					sendNativeTouchEvent(event, MotionEvent.ACTION_MOVE, i);
	    				}
	    			}catch(Exception e){
	    				Log.e("MotionERROR","MotionEvent Move: Node[0]");
	    				Log.e("MotionERROR","Dual_move i : "+i);
	    			}
	    		}
	    		return true;
			}
	    	
			case MotionEvent.ACTION_POINTER_1_UP:{
	    		sendNativeTouchEvent(event, MotionEvent.ACTION_UP, 1);
				return true;
			}
        	
    	}

    	return false;
    }
    
    public boolean detourDualProcess(MotionEvent event){
    	int action = event.getAction();
    	
    	switch(action & MotionEvent.ACTION_MASK){
    		case MotionEvent.ACTION_POINTER_DOWN:{
    			return false;
    		}
    		case MotionEvent.ACTION_MOVE:{
	    		if(m_arrTouchPointer[0].m_id == 0){
	    			int i = 0;
	    			try{
	    				for(i = 0; i < 2; i++){
	    					m_arrTouchPointer[i].m_x = event.getX(i);
	    					m_arrTouchPointer[i].m_y = event.getY(i);
	    					sendNativeTouchEvent(event, MotionEvent.ACTION_MOVE, i);
	    				}
	    			}catch(Exception e){
	    				Log.e("MotionERROR","MotionEvent Move: Node[0]");
	    				Log.e("MotionERROR","Dual_move i : "+i);
	    			}
	    		}
    			return true;
    		}
    		case MotionEvent.ACTION_POINTER_UP:{
    			return false;
    		}
    	
    	}
    	return false;
    }


    /*Gesture Linstener
     * birf:
     * 
     */
	private class npGestureLinstener extends GestureDetector.SimpleOnGestureListener
	{
		@Override
		public boolean onDoubleTap(MotionEvent event)
		{
			npNativeEvent.npDoubleTap((int)event.getX(), (int)event.getY());
			return true;
		}
	}
}
