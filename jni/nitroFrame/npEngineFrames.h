/*
 * npEngineFrames.h
 *
 *  Created on: 2012. 8. 7.
 *      Author: NitroPigSoft02
 */

#ifndef NPENGINEFRAMES_H_
#define NPENGINEFRAMES_H_

#include <GLES/gl.h>
#include <GLES/glext.h>
#include <time.h>
#include <math.h>
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "NitroFrame.h"

/*
 * ���� �����ӿ�(����)�� ���븦 �����ϴ� abstract Ŭ����
 * npGameDisplay()�Լ��� Define�ؼ� ����ؾ� �Ѵ�
 * ���ӿ��� ���Ǵ� ���³� �⺻ ����(�ػ�,�ð� ���� ��)�� ������ �ְ� �ȴ�.
 *
 * #member Variable
 * (i)Width = ���� ����̽��� Width��
 * (i)Height = ���� ����̽��� height ��
 * (clock_t)beginTime = update �Լ��� ȣ��Ǳ� ���� �ð� ��
 * (f)FPS = ������ ��ü FPS
 * (d)timeDelta = ���� ������Ʈ�� ���� ������Ʈ���� �ð����̸� ������ ����
 *
 * @npGameCreate : Process�� ��Ǿ��� ��� ȣ��Ǵ� �Լ�
 * @npGameDestroy : Process�� ����Ǿ��� ��� ȣ��Ǵ� �Լ�
 * @npGameLoop: ������ �⺻���� LoopLogic
 * @npGameUpdate : ������ ��� ��������� ��� ȣ��Ǵ� �Լ�
 * @npGamePaused : ������ ���������� �Ͻ� �����ÿ� ȣ��Ǵ� �Լ�
 * @npGameChanged : ������ ���°� ����Ǿ��� ���
 * @npShowIntro : ���� ���μ����� ����ȭ�ǰ� Intro ȭ���� �����ִ� Logic
 * @npMainMenuState : �������μ����� ���°� ���θ޴�ȭ������ �Ѿ ��Ȳ �Ͻ�
 *
 * ==Utiltiy ��==
 * @npUpdateTimeDelta : beginTime�� ���� �����Ѵ�.
 * @npCheckTimeDelta : beginTime�������� ���ݱ����� �ð��� �����Ͽ� ���� �����Ѵ�.
 * @setOrth : ������������� ����� ��� ȣ���Ѵ�.
 * @setFrustum : ������������ ����� ��� ȣ���Ѵ�.
 *
 */
namespace nitroFrame{

class npEngineFrame {

public:
	/*
	 * #GAMECREATE : ���� ���μ����� ���μ����� OS�� ����ť(Ready Queue)�� �ö� ��ø� ���Ѵ�.
	 *   �̶����� intro �� ���� ���μ��� �ʱ� �۾��� �����Ѵ�.
	 * #GAMEMAINMENU : ������ Ÿ��Ʋ�� ���� �޴��� �б��� UI�� �ַ� ��������
	    * ���� In-game ���� ��������. ����ȭ������ �������� ���� �����Ѵ�.
	 * #GAMEUPDATE : In-Game ������ update ������ ���Ѵ�
	 * #GAMEPAUSE : In-Game���� Pause�� ��Ȳ�� ���Ѵ�. �ַ� �޴�ȭ���� ����� ��츦 ���Ѵ�.
	 * #GAMEDESTROY : ���� ���μ����� ������ Interrupt Signal�� ���ؼ� ����Ǵ� ���¸� ���Ѵ�.
	 */
	typedef enum {GAMECREATE = 0,GAMEMAINMENU,GAMEUPDATE,GAMEPAUSE,GAMEDESTROY}GAMESTATE;

protected:
	int m_Width;
	int m_Height;

	double m_InnerWidthRatio;
	double m_InnerHeightRatio;

	clock_t m_beforeTime;
	double m_timeDelta;
	float m_FPS;
	JNIEnv* m_env;
	jobject m_AssetManager;
	AAsset* m_Asset;

	float m_bFirstLoad;  // �߰� : ��ε� & �޸� ���� ����  8-27
protected:
	virtual void setOrth(int width, int height, float zNear, float zFar);
	virtual void setFrustum(int width, int hegith, float zNear, float zFar);

public:
	GAMESTATE m_GameStates;

	npEngineFrame();
	virtual ~npEngineFrame();

	//�⺻ Frame �Լ���
	virtual void npGameCreate(JNIEnv* env,int width, int height);

	virtual void npShowIntro()=0;
	virtual void npMainMenuState();
	virtual void npGameLoop();
	virtual void npGameDestroy();
	virtual void npGameUpdate();
	virtual void npGamePaused();

	virtual void npGameChanged(int width, int hegith);
	virtual void npGameDisplay();

	//Utility �Լ�
	double npUpdateTimeDelta();
	double npCheckTimeDelta();

	int GetWidth() { return m_Width;}
	int GetHeight() { return m_Height;}

	JNIEnv* GetEnv() {return m_env;}
	jobject GetAssetManager() { return m_AssetManager;}
};
}

#endif /* NPENGINEFRAMES_H_ */

