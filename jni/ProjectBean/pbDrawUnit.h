/*
 * pbDrawable.h
 *
 *  Created on: 2012. 11. 7.
 *      Author: goldbin
 */

#ifndef PBDRAWUNIT_H_
#define PBDRAWUNIT_H_

#include "stdafx.h"

/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ IDraw ------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class pbIDraw {
public:
	pbIDraw() {};
	virtual ~pbIDraw() {};
	virtual void Draw() = 0;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ DrawUnit ------------------------------------------------------------------------------///////
// �⺻������ �ϳ��� �ؽ��ĸ� �����, �ڽ��� �ϳ��� ����ϸ� List����� ��Ǹ鼭 ������ ���鼭 �׸��� �ȴ�

class pbDrawUnit : public pbIDraw {
	void registLinkNode(pbDrawUnit* pUnit);
	void removeLinkNode(pbDrawUnit* pUnit);
public:
	pbDrawUnit();
	virtual ~pbDrawUnit();

	virtual void Draw();

	virtual void AddChildUnit(pbDrawUnit* pChild);
	virtual void DeleteChildUnit(GLuint ID);

	inline GLuint GetID() { return m_ID;}
	inline float* GetV2Pos() { return m_vPos;}
	inline float* GetSizeWH() { return m_SizefWH;}
	inline GLuint GetUVIndex() { return m_UVIndex;}

	inline void SetID(GLuint ID) { m_ID = ID; }
	inline void SetV2Pos(float X, float Y) { m_vPos[iX] = X; m_vPos[iY] = Y; }
	inline void SetSizeWH(float Width, float Height) { m_SizefWH[iW] = Width; m_SizefWH[iH] = Height; }
	inline void SetUVIndex(GLuint Index) { m_UVIndex = Index; }
private:
	COLOR_RGBA m_Color;
	npV2Vector m_vPos;
	npSizef_WH m_SizefWH;
	GLuint m_UVIndex;
	GLuint m_ID;
	typedef npLinkNode<pbDrawUnit*> pbDrawUnit_List;
	pbDrawUnit_List* m_pDrawableUnitListHeader;
};*/

class pbBasicDrawUnit: public npDrawable {
public:
	pbBasicDrawUnit();
	pbBasicDrawUnit(screenplayTag TAG);
	pbBasicDrawUnit(const pbBasicDrawUnit& source);
	virtual ~pbBasicDrawUnit();

	virtual void PreSettingDraw();
	virtual void DrawThis();

	NP_DEFINE_PROPERTY(float, m_Width, Width);
	NP_DEFINE_PROPERTY(float, m_Height, Height);

	void SetSize(float width, float Height);
	bool SetTextureTAG(screenplayTag TAG);
	sprite* getSprite();
protected:
	static GLfloat vertex[12];
	sprite* sprtie;
	screenplayTag tag;
};

#endif /* PBDRAWUNIT_H_ */
