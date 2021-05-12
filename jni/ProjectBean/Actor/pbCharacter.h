#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "../stdafx.h"

using namespace projectBean;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////----------------------------------------------------		pbCharacter		------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class pbMarionette;
class pbCharacter : public npDrawable, public iTouchObserver{
private:
	pbCharacter();
	~pbCharacter();

public:
	/////---------------일반------------------------------//
	static void Create();
	void LoadData(sceneTag RegistSceneTag);
	void ClearDataStore();
	static void Release();

	virtual void PreSettingDraw();
	virtual void DrawThis();

	void Update(float fTime);

	////--------------------------------------Get&Set----------------------------------------//
	void SetPos(float X, float Y);
	float* GetPos();
	float GetHeight();

	void SetVehicleTagData(TAGDATA& TagData);
	////---------------------------------------피버-----------------------------------------//
	void FeverEffectOn();

	////----------------------------------------Touch-----------------------------------------//
	virtual void notify();

	void SetTouchFunction(void(Func)()) { m_fpTouchFunc = Func; }
	static void PlayGame_TouchFunc();
	static void Result_TouchFunc();
	static void Shop_TouchFunc();


	/////---------------마리오네트 컨디션------------------------------//
	void SetConditionPos(float X, float Y) { m_vConditionPos[0] = X; m_vConditionPos[1] = Y;}
	static bool AppearedCondition(float* pV2Pos);
	static bool WeavingUpCondition(float* pV2Pos);
	static bool WeavingDownCondition(float* pV2Pos);
	static bool WalkOutCondition(float* pV2Pos);

	/////---------------스테이지 트리거 ------------------------------//
	static void Appeared();
	static void WalkOut();

	////-----------------------------------인스턴스-------------------------------------//
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

	 void (*m_fpTouchFunc)();
	 /////---------------마리오네트 컨디션------------------------------//
	pbMarionette* m_pMarionette;
	 npV2Vector m_vConditionPos;


	 //float m_fLifeRotate;
};


#endif
