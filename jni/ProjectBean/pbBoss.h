#ifndef PBBOSS_H_
#define PBBOSS_H_

#include "stdafx.h"
//----------------------------------------------pbHPMeter------------------------------------------------------------------------------------//

class pbHPMeter {
public:
	pbHPMeter();
	~pbHPMeter();

	void Initialize(float CurrentHP, float OneLineHP);

	void SetTag(screenplayTag PanelTag, screenplayTag MeterTag, float fWidth, float fHeight);
	void SetPos(float X, float Y) { m_vPos[0] = X, m_vPos[1] = Y;}
	void SetCurrentHP(float HP, float fOneLineHP);

	void BindLineColor(int Line);

	float GetTotalHP() { return m_fTotalHP;}
	void DecreaseHP(float Damage);
	void AnimationGauge(float fTime);
	bool IsDied() { return m_bDie; }

	void OpenMeter();
	void CloseMeter();

	void Draw();
	void Update(float fTime);

	enum {PHASE_NONE = 0, PHASE_OPEN_HORIZON, PHASE_OPEN_VERTICAL, PHASE_GAUGING, PHASE_CLOSE_HORIZON};
private:
	int m_Phase;
	npV2Vector m_vPos;
	pbBasicDrawUnit* m_pBaseDrawUnit;
	UVPacket* m_pUVPacket;

	////---------------체력 변수-------------------////
	bool m_bDie;
	float m_fOneLineHP;
	float m_fTotalHP;

	////---------------게이지 변수-------------------////
	//--게이지 컨트롤 변수--//
	bool m_bGaugeChangeStart;
	bool m_bGaugeSetting;
	float m_fDecreaseLinePercent;
	int m_iLineCount;	// LineCount = 0 is Last HPLine

	//--선형보간 변수--//
	float m_fAniSpeed;
	float m_fAniTime;
	float m_fDestPercent;
	float m_fStartPercent;
	float m_fDrawPercent;
	float m_fDrawPosX;

	//--Drawing 관련 변수--//
	float m_fHorizonScale;
	float m_fVerticalScale;

	float m_fGaugeHalfWidth;
	float m_fGaugeHalfHeight;

	GLfloat m_GaugeUV[8];
	GLfloat m_GaugeVertex[12];
	GLuint m_GaugeUVBindID;

	float m_GaugeUV_WidthPercent;
};
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
	static void PostDieProcess();
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
	 /////------------------HPMeter------------------------------//
//	 void SetPos_HPMeter(float X, float Y) { m_pHPMeter->SetPos(X, Y);}

	inline static pbBoss* GetInstance() { return SingleObject; }
	inline static pbMarionette* GetMarionette() { return GetInstance()->m_pMarionette;}
	inline static pbHPMeter* GetHPMeter() { return GetInstance()->m_pHPMeter;}

	enum {NONE = -1, APPROACHING = 20, WEAVING_UP, WEAVING_DOWN, WALKOUT, DIE};
private:
	static pbBoss* SingleObject;
	pbMarionette* m_pMarionette;

	pbBasicDrawUnit* m_pBodyDrawUnit;

	bool m_bBattlePhase;
	bool m_bBossAlive;

	 /////---------------마리오네트 컨디션------------------------------//
	 npV2Vector m_vConditionPos;

	 /////------------------HPMeter------------------------------//
	 pbHPMeter* m_pHPMeter;
};


#endif
