#ifndef PBBOSS_H_
#define PBBOSS_H_

#include "../stdafx.h"

//----------------------------------------------pbBoss------------------------------------------------------------------------------------//
class pbMarionette;
class pbHPMeter;
class pbTargetStamp;
class pbBoss : public npDrawable, public iTouchObserver{
private:
	pbBoss();
	~pbBoss();

public:
	static void Create();
	void LoadData();

	virtual void PreSettingDraw();
	virtual void DrawThis();

	void Update(float fTime);

	void ClearDataStore();
	static void Release();

	inline void SetState(int State);
	inline void SetBattlePhase(bool Switch) { m_bBattlePhase = Switch;}
	inline bool IsBattlePhase() { return m_bBattlePhase;}

	////------------------------------터치----------------------------------------------//
	virtual void notify();
	static void Game_TouchFunc();
	void SetTouchFunc(void(Func)() ) { m_fpTouchFunc = Func;}

	/////---------------마리오네트 컨디션------------------------------//
	void SetConditionPos(float X, float Y) { m_vConditionPos[0] = X; m_vConditionPos[1] = Y;}
	static bool ApproachingCondition(float* pV2Pos);
	static bool WeavingUpCondition(float* pV2Pos);
	static bool WeavingDownCondition(float* pV2Pos);
	static bool WalkOutCondition(float* pV2Pos);

	/////---------------스테이지 트리거용 함수------------------------------//
	static void Approaching();
	static void PostDieProcess();

	/////---------------HP감소 정적함수------------------------------//
	static void DecreaseHP(float fDamage);

	////---------------------------인스턴스 접근-------------------------------------//
	inline static pbBoss* GetInstance() { return SingleObject; }
	inline static pbMarionette* GetMarionette() { return GetInstance()->m_pMarionette;}
	inline static pbHPMeter* GetHPMeter() { return GetInstance()->m_pHPMeter;}
	inline static pbTargetStamp* GetTargetStamp() { return GetInstance()->m_pTargetStamp;}

	enum {NONE = -1, APPROACHING = 20, WEAVING_UP, WEAVING_DOWN, WALKOUT, DIE};
private:
	static pbBoss* SingleObject;
	pbBasicDrawUnit* m_pBodyDrawUnit;

	bool m_bBattlePhase;
	bool m_bBossAlive;

	////------------------------------터치----------------------------------------------//
	void(*m_fpTouchFunc)();

	 /////---------------마리오네트 컨디션------------------------------//
	pbMarionette* m_pMarionette;
	 npV2Vector m_vConditionPos;

	 /////------------------HPMeter------------------------------//
	 pbHPMeter* m_pHPMeter;

	 /////------------------pbTargetStamp------------------------------//
	 pbTargetStamp* m_pTargetStamp;
};


#endif
