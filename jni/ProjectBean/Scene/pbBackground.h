#ifndef _PBBACKGROUND_H
#define _PBBACKGROUND_H

#include "../stdafx.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------pbBackground   -------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class pbBackground : public npDrawable {
public:
	pbBackground() { m_ID = -1; m_vPos[0] = 0.0f; m_vPos[1] = 0.0f; m_fScrollRatio = 1.0f; m_fBackgroundFlowPercent = 1.0f;m_fBGAreaWidth = 0.0f; m_fVertexWidth = 0.0f; }
	virtual ~pbBackground() {};

	virtual void Initialize(float Width, float Height, screenplayTag Tag)=0;
	virtual void PreSettingDraw()=0;
	virtual void DrawThis()=0;
	virtual void Update(float fTime)=0;
	void SetFlowSpeedRatio(float fSpeedRatio){	m_fScrollRatio = fSpeedRatio; }

	virtual void SetPos(float X, float Y){ m_vPos[0] = X; m_vPos[1] = Y;}

	inline int GetID() const { return m_ID; }
	//void Release();
	bool operator==(const pbBackground& rhs) {
		return m_ID ==  rhs.GetID();
	}

protected:
	npV2Vector m_vPos;
	float m_fScrollRatio;
	float m_fBackgroundFlowPercent;
	float m_fBGAreaWidth;
	float m_fVertexWidth;

	int m_ID;
};

class pbBasicDrawUnit;
class pbBasicBackground : public pbBackground{
public:
	pbBasicBackground(){ m_BackgroundTextureRect = NULL; }
	virtual ~pbBasicBackground(){};

	virtual void Initialize(float Width, float Height,  screenplayTag Tag)=0;
	virtual void PreSettingDraw();
	virtual void DrawThis();
	virtual void Update(float fTime)=0;
protected:
	pbBasicDrawUnit* m_BackgroundTextureRect;
};

class pbUVControllBackground : public pbBackground{
public:
	pbUVControllBackground() {
		m_fBackgroundUV_Width = 1.0f; m_fBackgroundUV_WidthPercent = 1.0f;
		m_UVBindID = 0;		m_BackgroundUVIndex  = 0 ;
		m_StandardUV_LeftSideUCoord = 0;
		m_StandardUV_RightSideUCoord = 0;
	}
	virtual ~pbUVControllBackground(){};

	virtual void Initialize(float Width, float Height,  screenplayTag Tag)=0;
	virtual void PreSettingDraw()=0;
	virtual void DrawThis()=0;
	virtual void Update(float fTime)=0;
protected:
	GLfloat m_UVControllVertex[18];

	GLfloat m_BackgroundUV[8];
	GLuint m_BackgroundUVIndex;

	GLfloat m_StandardUV_LeftSideUCoord;
	GLfloat m_StandardUV_RightSideUCoord;

	float m_fBackgroundUV_WidthPercent;
	float m_fBackgroundUV_Width;

	GLuint m_UVBindID;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------pbStaticBackground -------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class pbStaticBackground : public pbBasicBackground{
public:
	pbStaticBackground();
	virtual ~pbStaticBackground();

	virtual void Initialize(float Width, float Height,  screenplayTag Tag);
	virtual void Update(float fTime);
private:
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------pbMoveBackground   -------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class pbMoveBackground : public pbBasicBackground{
public:
	pbMoveBackground();
	virtual ~pbMoveBackground();

	virtual void Initialize(float Width, float Height, screenplayTag Tag);
	virtual void Update(float fTime);
private:
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------pbScrollBackground    -------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class pbScrollBackground : public pbUVControllBackground{
public:
	pbScrollBackground();
	virtual ~pbScrollBackground();

	virtual void Initialize(float Width, float Height, screenplayTag Tag);

	virtual void PreSettingDraw();
	virtual void DrawThis();

	virtual void Update(float fTime);

private:
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------pbTouchableBackground   -------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class pbTouchableBackground : public pbBasicBackground, public iTouchObserver{
public:
	pbTouchableBackground();
	virtual ~pbTouchableBackground();

	virtual void Initialize(float Width, float Height, screenplayTag Tag);
	virtual void Update(float fTime);

	virtual void notify();

	inline bool IsTouched() {return m_bTouched;}
	inline void ResetTouched() { m_bTouched = false; }
private:
	bool m_bTouched;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------pbBackgroundProcessor-------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef npLinkNode<pbBackground*> BackgroundList;
class pbBackgroundProcessor {
public:
	pbBackgroundProcessor();
	~pbBackgroundProcessor();

	static pbBackgroundProcessor& GetInstance();

	pbBackground* AddScrollBackGround(float Width, float Height, float X, float Y, float SpeedRatio, screenplayTag Tag);
	pbBackground* AddMoveBackGround(float Width, float Height, float X, float Y, float SpeedRatio, screenplayTag Tag);
	pbBackground* AddStaticBackGround(float Width, float Height, float X, float Y, screenplayTag Tag);
	pbTouchableBackground* AddTouchableBackGround(float Width, float Height, float X, float Y, screenplayTag Tag);
	void Update(float fTime);

	void ClearDataStore();

	void registControled(pbBackground* pBG);
	void DeleteBackground(pbBackground* pBG);

private:
	BackgroundList* m_pBackgroundQueue;

	int m_iBackgroundCount;


};


/*class pbShootingStar {
public:
	pbShootingStar();
	~pbShootingStar();

	static void Initialize(int size);

	void Draw();

	void Update(float fTime);

	void DataReset();

	static void Release();
public:
	static pbShootingStar* SingleObject;
private:
	GLuint m_iMaxStar;

	GLuint m_VertexIndex;
	GLuint m_UVIndex;

	float* m_pfScale;
	float* m_pfAlpha;	//���Ĵ� �ӵ��� ���
	float** m_pvPos;
};*/



#endif
