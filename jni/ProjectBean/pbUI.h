#ifndef _UI_H
#define _UI_H

#include "stdafx.h"

using namespace projectBean;

class pbUI : public npDrawable{
public:
	pbUI(){ /*m_Color.Init(1.0f, 1.0f, 1.0f, 1.0f);*/ m_Type = UITYPE::UI; m_ID = 0; m_pBaseDrawUnit = NULL; }
	virtual ~pbUI(){};

	virtual void SetID(GLuint ID) {m_ID = ID;}
	virtual void SetPos(float PosX, float PosY) { m_vPos[0] = PosX; m_vPos[1] = PosY;}
	virtual void SetBaseSprite(screenplayTag Tag, float Width, float Height) = 0;

	virtual void PreSettingDraw() = 0;
	virtual void DrawThis() = 0;
	virtual void Update(float fTime) = 0;

	NP_DEFINE_PROPERTY(UITYPE::TYPE, m_Type, Type);
protected:
	GLuint m_ID;
	//COLOR_RGBA m_Color;
	npV2Vector m_vPos;
	pbBasicDrawUnit* m_pBaseDrawUnit;

};

class pbBasicUI : public pbUI{
public:
	pbBasicUI() {};
	virtual ~pbBasicUI(){};

	virtual void SetID(GLuint ID) {m_ID = ID;}
	virtual void SetPos(float PosX, float PosY) { m_vPos[0] = PosX; m_vPos[1] = PosY;}
	virtual void SetBaseSprite(screenplayTag Tag, float Width, float Height) = 0;

	virtual void PreSettingDraw() = 0;
	virtual void DrawThis() = 0;
	virtual void Update(float fTime) = 0;

protected:

};

class pbTouchUI : public pbUI, public iTouchObserver{
public:
	pbTouchUI() {m_bTouched = false; }
	virtual ~pbTouchUI(){};

	virtual void SetID(GLuint ID) {m_ID = ID;}
	virtual void SetPos(float PosX, float PosY) { m_vPos[0] = PosX; m_vPos[1] = PosY;}
	virtual void SetBaseSprite(screenplayTag Tag, float Width, float Height) = 0;

	virtual void PreSettingDraw() = 0;
	virtual void DrawThis() = 0;
	virtual void Update(float fTime) = 0;

	virtual void notify() = 0;


	inline bool IsTouched() { return m_bTouched;}
	inline void ResetTouched() { m_bTouched = false; }
protected:
	bool m_bTouched;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ pbAbilityPower_Indicator------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class pbAbilityPower_Indicator : public pbBasicUI {
public:
	pbAbilityPower_Indicator();
	virtual ~pbAbilityPower_Indicator();

	virtual void SetPos(float PosX, float PosY);
	virtual void SetBaseSprite(screenplayTag Tag, float Width, float Height);
	virtual void SetGaugeSprite(screenplayTag Tag, float Width, float Height);

	virtual void PreSettingDraw();
	virtual void DrawThis();
	virtual void Update(float fTime);

private:
	float m_fMaxAbilityPoint;
	float m_fGaugePercent;
	float m_fGaugePosX;
	float m_fMinusPercent;
	float m_fAniTime;
	float m_fDecreasePercent;
	float m_fDecreasePosX;

	float m_fGaugeHalfWidth;
	float m_fGaugeHalfHeight;

	bool m_bNoHaveGauge;

	GLfloat m_GaugeUV[8];
	GLfloat m_GaugeVertex[12];
	GLuint m_GaugeUVBindID;

	float m_GaugeUV_WidthPercent;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ pbBackPanelUI------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class pbBackPanelUI : public pbBasicUI {
public:
	pbBackPanelUI();
	virtual ~pbBackPanelUI();

	virtual void SetPos(float PosX, float PosY);
	virtual void SetBaseSprite(screenplayTag Tag, float Width, float Height);

	virtual void PreSettingDraw();
	virtual void DrawThis();
	virtual void Update(float fTime);

private:
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ pbButtonUI------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class pbButtonUI : public pbTouchUI{
public:
	pbButtonUI();
	virtual ~pbButtonUI();

	virtual void SetPos(float PosX, float PosY);
	virtual void SetBaseSprite(screenplayTag Tag, float Width, float Height);

	virtual void PreSettingDraw();
	virtual void DrawThis();
	virtual void Update(float fTime);

	virtual void notify();
private:

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------pbScore_Indicator------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class pbScore_Indicator : public pbBasicUI {
public:
	pbScore_Indicator();
	virtual ~pbScore_Indicator();

	virtual void SetBaseSprite(screenplayTag Tag, float Width, float Height);
	virtual void SetScoreSprite(screenplayTag FirstSpriteTag, float Width, float Height);

	virtual void PreSettingDraw();
	virtual void DrawThis();
	virtual void Update(float fTime);

	void DataReset();

	enum{ MAX_DIGITS = 10, NUMBERING = 10 };
private:
	float m_fTextHalfWidth;


	GLfloat m_ScoreVertex[12];
	float m_ScoreWidth;
	GLfloat* m_ScoreUV[NUMBERING];
	GLuint m_ScoreBindID[NUMBERING];	//0~9�� UVIndex
	GLuint m_DigitsNumber[MAX_DIGITS];
	int m_NumberData;


};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////---------------------------------------------------- XML Parser-----------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class XMLParsingBufferCreater {
public:
	XMLParsingBufferCreater();
	~XMLParsingBufferCreater();

	void SetArchive(std::string apkPath);
	char* OpenAssetsByPath(xmlPath path);
	void CloseAssetsAndBuffer(char* buffer);

	static XMLParsingBufferCreater& GetInstance();
private:
	zip* apkArchive;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------pbUIProcessor ------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class pbUIProcessor{
private:
	pbUIProcessor();
	~pbUIProcessor();

public:
	static void Create();
	bool LoadData(const char* filename);

	void CreateUIByType(const char* Type, float X, float Y, screenplayTag BaseTag, screenplayTag ExtraTag);

	pbBasicUI* AddBackPanelUI(float X, float Y, screenplayTag Tag, float Width, float Height);
	pbTouchUI* AddButtonUI(float X, float Y,  screenplayTag Tag, float Width, float Height);
	pbBasicUI* AddAbillityPointUI(float X, float Y,screenplayTag TextTag, float TextWidth, float TextHeight,  screenplayTag GaugeTag, float GaugeWidth, float GaugeHeight);
	pbBasicUI* AddScoreUI(float X, float Y, screenplayTag TextTag, float TextWidth, float TextHeight, screenplayTag ZeroNumberTag, float NumberWidth, float NumberHeight);


	void Update(float time);

	void DeleteUI(pbUI* pUI);
	static void Release();

	void ClearDataStore();

	static  pbUIProcessor* GetInstance() { return SingleObject;}
private:
	static pbUIProcessor* SingleObject;
	typedef  npLinkNode<pbUI*> UIList;
	UIList* m_ControledUIStore;

	int m_UICounts;

	void registControled(pbUI* pUI);
	void removeControled(pbUI* pUI);

};

#endif
