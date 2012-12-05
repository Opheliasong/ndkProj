/*
 * pbHPMeter.h
 *
 *  Created on: 2012. 12. 4.
 *      Author: NitroSoft
 */

#ifndef PBHPMETER_H_
#define PBHPMETER_H_

#include "../stdafx.h"
class pbBasicDrawUnit;
//----------------------------------------------pbHPMeter------------------------------------------------------------------------------------//
class pbHPMeter {
public:
	pbHPMeter();
	~pbHPMeter();

	void Initialize(float CurrentHP, float OneLineHP);

	void SetTag(screenplayTag PanelTag, screenplayTag MeterTag, float fWidth, float fHeight);
	void SetPos(float X, float Y) { m_vPos[0] = X, m_vPos[1] = Y;}
	void SetCurrentHP(float HP, float fOneLineHP);

	void BindLineColor(int Line);

	float GetTotalHP() { return m_fTotalHP;}
	void DecreaseHP(float Damage);
	void AnimationGauge(float fTime);
	bool IsDied() { return m_bDie; }

	void OpenMeter();
	void CloseMeter();

	void Draw();
	void Update(float fTime);

	enum {PHASE_NONE = 0, PHASE_OPEN_HORIZON, PHASE_OPEN_VERTICAL, PHASE_GAUGING, PHASE_CLOSE_HORIZON};
private:
	int m_Phase;
	npV2Vector m_vPos;
	pbBasicDrawUnit* m_pBaseDrawUnit;
	UVPacket* m_pUVPacket;

	////---------------체력 변수-------------------////
	bool m_bDie;
	float m_fOneLineHP;
	float m_fTotalHP;

	////---------------게이지 변수-------------------////
	//--게이지 컨트롤 변수--//
	bool m_bGaugeChangeStart;
	bool m_bGaugeSetting;
	float m_fDecreaseLinePercent;
	int m_iLineCount;	// LineCount = 0 is Last HPLine

	//--선형보간 변수--//
	float m_fAniSpeed;
	float m_fAniTime;
	float m_fDestPercent;
	float m_fStartPercent;
	float m_fDrawPercent;
	float m_fDrawPosX;

	//--Drawing 관련 변수--//
	float m_fHorizonScale;
	float m_fVerticalScale;

	float m_fGaugeHalfWidth;
	float m_fGaugeHalfHeight;

	GLfloat m_GaugeUV[8];
	GLfloat m_GaugeVertex[12];
	GLuint m_GaugeUVBindID;

	float m_GaugeUV_WidthPercent;
};

#endif /* PBHPMETER_H_ */
