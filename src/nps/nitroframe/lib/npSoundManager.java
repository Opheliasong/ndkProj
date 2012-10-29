package nps.nitroframe.lib;

import java.util.Collection;
import java.util.HashMap;

import android.content.Context;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;
import android.util.Log;

public class npSoundManager {
	
	private static final String LOG_TAG = npSoundManager.class.getSimpleName();
	
	static private npSoundManager m_Instance;
	
	//Audio 관련
	//private MediaPlayer background;		//배경 음악 Media player
	static private HashMap<Integer, Integer> m_soundPoolMap;
	static private SoundPool m_SoundPool;
	static private AudioManager m_AudioManager;
	static 	private MediaPlayer m_background;	
	
	static private Context m_Context;
	
	public npSoundManager(){
		if(m_Instance != null){
			m_Instance = new npSoundManager(); 	
		}
	}
	
	static public npSoundManager GetInstance(){
		if(m_Instance == null){
			m_Instance = new npSoundManager();
		}
		return m_Instance;
	}
	
	static public void Initialize(Context context){
		
		if(m_Instance == null){
			m_Instance = new npSoundManager();
		}
		
		m_Context = context;
		m_SoundPool = new SoundPool(50,AudioManager.STREAM_MUSIC,0);
		m_soundPoolMap = new HashMap<Integer, Integer>();
		m_AudioManager = (AudioManager)m_Context.getSystemService(Context.AUDIO_SERVICE);
	}
	
	static public void Release(){
		if(m_SoundPool != null){
			Collection<Integer> soundIdCollection = m_soundPoolMap.values();
			for(int soundID : soundIdCollection){
				m_SoundPool.unload(soundID);
				Log.d(LOG_TAG, "Destroy sound ID"+soundID);
			}
			m_soundPoolMap = null;
		}
		
		if(m_background != null){
			m_background.stop();
			m_background.release();
		}
	}
	
	/*
	 * Load(Adding hash map & Sound Pool)
	 * 
	 */
	
	static public void LoadSound(int key, int resourceID){
		m_soundPoolMap.put(key, m_SoundPool.load(m_Context, resourceID, 1));
	}
	
	static public void LoadBackGroundMusic(int resourceID){
	   m_background = MediaPlayer.create(m_Context, resourceID);
	   m_background.setLooping(true);
	}
	
	/*
	 * Play & Stop
	 * 
	 */
	
	static public void PlayEffectSound(int key){
		
	   	float streamCurrent = m_AudioManager.getStreamVolume(AudioManager.STREAM_MUSIC);
	   	float streamMax = m_AudioManager.getStreamMaxVolume(AudioManager.STREAM_MUSIC);
	   	float streamVolume = streamCurrent/streamMax;
	   	
	   	m_SoundPool.play(m_soundPoolMap.get(key), streamVolume, streamVolume, 0, 0, 1.0f);
	}
	
	static public void playLoopEffectSound(int key){
	   	float streamCurrent = m_AudioManager.getStreamVolume(AudioManager.STREAM_MUSIC);
	   	float streamMax = m_AudioManager.getStreamMaxVolume(AudioManager.STREAM_MUSIC);
	   	float streamVolume = streamCurrent/streamMax;
	   	
	   	m_SoundPool.play(m_soundPoolMap.get(key), streamVolume, streamVolume, 1,-1, 1f);
	}
	
	static public void playRepeatEffectSound(int key,int LoopCount){
	   	float streamCurrent = m_AudioManager.getStreamVolume(AudioManager.STREAM_MUSIC);
	   	float streamMax = m_AudioManager.getStreamMaxVolume(AudioManager.STREAM_MUSIC);
	   	float streamVolume = streamCurrent/streamMax;
	   	
	   	m_SoundPool.play(m_soundPoolMap.get(key), streamVolume, streamVolume, 1,LoopCount, 1f);
	}
	
	static public void StopEffectSound(int key){
		m_SoundPool.stop(m_soundPoolMap.get(key));
	}
	
	static public void PauseEffectSound(int key){
		m_SoundPool.pause(m_soundPoolMap.get(key));
	}
	
	static public void ResumeEffectSound(int key){
		m_SoundPool.resume(m_soundPoolMap.get(key));
	}
	
	static public void PlayBGM(){
		if(m_background != null){
			m_background.start();
		}
	}
	
	static public void PauseBGM(){
		if(m_background != null){
			m_background.pause();
		}
	}
	/*
	static public void ResumeBGM(){
		if(m_background != null){
			m_background.re();
		}
	}*/
	
	static 	public void StopBGM(){
		if(m_background != null){
			m_background.stop();
		}
	}
	
	
	/*
	 * BGM Volume Control 
	 */
	
	static public void ControlBackGroundVolume(float left,float right){
		m_background.setVolume(left, right);
	}
}
