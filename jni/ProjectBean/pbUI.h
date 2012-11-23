#ifndef _UI_H
#define _UI_H

#include "stdafx.h"

using namespace projectBean;

////------------------------------------------------------ UI �������̽�------------------------------------------------------------------------------///////
class pbInterfaceUI {
	void registLinkNode(pbInterfaceUI* pUI);
	void removeLinkNode(pbInterfaceUI* pUI);
public:
	pbInterfaceUI();
	virtual ~pbInterfaceUI();

	void AddChildUnit(pbInterfaceUI* pChild);
	void DeleteChildUnit(pbInterfaceUI* pChild);

	void Draw();
	virtual void Update(float fTime) = 0;

	virtual void SetV2Pos(float X, float Y);
	inline void SetType(UITYPE::TYPE Type) { m_Type = Type; }
	/*inline void SetV2Pos(float X, float Y) { m_vPos[iX] = X; m_vPos[iY] = Y; }*/

	inline void SetV2ParentPos(float X, float Y) { m_vParentPos[iX] = X; m_vParentPos[iY] = Y; }

	inline pbDrawUnit* GetBaseDrawUnit() { return m_pDrawUnit_Base;}
	inline UITYPE::TYPE GetType() { return m_Type; }
//	inline pbLinkNode<pbInterfaceUI>* GetUIListHeader() { return m_pUIListHeader; }

private:
	UITYPE::TYPE m_Type;
	pbDrawUnit* m_pDrawUnit_Base;
	npLinkNode<pbInterfaceUI>* m_pUIListHeader;
	npV2Vector m_vPos;
	npV2Vector m_vParentPos;	//��ġ�� ���� ����� ��ġ
};

typedef npLinkNode<pbInterfaceUI> pbUIList;

////------------------------------------------------------ ��ġ �Ұ� UI------------------------------------------------------------------------------///////
class pbUI_Untouchable : public pbInterfaceUI{
public:
	pbUI_Untouchable();
	virtual ~pbUI_Untouchable();

	virtual void Update(float fTime) = 0;
private:
};

////------------------------------------------------------ ��ġ ���� UI------------------------------------------------------------------------------///////
class pbUI_Touchable : public pbInterfaceUI, public iTouchObserver{
public:
	pbUI_Touchable();
	virtual ~pbUI_Touchable();

	virtual void SetV2Pos(float X, float Y);

	virtual void Update(float fTime) = 0;

	virtual void notify() = 0;
private:
	npSizef_WH m_pHalfWH;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ Base UI------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class pbBaseUI : public pbUI_Untouchable {
public:
	pbBaseUI();
	virtual ~pbBaseUI();

	virtual void Update(float fTime);

private:
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ TouchBase UI------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class pbTouchBaseUI : public pbUI_Touchable{
public:
	pbTouchBaseUI();
	virtual ~pbTouchBaseUI();

	void SetTouchActFunction(void (fpAction)() ) { m_fpTouchAction = fpAction; }

	virtual void Update(float fTime);

	virtual void notify(int x, int y, TOUCHSTATUS::TYPE Touchstatus);
private:
	void (*m_fpTouchAction)();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ ����ġ UI------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ѹ� ��ġ�ϸ� �����Ǵ� UI
class pbOneTouchUI : public pbUI_Touchable{
public:
	pbOneTouchUI();
	virtual ~pbOneTouchUI();

	virtual void Update(float fTime);

	virtual void notify(int x, int y, TOUCHSTATUS::TYPE Touchstatus);

	bool IsTouched() { return m_bTouched; }
private:
	bool m_bTouched;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ ������ UI------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ѹ� ��ġ�ϸ� �����Ǵ� UI
class pbGaugeUI : public pbUI_Untouchable{
public:
	pbGaugeUI();
	virtual ~pbGaugeUI();

	virtual void Update(float fTime);

private:
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ �ֹ��� UI------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*class pbAbilityPower_Indicator : public pbUI {
public:
	pbAbilityPower_Indicator();
	virtual ~pbAbilityPower_Indicator();
	virtual void Init();

	virtual void SetPos(float PosX, float PosY);
	virtual void SetVertexIndex(GLuint BodyIndex, GLuint TextIndex);
	virtual void SetUVIndex(GLuint BodyIndex, GLuint TextIndex);

	virtual void Draw();
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
	float m_fTextHalfWidth;
	float m_fTextHalfHeight;

	bool m_bNoHaveGauge;

	GLuint m_TextVertexIndex;
	GLuint m_TextUVIndex;

	GLfloat m_GaugeUV[8];
	float m_GaugeUV_WidthPercent;
};*/


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ ���� ���------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*class pbScore_Indicator : public pbUI {
public:
	pbScore_Indicator();
	virtual ~pbScore_Indicator();
	virtual void Init();

	virtual void SetVertexIndex(GLuint BodyIndex, GLuint TextIndex);
	virtual void SetUVIndex(GLuint StartNumberUVIndex, GLuint TextIndex);

	virtual void Draw();

	virtual void Update(float fTime);

	void DataReset();

	enum{ MAX_DIGITS = 10, NUMBERING = 10 };
private:
	float m_PlacementWidth;
	GLuint m_NumberUVIndex[NUMBERING];	//0~9�� UVIndex
	GLuint m_DigitsNumber[MAX_DIGITS];
	int m_NumberData;

	float m_fTextHalfWidth;
	float m_fTextHalfHeight;

	GLuint m_TextVertexIndex;
	GLuint m_TextUVIndex;
};*/


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ �ε���� ���------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
class pbLoadingPercent_UI : public pbUI {
public:
	pbLoadingPercent_UI();
	virtual ~pbLoadingPercent_UI();
	virtual void Init();

	virtual void SetVertexIndex(GLuint TextIndex, GLuint NumberIndex);
	virtual void SetUVIndex(GLuint TextIndex ,GLuint StartNumberIndex);

	virtual void Draw();

	virtual void Update(float fTime);

	void DataReset();

	void SetLoadingPercentage(float fPercentage);

	enum{ MAX_DIGITS = 3, NUMBERING = 10 };
private:
	float m_PlacementWidth;
	GLuint m_NumberUVIndex[NUMBERING];	//0~9�� UVIndex
	GLuint m_DigitsNumber[MAX_DIGITS];

	float m_fTextHalfWidth;
	float m_fTextHalfHeight;

	GLuint m_TextVertexIndex;
	GLuint m_TextUVIndex;

	int m_CurrentDigits;
};
*/


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------���μ��� ------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class pbUIProcessor{
private:
	pbUIProcessor();
	~pbUIProcessor();

public:
	static void Create();
	bool LoadData(const char* filename);

	void CreateUIByType(const char* Type, float X, float Y, float Width, float Height);

	pbInterfaceUI* AddBackPanelUI(float X, float Y, float Width, float Height, GLuint UVIndex);
	pbInterfaceUI* AddMenuButtonUI(float X, float Y, float Width, float Height, GLuint UVIndex);
	pbInterfaceUI* AddHelpButtonUI(float X, float Y, float Width, float Height, GLuint UVIndex);
	pbInterfaceUI* AddOneTouchUI(float X, float Y, float Width, float Height, GLuint UVIndex);

	pbInterfaceUI* AddTopSideInfoUI(float X, float Y, pbInterfaceUI* Menu, pbInterfaceUI* Help, pbInterfaceUI* Score, pbInterfaceUI* Gauge);

/*	pbUI* AddAbillityPointUI(float X, float Y, GLuint GaugeVertexIndex, GLuint TextVertexIndex, GLuint GaugeUVIndex, GLuint TextUVIndex);
	pbUI* AddScoreUI(float X, float Y, GLuint NumberVertexIndex, GLuint TextVertexIndex, GLuint NumberUVIndex, GLuint TextUVIndex);
	pbUI* AddLoadingPercentUI(float X, float Y, GLuint TextVertexIndex, GLuint NumberVertexIndex, GLuint TextUVIndex, GLuint NumberUVIndex);*/


	void Update(float time);

	void DeleteUI(pbInterfaceUI* pUI);
	static void Release();

	void ClearDataStore();

	static  pbUIProcessor* GetInstance() { return SingleObject;}
private:
	static pbUIProcessor* SingleObject;

	npLinkNode<pbInterfaceUI>* m_ControledUIStore;

	int m_UICounts;

	void registControled(pbInterfaceUI* pUI);
	void removeControled(pbInterfaceUI* pUI);

};

#endif
