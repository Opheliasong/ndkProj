#ifndef _UI_H
#define _UI_H

#include "../stdafx.h"

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
	pbTouchUI() {m_fpTouchFunc = NULL; }
	virtual ~pbTouchUI(){};

	void SetTouchFunc(void(Func)() ) {m_fpTouchFunc = Func; }
	virtual void SetID(GLuint ID) {m_ID = ID;}
	virtual void SetPos(float PosX, float PosY) { m_vPos[0] = PosX; m_vPos[1] = PosY;}
	virtual void SetBaseSprite(screenplayTag Tag, float Width, float Height) = 0;

	virtual void PreSettingDraw() = 0;
	virtual void DrawThis() = 0;
	virtual void Update(float fTime) = 0;

	virtual void notify() = 0;

protected:
	void (*m_fpTouchFunc)();
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ pbAbilityPower_Indicator------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class pbGauge_Indicator : public pbBasicUI {
public:
	pbGauge_Indicator();
	virtual ~pbGauge_Indicator();

	virtual void SetPos(float PosX, float PosY);
	virtual void SetBaseSprite(screenplayTag Tag, float Width, float Height);
	virtual void SetGaugeSprite(screenplayTag Tag, float Width, float Height);

	void SetGaugeReturnFunc(float(Func)() ) { m_fpGaugeReturnFunc = Func;}
	void SetMaxGaugePoint(float fMaxGauge) { m_fMaxGaugePoint = fMaxGauge;}

	void SetRelativePos(float* pV2Pos);

	virtual void PreSettingDraw();
	virtual void DrawThis();
	virtual void Update(float fTime);

private:
	float m_fMaxGaugePoint;

	float m_fAniTime;

	float m_fDestPercent;
	float m_fStartPercent;
	float m_fDrawPercent;
	float m_fDrawPosX;

	float m_fGaugeHalfWidth;
	float m_fGaugeHalfHeight;

	bool m_bGaugeChangeStart;

	GLfloat m_GaugeUV[8];
	GLfloat m_GaugeVertex[12];
	GLuint m_GaugeUVBindID;

	float m_GaugeUV_WidthPercent;

	float (*m_fpGaugeReturnFunc)();

	float* m_pV2RelativePos;
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
class pbNumber_Indicator : public pbBasicUI {
public:
	pbNumber_Indicator();
	virtual ~pbNumber_Indicator();

	virtual void SetBaseSprite(screenplayTag Tag, float Width, float Height);
	virtual void SetScoreSprite(screenplayTag FirstSpriteTag, float Width, float Height);

	void SetShowDigits(int ShowDigits);
	void SetNumberReturnFunc(int(Func)());

	virtual void PreSettingDraw();
	virtual void DrawThis();
	virtual void Update(float fTime);

	void DataReset();

	enum{ MAX_DIGITS = 10, NUMBERING = 10 };
private:
	float m_fTextHalfWidth;

	GLfloat m_ScoreVertex[12];
	float m_ScoreWidth;
	UVPacket* m_ScoreUV[NUMBERING];
	GLuint m_DigitsNumber[MAX_DIGITS];
	int m_CurrentDigits;

	int(*m_fpNumberReturnFunc)();	// 넘버리턴 함수는 숫자 갱신 필요가 없을때 -1을 리턴하도록 한다
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
	pbTouchUI* AddButtonUI(float X, float Y,  screenplayTag Tag, float Width, float Height, void(Func)());
	pbBasicUI* AddGaugeUI(float X, float Y, screenplayTag GaugePanelTag, screenplayTag GaugeTag, float GaugeWidth, float GaugeHeight, float MaxGauge, float(GaugeReturnFunc)());
	pbBasicUI* AddGaugeUI_RelativePos(float* pV2RelativePos, float X, float Y, screenplayTag GaugePanelTag, screenplayTag GaugeTag, float GaugeWidth, float GaugeHeight, float MaxGauge, float(GaugeReturnFunc)());//위치따라가는 UI
	pbBasicUI* AddNumberUI(float X, float Y, screenplayTag TextTag, float TextWidth, float TextHeight, screenplayTag ZeroNumberTag, float NumberWidth, float NumberHeight, int MaxDigits, int(NumberRetunFunc)());


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
