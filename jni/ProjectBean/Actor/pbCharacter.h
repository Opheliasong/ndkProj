#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "../stdafx.h"

using namespace projectBean;

class pbMarionette;
class pbCharacter : public npDrawable, public iTouchObserver{
private:
	pbCharacter();
	~pbCharacter();

public:
	static void Create();

	void LoadData(sceneTag RegistSceneTag);

	virtual void PreSettingDraw();
	virtual void DrawThis();

	void Update(float fTime);

	void SetPos(float X, float Y);
	float* GetPos();
/*	float GetPosX();
	float GetPosY();*/
	float GetHeight();
	void SetTouchFunction(void(Func)()) { m_fpTouchFunc = Func; }

	void FeverEffectOn();
/*	void FeverEffectReady();
	void FeverEffectCancle();*/
	//bool GetFeverReady() { return m_bFeverReady;}

	virtual void notify();
	/////---------------마리오네트 컨디션------------------------------//
	void SetConditionPos(float X, float Y) { m_vConditionPos[0] = X; m_vConditionPos[1] = Y;}
	static bool AppearedCondition(float* pV2Pos);
	static bool WeavingUpCondition(float* pV2Pos);
	static bool WeavingDownCondition(float* pV2Pos);
	static bool WalkOutCondition(float* pV2Pos);

	//상태 관련 함수
	static void Appeared();
	static void WalkOut();

	// 터치 갈아끼기용 함수
	static void PlayGame_TouchFunc();
	static void Result_TouchFunc();

	void ClearDataStore();
	static void Release();

	static pbMarionette* GetMarionette() {return SingleObject->m_pMarionette;}
	static pbCharacter* GetInstance() { return SingleObject;}

	enum {WALKOUT_SPEED = 600,  WALKOUT_DEST_POS = 1000};
	enum {NONE = -1, APPEARED = 0, WEAVING_UP, WEAVING_DOWN, WALKOUT, DIE, NEXT_SCENE };
private:
	static pbCharacter* SingleObject;

	sceneTag m_RegistSceneTag;

	pbBasicDrawUnit* m_pBodyDrawUnit;
	pbBasicDrawUnit* m_pVehicleDrawUnit;

	float m_fVehiclePosY;

	COLOR_RGBA m_Color;

/*	float m_fLifeRotate;
	float m_fLifePosX;

	enum { FEVER_NONE = 0, FEVER_EXPAND, FEVER_RETURN , FEVER_DISTANCE_READY = 10, FEVER_DISTANCE_EXPLOSION = 100};
	int m_iFeverEffectMode;
	float m_fFeverEffectDistance;
	float m_fFeverTime;
	float m_fFeverDestDistance;
	bool m_bFeverReady;

	GLuint m_EffectVertexIndex;
	GLuint m_EffectUVIndex;

	float m_fFeverTargetTime;
	float m_fEffectScale;
*/

	 void (*m_fpTouchFunc)();
	 /////---------------마리오네트 컨디션------------------------------//
	pbMarionette* m_pMarionette;

	 npV2Vector m_vConditionPos;
};


#endif
