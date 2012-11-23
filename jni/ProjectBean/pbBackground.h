#ifndef _PBBACKGROUND_H
#define _PBBACKGROUND_H

#include "stdafx.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------��׶���     -------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * ��׶���� ������ �ִ� : �̵��ϴ°�, �����Ȱ�
 * ������ ���� m_vPos ������ �ʿ�
 * �̵��ϴ� ���� ���̿�, ��ũ�� ��� �������� �ʿ��ϴ�
 */

class pbBackground : public npDrawable{
public:
	pbBackground() { m_ID = -1; m_vPos[0] = 0.0f; m_vPos[1] = 0.0f; m_fScrollRatio = 1.0f; m_fBackgroundFlowPercent = 1.0f;m_fBGAreaWidth = 0.0f; m_fVertexWidth = 0.0f; m_BackgroundUVIndex = 0;}
	virtual ~pbBackground(){};

	virtual void Initialize(float Width, float Height, GLuint UVIndex){};
	virtual void PreSettingDraw()=0;
	virtual void DrawThis()=0;
	virtual void Update(float fTime)=0;
	virtual void SetFlowSpeedRatio(float fSpeedRatio){	m_fScrollRatio = fSpeedRatio; }

	virtual void SetPos(float X, float Y){ m_vPos[0] = X; m_vPos[1] = Y;}

	inline int GetID() const { return m_ID; }
	//void Release();
	bool operator==(const pbBackground& rhs) {
		return m_ID ==  rhs.GetID();
	}
protected:
	static GLfloat m_BackgrounVertex[12];
	GLuint m_BackgroundUVIndex;
	npV2Vector m_vPos;
	float m_fScrollRatio; //��� ���̿� �� �÷ο� �ӵ� ������
	float m_fBackgroundFlowPercent;		// 0���� ���� 1���� ��
	float m_fBGAreaWidth;
	float m_fVertexWidth;

	int m_ID;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------Static ��׶���     -------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class pbStaticBackground : public pbBackground{
public:
	pbStaticBackground();
	virtual ~pbStaticBackground();

	virtual void Initialize(float Width, float Height, GLuint UVIndex);
	virtual void PreSettingDraw();
	virtual void DrawThis();
	virtual void Update(float fTime);
private:
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------Move ��׶���     -------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class pbMoveBackground : public pbBackground{
public:
	pbMoveBackground();
	virtual ~pbMoveBackground();

	virtual void Initialize(float Width, float Height, GLuint UVIndex);
	virtual void PreSettingDraw();
	virtual void DrawThis();
	virtual void Update(float fTime);
private:
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------��ũ�� ��׶���     -------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class pbScrollBackground : public pbBackground{
public:
	pbScrollBackground();
	virtual ~pbScrollBackground();

	virtual void Initialize(float Width, float Height, GLuint UVIndex);

	virtual void PreSettingDraw();
	virtual void DrawThis();

	virtual void Update(float fTime);

	//void Release();
private:
	static GLfloat m_BackgrounVertex[18];

	GLfloat m_BackgroundUV[8];
	GLuint m_BackgroundUVIndex;

	GLfloat m_StandardUV_LeftSideUCoord;
	GLfloat m_StandardUV_RightSideUCoord;

	float m_fBackgroundUV_WidthPercent;
	float m_fBackGroundUV_Width;

	GLuint m_UVBindID;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------��׶��� ���μ���-------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef npLinkNode<pbBackground*> BackgroundList;
class pbBackgroundProcessor {
public:
	pbBackgroundProcessor();
	~pbBackgroundProcessor();

	static pbBackgroundProcessor& GetInstance();

	pbBackground*  AddScrollBackGround(float Width, float Height, float X, float Y, float SpeedRatio, screenplayTag Tag);
	pbBackground*  AddMoveBackGround(float Width, float Height, float X, float Y, float SpeedRatio, screenplayTag Tag);
	pbBackground*  AddStaticBackGround(float Width, float Height, float X, float Y, screenplayTag Tag);

	void Update(float fTime);

	void ClearDataStore();	// Clear �� ��ũ�帮��Ʈ�� ����� ����� ��� ����, ������ ���� �������� Ŀ�ε� ����

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
