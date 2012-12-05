/*
 * pbComboManager.h
 *
 *  Created on: 2012. 12. 4.
 *      Author: NitroSoft
 */

#ifndef PBCOMBOMANAGER_H_
#define PBCOMBOMANAGER_H_

#include "../stdafx.h"

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



#endif /* PBCOMBOMANAGER_H_ */
