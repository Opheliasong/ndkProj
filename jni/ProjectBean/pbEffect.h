#ifndef _EFFECT_H
#define _EFFECT_H

#include "stdafx.h"

using namespace projectBean;


class pbEffect {
public:
	pbEffect():m_BodyUVIndex(0), m_BodyVertexIndex(0), m_bAlive(true), m_bInfinite(false), m_fLifeTime(0.0f), m_fScale(1.0f){};
	virtual ~pbEffect(){};

	virtual void SetInfinite(bool bInfinite){ m_bInfinite = bInfinite;}
	virtual void SetAlive(bool Alive) { m_bAlive = Alive;}
	virtual bool GetAlive() { /*if(m_bAlive)LOGE("ALIVE");else LOGE("DEAD");*/ return m_bAlive;}

	virtual void SetPos(float PosX, float PosY) { m_vPos[0] = PosX; m_vPos[1] = PosY; }
	virtual float* GetVPos() { return m_vPos;}

	virtual void SetBodyUVIndex(GLuint UVIndex) { m_BodyUVIndex = UVIndex;}

	virtual void SetScale(float fScale) { m_fScale = fScale;}

	virtual void Draw(){};

	virtual void Update(float fTime){};



	EFFECTTYPE::TYPE m_Type;
	int m_RentalIndex;
protected:
	GLuint m_BodyVertexIndex;
	GLuint m_BodyUVIndex;

	npV2Vector m_vPos;

	float m_fLifeTime;
	bool m_bInfinite;
	bool m_bAlive;

	float m_fScale;
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
	~pbStepUpEffect();

	void Initialize(GLuint VertexIndex, GLuint UVIndex, float LifeTime, bool Infinite);

	void DataReset();

	void SetMaxLifeTime(float fLifeTime);

	void Draw();

	void Update(float fTime);

private:
	float m_fMaxLifeTime;

	//----�г� ����-----//
//	bool m_bPanelDrawing;
	int m_iPanelPhase;
	float m_fPanelScale;
	float m_fPanelLerp;
	float m_PanelPosX;

	//----���ܾ� ����-----//
	GLuint m_StepUpVertexIndex;
	GLuint m_StepUpUVIndex;
	GLuint m_StepUpAniCount;

	int m_iStepUpPhase;
	float m_fStepUpLerp;
	float m_fStepUpScale;
	float m_fStepUpPosX;
	float m_fStepUpAniTime;
//	bool m_bStepUpDrawing;
	//----�� ����-----//
	GLuint m_LabelVertexIndex;
	GLuint m_LabelUVIndex;

	GLuint m_LabelTextVertexIndex;
	GLuint m_LabelTextUVIndex;

	bool m_bLabelDrawing;
	float m_fLabelWidth;
	float m_fLabelPosX;
	int m_iLabelPhase;
	float m_fLabelLerp;

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ ���ܾ� ����Ʈ------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class pbFeverAvailableEffect : public pbEffect{
public:
	pbFeverAvailableEffect();
	~pbFeverAvailableEffect();

	void Initialize(GLuint VertexIndex, GLuint UVIndex, float LifeTime, bool Infinite);

	void DataReset();

	void SetMaxLifeTime(float fLifeTime);

	void Draw();

	void Update(float fTime);

private:
	float m_fMaxLifeTime;
	COLOR_RGBA m_Color;

	//----�г� ����-----//
//	bool m_bPanelDrawing;
	int m_iPanelPhase;
	float m_fPanelScale;
	float m_fPanelLerp;
	float m_PanelPosX;

	//----���ܾ� ����-----//
	GLuint m_StepUpVertexIndex;
	GLuint m_StepUpUVIndex;
	GLuint m_StepUpAniCount;

	int m_iStepUpPhase;
	float m_fStepUpLerp;
	float m_fStepUpScale;
	float m_fStepUpPosX;
	float m_fStepUpAniTime;
//	bool m_bStepUpDrawing;
	//----�� ����-----//
/*	GLuint m_LabelVertexIndex;
	GLuint m_LabelUVIndex;

	GLuint m_LabelTextVertexIndex;
	GLuint m_LabelTextUVIndex;

	bool m_bLabelDrawing;
	float m_fLabelWidth;
	float m_fLabelPosX;
	int m_iLabelPhase;
	float m_fLabelLerp;*/

};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ �޺� ����Ʈ------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
class pbComboEffect : public pbEffect{
public:
	pbComboEffect();
	~pbComboEffect();

	void Initialize(GLuint VertexIndex, GLuint UVIndex, float LifeTime, bool Infinite);

	void SetCombo(int Combo);
	void DataReset();

	void Draw();

	void Update(float fTime);

	enum{ MAX_DIGITS = 3, NUMBERING = 10 };
private:
	float m_PlacementWidth;
	GLuint m_NumberUVIndex[NUMBERING];	//0~9�� UVIndex
	GLuint m_DigitsNumber[MAX_DIGITS];
	GLuint m_CurrentDigits;

	float m_fTextHalfWidth;
	float m_fTextHalfHeight;
};
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ ��ƼĿ ����Ʈ (UV ��������)------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class pbStickerEffect : public pbEffect{
public:
	pbStickerEffect();
	~pbStickerEffect();

	void Initialize(GLuint VertexIndex, GLuint UVIndex, float LifeTime, bool Infinite);

	void Draw();

	void SetWidth(float Width) { m_fScaleX = Width;}
	void SetHeight(float Height) { m_fScaleY = Height;}

	void Update(float fTime);

public:
	COLOR_RGBA m_Color;
private:

	float m_fScaleX;
	float m_fScaleY;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ �����̻��� ����Ʈ------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class pbHomingMissileEffect : public pbEffect{
public:
	pbHomingMissileEffect();
	~pbHomingMissileEffect();

	void Initialize(GLuint VertexIndex, GLuint UVIndex, float LifeTime, bool Infinite);

	void Draw();

	void SetWidth(float Width) { m_fScaleX = Width;}
	void SetHeight(float Height) { m_fScaleY = Height;}

	void Update(float fTime);

	void SetMissileCurve(float* pV2Origin, float* pV2Destination);

	void SetDecreaseHPFunc(void (DecreaseHP)(float fDamage) ) { m_fpDecreaseHP = DecreaseHP; }
	void SetDamage(float fDamage) { m_fDamage = fDamage;}

public:
	COLOR_RGBA m_Color;
private:
	npV2Vector m_vOrigin;
	npV2Vector m_vDestination;

	float m_fScaleX;
	float m_fScaleY;

	float m_fBezierLerp;

	BEZIER_2POINT m_Bezier_Start;

	BEZIER_2POINT m_Bezier_1_1;
//	BEZIER_2POINT m_Bezier_1_2;
	BEZIER_2POINT m_Bezier_2_1;
//	BEZIER_2POINT m_Bezier_2_2;
	BEZIER_2POINT m_Bezier_3_1;
//	BEZIER_2POINT m_Bezier_3_2;
	BEZIER_2POINT m_Bezier_4_1;
//	BEZIER_2POINT m_Bezier_4_2;
	BEZIER_2POINT m_Bezier_0_1;
//	BEZIER_2POINT m_Bezier_0_2;

	BEZIER_2POINT m_Bezier_End;

	npV2Vector m_vPos_1;
	npV2Vector m_vPos_2;
	npV2Vector m_vPos_3;
	npV2Vector m_vPos_4;

	 void (*m_fpDecreaseHP)(float fDamage);
	 float m_fDamage;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ ����Ʈ �Ŵ���------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class pbEffectProcess{
	pbEffectProcess();
	~pbEffectProcess();

	void registControled(pbEffect* pEffect);
	void removeControled(pbEffect* pEffect);

public:
	static void Create();

	void AddStepUpEffect(float X, float Y, GLuint StartVertexIndex, GLuint StartUVIndex, float fScale);
	void AddFeverAvailableEffect(float X, float Y, GLuint StartVertexIndex, GLuint StartUVIndex, float fScale);
	void AddStickerEffect(float X, float Y, GLuint StartUVIndex, float fScale);
	void AddHomingMissileEffect(float fStartX, float fStartY, float fDestX, float fDestY, GLuint StartUVIndex, float LifeTime,float fDamage, void (*DecreaseHP)(float fDamage) );
	void AddMissEffect();

	void Update(float fTime);
	void ClearDataStore();
	static void Release();

	// �����ε�
	void RemoveEffectAndReturningMemory(pbStickerEffect* pEffect);
	void RemoveEffectAndReturningMemory(pbStepUpEffect* pEffect);
	void RemoveEffectAndReturningMemory(pbFeverAvailableEffect* pEffect);
	void RemoveEffectAndReturningMemory(pbHomingMissileEffect* pEffect);

	static pbEffectProcess* GetInstance() { return SingleObject; }
private:
	static pbEffectProcess* SingleObject;

	npLinkNode<pbEffect>* m_pEffectLinkHeader;

	pbMemoryRentalUnit<pbStickerEffect>* m_pStickerEffectRentalUnit;
	pbMemoryRentalUnit<pbStepUpEffect>* m_pStepUpEffectRentalUnit;
	pbMemoryRentalUnit<pbFeverAvailableEffect>* m_pFeverAvailableEffectRentalUnit;
	pbMemoryRentalUnit<pbHomingMissileEffect>* m_pHomingMissileEffectRentalUnit;

};
#endif
