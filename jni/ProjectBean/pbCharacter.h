#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "stdafx.h"

using namespace projectBean;

class pbCharacter : public iTouchObserver{
private:
	pbCharacter();
	~pbCharacter();

	void SetVertexIndex(GLuint BodyIndex, GLuint LifeIndex, GLuint EffectIndex);
	void SetUVIndex(GLuint BodyIndex, GLuint LifeIndex, GLuint EffectIndex);
public:
	static void Create();
	void LoadData();
	void Draw();

	void Update(float fTime);

	void SetPos(float X, float Y){ m_vBodyPos[0] = X; m_vBodyPos[1] = Y; }
	void DecreaseLife();

	void FeverEffectOn();
	void FeverEffectReady();
	void FeverEffectCancle();
	bool GetFeverReady() { return m_bFeverReady;}

	inline float GetPosX() {return m_vBodyPos[0]; }
	inline float GetPosY() {return m_vBodyPos[1]; }
	inline float* GetPos() {return m_vBodyPos;}

	void ClearDataStore();
	static void Release();


	virtual void notify();



	static pbCharacter* GetInstance() { return SingleObject;}
private:
	static pbCharacter* SingleObject;

	COLOR_RGBA m_Color;

	GLuint m_BodyVertexIndex; //�ٵ��� ���ؽ����� index
	GLuint m_BodyUVIndex; //�ٵ��� UV ���� index

	npV2Vector m_vBodyPos;

	float m_fBodyWidth;
	float m_fBodyHeight ;

	GLuint m_LifeVertexIndex; // �������� ���ؽ����� index
	GLuint m_LifeUVIndex; //�������� UV ���� index

//	GLfloat** m_ppfFollowLineVertexPointer;
//	float m_fLerp;

	float m_fLifeRotate;
	float m_fLifePosX;

	enum { FEVER_NONE = 0, FEVER_EXPAND, FEVER_RETURN , FEVER_DISTANCE_READY = 10, FEVER_DISTANCE_EXPLOSION = 100};
	int m_iFeverEffectMode;
	float m_fFeverEffectDistance;
	float m_fFeverTime;
	float m_fFeverDestDistance;
	bool m_bFeverReady;


	GLuint m_EffectVertexIndex;
	GLuint m_EffectUVIndex; //����Ʈ

	float m_fFeverTargetTime;
	float m_fEffectScale;

};


//------------------------------------------�޺� �Ŵ���------------------------------------------------------//
/*
class pbComboManager : public pbBasicUI {
public:
	pbComboManager();
	~pbComboManager();

	static void Create();
	void LoadData();
	virtual void Draw();

	virtual void Update(float fTime);

	void IncreaseCombo(int Score);

	void ResetCombo();

	bool FeverOn();

	void ClearDataStore();
	static void Release();

	void SetVertexIndex(GLuint BodyIndex);
	void SetUVIndex(GLuint StartNumberUVIndex);

	void DataReset();

	inline int GetFever() { return m_iFever;}

	static pbComboManager* GetInstance() {return SingleObject; }

	enum{ MAX_DIGITS = 3, NUMBERING = 10 };
private:
	static pbComboManager* SingleObject;

	GLuint m_iCombo;
	GLuint m_iFever;
	GLuint m_iNextFeverCombo;

	float m_PlacementWidth;
	float m_fTextPlacementWidth;
	GLuint m_NumberUVIndex[NUMBERING];	//0~9�� UVIndex
	GLuint m_FeverNumberUVIndex[NUMBERING];	//0~9�� UVIndex
	GLuint m_DigitsNumber[MAX_DIGITS];
	GLuint m_CurrentDigits;

	GLuint m_FeverBodyVertexIndex;
	GLuint m_FeverBodyUVIndex;
	GLuint m_FeverNumberVertexIndex;
	GLuint m_NumberVertexIndex;


};*/



#endif
