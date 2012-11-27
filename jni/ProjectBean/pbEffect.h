#ifndef _EFFECT_H
#define _EFFECT_H

#include "stdafx.h"

using namespace projectBean;


class pbEffect : public npDrawable{
public:
	pbEffect():m_bAlive(true), m_bInfinite(false), m_fLifeTime(0.0f), m_RentalIndex(-1), m_Type(EFFECTTYPE::NONE) /*,m_fScale(1.0f) */ {};
	virtual ~pbEffect(){};

	virtual void SetInfinite(bool bInfinite){ m_bInfinite = bInfinite;}
	virtual void SetAlive(bool Alive) { m_bAlive = Alive;}
	virtual bool GetAlive() { /*if(m_bAlive)LOGE("ALIVE");else LOGE("DEAD");*/ return m_bAlive;}

	virtual void SetPos(float PosX, float PosY) { m_vPos[0] = PosX; m_vPos[1] = PosY; }
	virtual float* GetVPos() { return m_vPos;}

//	virtual void SetScale(float fScale) { m_fScale = fScale;}

	virtual void PreSettingDraw() = 0;
	virtual void DrawThis()=0;

	virtual void Update(float fTime)=0;


	inline void SetRegistedSceneTag(sceneTag sceneTag) { m_RegistedScene = sceneTag;}

	EFFECTTYPE::TYPE m_Type;
	int m_RentalIndex;
protected:
	sceneTag m_RegistedScene;
	npV2Vector m_vPos;

	float m_fLifeTime;
	bool m_bInfinite;
	bool m_bAlive;

//	float m_fScale;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ ���� ����Ʈ------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*class pbJudgementEffect : public pbEffect{
public:
	pbJudgementEffect();
	~pbJudgementEffect();

	void Initialize(GLuint VertexIndex, GLuint UVIndex, float LifeTime, bool Infinite);

	void Draw();

	void Update(float fTime);

private:
};*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ ���ܾ� ����Ʈ------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class pbStepUpEffect : public pbEffect{
public:
	pbStepUpEffect();
	virtual ~pbStepUpEffect();

	void Initialize(screenplayTag PanelTag, float PanelWidth, float PanelHeight,
			screenplayTag LabelTextTag, float LabelTextWidth, float LabelTextHeight,
			screenplayTag LabelTag, float LabelWidth, float LabelHeight,
			screenplayTag StepUpTag, float StepUpWidth, float StepUpHeight );

	void DataReset();

	void SetMaxLifeTime(float fLifeTime);

	virtual void PreSettingDraw();
	virtual void DrawThis();

	virtual void Update(float fTime);

private:
	float m_fMaxLifeTime;
	pbBasicDrawUnit* m_pPanelDrawUnit;
	pbBasicDrawUnit* m_pLabelDrawUnit;
	pbBasicDrawUnit* m_pLabelTextDrawUnit;
	pbBasicDrawUnit* m_pStepUpDrawUnit;

	//----�г� ����-----//
	int m_iPanelPhase;
	float m_fPanelScale;
	float m_fPanelLerp;
	float m_fPanelPosX;

	//----���ܾ� ����-----//

	int m_iStepUpPhase;
	float m_fStepUpLerp;
	float m_fStepUpScale;
	float m_fStepUpPosX;
	float m_fStepUpAniTime;
//	bool m_bStepUpDrawing;

	//----�� ����-----//
	bool m_bLabelDrawing;
	float m_fLabelTextWidth;
	float m_fLabelTextPosX;
	int m_iLabelPhase;
	float m_fLabelLerp;

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ ��ƼĿ ����Ʈ (UV ��������)------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class pbStickerEffect : public pbEffect{
public:
	pbStickerEffect();
	virtual ~pbStickerEffect();

	void Initialize(screenplayTag BaseTag, float fWidth, float fHeight, float LifeTime, bool Infinite);

	virtual void PreSettingDraw();
	virtual void DrawThis();

	virtual void Update(float fTime);

	inline void SetColor(float R, float G, float B, float A) { m_Color.Init(R, G, B, A); }

private:
	COLOR_RGBA m_Color;
	pbBasicDrawUnit* m_pDrawUnit;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ �����̻��� ����Ʈ------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class pbHomingMissileEffect : public pbEffect{
public:
	pbHomingMissileEffect();
	virtual ~pbHomingMissileEffect();

	void Initialize(screenplayTag MissileTag, float fWidth, float fHeight, float LifeTime, bool Infinite);

	virtual void PreSettingDraw();
	virtual void DrawThis();

	virtual void Update(float fTime);

	void SetMissileCurve(float* pV2Origin, float* pV2Destination);

	void SetDecreaseHPFunc(void (DecreaseHP)(float fDamage) ) { m_fpDecreaseHP = DecreaseHP; }
	void SetDamage(float fDamage) { m_fDamage = fDamage;}

private:
	npV2Vector m_vOrigin;
	npV2Vector m_vDestination;

	pbBasicDrawUnit* m_pMissileDrawUnit;

	float m_fBezierLerp;

	BEZIER_2POINT m_Bezier_Start;

	BEZIER_2POINT m_Bezier_0_1;
	BEZIER_2POINT m_Bezier_1_1;
	BEZIER_2POINT m_Bezier_2_1;
	BEZIER_2POINT m_Bezier_3_1;

	BEZIER_2POINT m_Bezier_End;

	enum {MAX_MISSILE = 4};
	float* m_pPosArray[MAX_MISSILE];
	npV2Vector m_vPos_1;
	npV2Vector m_vPos_2;
	npV2Vector m_vPos_3;
//	npV2Vector m_vPos_4;

	 void (*m_fpDecreaseHP)(float fDamage);
	 float m_fDamage;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ ����Ʈ �Ŵ���------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class pbEffectManager{
	pbEffectManager();
	~pbEffectManager();

	void registControled(pbEffect* pEffect);
	void removeControled(pbEffect* pEffect);

public:
	static void Create();

	inline void SetSceneTag(sceneTag SceneTag) { m_SceneTag = SceneTag;}

	void AddStepUpEffect(float X, float Y, screenplayTag PanelTag, float PanelWidth, float PanelHeight,		screenplayTag LabelTextTag, float LabelTextWidth, float LabelTextHeight,
															screenplayTag LabelTag, float LabelWidth, float LabelHeight,		screenplayTag StepUpTag, float StepUpWidth, float StepUpHeight );
	void AddStickerEffect(float X, float Y, screenplayTag BaseTag, float fWidth, float fHeight);
	void AddHomingMissileEffect(float fStartX, float fStartY, float fDestX, float fDestY, screenplayTag MissileTag, float fWidth, float fHeight, float LifeTime,float fDamage, void (*DecreaseHP)(float fDamage) );
	void AddMissEffect();

	void Update(float fTime);
	void ClearDataStore();
	static void Release();

	// �����ε�
	void RemoveEffectAndReturningMemory(pbStickerEffect* pEffect);
	void RemoveEffectAndReturningMemory(pbStepUpEffect* pEffect);
	void RemoveEffectAndReturningMemory(pbHomingMissileEffect* pEffect);

	static pbEffectManager* GetInstance() { return SingleObject; }
private:
	static pbEffectManager* SingleObject;

	sceneTag m_SceneTag;

	typedef npLinkNode<pbEffect*> EffectList;
	EffectList* m_pEffectLinkHeader;

	pbMemoryRentalUnit<pbStickerEffect>* m_pStickerEffectRentalUnit;
	pbMemoryRentalUnit<pbStepUpEffect>* m_pStepUpEffectRentalUnit;
	pbMemoryRentalUnit<pbHomingMissileEffect>* m_pHomingMissileEffectRentalUnit;

};
#endif
