#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "stdafx.h"

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

	inline void SetPos(float X, float Y);
	inline float GetPosX();
	inline float GetPosY();

	void DecreaseLife();
	void FeverEffectOn();
	void FeverEffectReady();
	void FeverEffectCancle();
	bool GetFeverReady() { return m_bFeverReady;}

	void ClearDataStore();
	static void Release();

	//상태 관련
	static void Appeared();
	static void WalkOut();

	virtual void notify();

	enum {NONE = -1, APPEARED = 0, WEAVING_UP, WEAVING_DOWN, WALKOUT, DIE };

	static pbCharacter* GetInstance() { return SingleObject;}
private:
	static pbCharacter* SingleObject;

	pbMarionette* m_pMarionette;

	sceneTag m_RegistSceneTag;

	pbBasicDrawUnit* m_pBodyDrawUnit;
	pbBasicDrawUnit* m_pSatelliteDrawUnit;

	COLOR_RGBA m_Color;

	float m_fLifeRotate;
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

};


//------------------------------------------�޺� �Ŵ���------------------------------------------------------//

class pbComboManager : public npDrawable {
public:
	pbComboManager();
	~pbComboManager();

	static void Create();
	void LoadData();

	virtual void PreSettingDraw();
	virtual void DrawThis();

	void IncreaseCombo(int Score);

	void ResetCombo();

	bool FeverOn();

	void ClearDataStore();
	static void Release();

	void SetTextTag(screenplayTag NormalTextTag, screenplayTag FeverTextTag, float fWidth, float fHeight);
	void SetNumberTag(screenplayTag NormalNumberTag, screenplayTag FeverNumberTag, float fWidth, float fHeight);

	void DataReset();

	void SetPos(float fPosX, float fPosY) { m_vPos[0] = fPosX; m_vPos[1] = fPosY; }

	inline int GetFever() { return m_iFever;}

	static pbComboManager* GetInstance() {return SingleObject; }

	enum{ MAX_DIGITS = 3, NUMBERING = 10 };
private:
	static pbComboManager* SingleObject;

	npV2Vector m_vPos;

	screenplayTag m_NormalTextTag;
	screenplayTag m_FeverTextTag;
	pbBasicDrawUnit* m_pTextDrawUnit;
	pbBasicDrawUnit* m_pFeverTextDrawUnit;

	GLfloat m_NumberVertex[12];

	float m_PlacementWidth;
	float m_fTextPlacementWidth;
	UVPacket* m_NumberUVPacket[NUMBERING];	//0~9�� UVIndex
	UVPacket* m_FeverNumberUVPacket[NUMBERING];	//0~9�� UVIndex
	GLuint m_DigitsNumber[MAX_DIGITS];
	GLuint m_CurrentDigits;

	GLuint m_iCombo;
	GLuint m_iFever;
	GLuint m_iNextFeverCombo;
};



#endif
