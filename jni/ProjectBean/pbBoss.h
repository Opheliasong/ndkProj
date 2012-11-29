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

};


#endif
