#ifndef PBBOSS_H_
#define PBBOSS_H_

#include "stdafx.h"
//----------------------------------------------pbBoss------------------------------------------------------------------------------------//

class pbMarionette;
class pbBoss : public npDrawable{
private:
	pbBoss();
	~pbBoss();

public:
	static void Create();
	void LoadData();

	void Update(float fTime);

	virtual void PreSettingDraw();
	virtual void DrawThis();

	static void Approaching();
	static void DecreaseHP(float fDamage);

	void ClearDataStore();
	static void Release();

	inline void SetState(int State);
	inline void SetBattlePhase(bool Switch) { m_bBattlePhase = Switch;}
	inline bool IsBattlePhase() { return m_bBattlePhase;}


	/////---------------마리오네트 컨디션------------------------------//
	void SetConditionPos(float X, float Y) { m_vConditionPos[0] = X; m_vConditionPos[1] = Y;}
	static bool ApproachingCondition(float* pV2Pos);
	static bool WeavingUpCondition(float* pV2Pos);
	static bool WeavingDownCondition(float* pV2Pos);
	static bool WalkOutCondition(float* pV2Pos);


	inline static pbBoss* GetInstance() { return SingleObject; }
	inline static pbMarionette* GetMarionette() { return GetInstance()->m_pMarionette;}

	enum {NONE = -1, APPROACHING = 10, WEAVING_UP, WEAVING_DOWN, WALKOUT, DIE};
private:
	static pbBoss* SingleObject;
	pbMarionette* m_pMarionette;
	float m_fHP;

	pbBasicDrawUnit* m_pBodyDrawUnit;

	bool m_bBattlePhase;
	bool m_bBossAlive;

	 /////---------------마리오네트 컨디션------------------------------//
	 npV2Vector m_vConditionPos;
};


#endif
