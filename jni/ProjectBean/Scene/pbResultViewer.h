/*
 * pbResultViewer.h
 *
 *  Created on: 2012. 11. 29.
 *      Author: NitroSoft
 */

#ifndef PBRESULTVIEWER_H_
#define PBRESULTVIEWER_H_

#include "../stdafx.h"

class pbBasicDrawUnit;
class pbScoreView {
	pbScoreView();
public:
	pbScoreView(screenplayTag NameTag, float fNameWidth, float fNameHeight,
			screenplayTag NumberZeroTag,	float fNumberWidth, float fNumberHeight, int Score);
	~pbScoreView();

	void Draw();

	void UpdateScore(int Score);
	void Update(float fTime);

	void NextViewState() { if(m_State < STATE_END)  m_State++; }

	void SetPos(float X, float Y) { m_vPos[0] = X; m_vPos[1] = Y; }
	int GetViewState() { return m_State; }

	enum {NUMBERING = 10, MAX_DIGITS = 10 };
	enum {STATE_NONE = 0, STATE_START, STATE_APPEARED, STATE_ANI, STATE_END} ;
private:
	pbBasicDrawUnit* m_pNameDrawUnit;

	GLfloat m_ScoreVertex[12];
	UVPacket* m_ScoreUVPacket[NUMBERING];
	GLuint m_DigitsNumber[MAX_DIGITS];
	GLuint m_CurrentDigits;
	float m_fTextPlacementWidth;
	float m_PlacementWidth;

	npV2Vector m_vPos;

	int m_iScore;
	int m_State;
};

class pbResultViewer : public npDrawable{
public:
	pbResultViewer();
	~pbResultViewer();

	void PushBackScoreView(float X, float Y, screenplayTag NameTag, float fNameWidth, float fNameHeight,
			screenplayTag NumberZeroTag,	float fNumberWidth, float fNumberHeight, int Score);

	virtual void PreSettingDraw();
	virtual void DrawThis();

	void Update(float fTime);

	void SetPos(float X, float Y) { m_vPos[0] = X; m_vPos[1]  = Y; }

	void ClearDataStore();

private:
	typedef std::vector<pbScoreView*>ScoreViewVector;
	ScoreViewVector m_ScoreViewVector;

	npV2Vector m_vPos;
};



#endif /* PBRESULTVIEWER_H_ */
