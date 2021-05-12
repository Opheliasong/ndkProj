/*
 * pbHPMeter.cpp
 *
 *  Created on: 2012. 12. 4.
 *      Author: NitroSoft
 */

#include "pbHPMeter.h"



//---------------------------------------------- pbBossHpMeter------------------------------------------------------------------------------------//
pbHPMeter::pbHPMeter() {
	m_Phase = PHASE_NONE;
	m_vPos[0] = 0;
	m_vPos[1] = 0;

	m_bDie = false;
	m_fOneLineHP = 0;
	m_fTotalHP = 0;


	////---------------게이지 변수-------------------////
	//--게이지 컨트롤 변수--//
	m_bGaugeSetting = false;
	m_bGaugeChangeStart = false;
	m_fDecreaseLinePercent  = 0.0f;
	m_iLineCount = 0;
	//--선형보간 변수--//
	m_fAniSpeed = 0.0f;
	m_fAniTime = 0.0f;
	m_fDestPercent = 1.0f;
	m_fStartPercent = 1.0f;
	m_fDrawPercent = 1.0f;
	m_fDrawPosX = 0.0f;

	//--Drawing 관련 변수--//
	m_fHorizonScale = 0.0f;
	m_fVerticalScale = 0.0f;
	m_fGaugeHalfWidth = 0.0f;
	m_fGaugeHalfHeight = 0.0f;

	m_GaugeUVBindID = 0;

	m_GaugeUV_WidthPercent = 0.0f;

	m_pBaseDrawUnit = new pbBasicDrawUnit();
	m_pUVPacket = NULL;
}

pbHPMeter::~pbHPMeter() {
	delete m_pBaseDrawUnit;
}

void pbHPMeter::Initialize(float fTotalHP, float fOneLineHP) {
	m_Phase = PHASE_NONE;
	m_fHorizonScale = 0.0f;
	m_fVerticalScale = 0.0f;
	m_fAniSpeed = 0.0f;
	m_fAniTime = 0.0f;

	SetCurrentHP(fTotalHP, fOneLineHP);
}


void pbHPMeter::SetCurrentHP(float HP, float fOneLineHP) {
	m_bDie = false;
	m_fTotalHP = HP;
	m_fOneLineHP = fOneLineHP;
	m_iLineCount = 0;

	float fCurrentHP = HP;
	while(fCurrentHP > m_fOneLineHP) {
		m_iLineCount++;
		fCurrentHP -= m_fOneLineHP;
	}

	m_fDestPercent = fCurrentHP/m_fOneLineHP;
	m_fDrawPercent = m_fDestPercent;
	m_fStartPercent = m_fDrawPercent;

	m_fDecreaseLinePercent = 0.0f;
	m_bGaugeChangeStart = false;
	m_bGaugeSetting = false;
}

void pbHPMeter::SetTag(screenplayTag PanelTag, screenplayTag MeterTag, float fWidth, float fHeight) {
	m_pBaseDrawUnit->SetTextureTAG(PanelTag);
	m_pBaseDrawUnit->SetSize(fWidth, fHeight);

	//크기 설정
	SetVertexByCenter(m_GaugeVertex, fWidth, fHeight);
	m_fGaugeHalfWidth =  fWidth/2;
	m_fGaugeHalfHeight =  fHeight/2;

	//UV 추출
	sprite* pSprite = npContainerDAO::GetInstance().getSpriteByTAG(MeterTag);

	int index = pSprite->currentScreenplay->getKernel();
	TextureAtlasIter textureAtlasIterator =  npAtlasMap::getInstance().FrameContainer.find(index);
	UVPacket* uvPacket = &textureAtlasIterator->second;
	m_pUVPacket = uvPacket;
	m_GaugeUVBindID = uvPacket->bindTextureID;
	memcpy(m_GaugeUV, uvPacket->texture, sizeof(GLfloat)*8);

	m_GaugeUV_WidthPercent = m_GaugeUV[4] - m_GaugeUV[0];

	m_fDrawPosX = - m_fGaugeHalfWidth*(1.0f - m_fDrawPercent);
}

void pbHPMeter::DecreaseHP(float Damage) {
	m_fTotalHP -= Damage;

	if( m_fTotalHP <= 0 ) {
		m_bDie = true;
	}

	m_fDecreaseLinePercent += Damage/m_fOneLineHP;
	m_bGaugeSetting = true;
}

void pbHPMeter::AnimationGauge(float fTime) {
	//게이지 세팅
	if( m_bGaugeSetting ) {
		float fDestPrevision = m_fStartPercent - m_fDecreaseLinePercent;
		if( fDestPrevision <= 0.0f ) {
			m_fDestPercent = 0.0f;
		}
		else
			m_fDestPercent = fDestPrevision;

		m_bGaugeChangeStart = true;
		float fWholeOfDistance = m_fStartPercent - m_fDestPercent;
		float fPartOfDistance = m_fStartPercent - m_fDrawPercent;
		if( fPartOfDistance == 0.0f )
			m_fAniTime = 0.0f;
		else
			m_fAniTime = (fPartOfDistance/fWholeOfDistance);  // distance 0.5 -> 0.8 time == 0.4 percent      distance 1.0  ->    ? time == 0.6 percent

		m_fAniSpeed = (fWholeOfDistance)*2;	//곱해지는 양은 걸리는 시간과 같다

		m_bGaugeSetting = false;
	}

	// 게이지 선형 보간
	if( m_bGaugeChangeStart ) {
		m_fAniTime += (1.0f / m_fAniSpeed) * fTime;

		if (m_fAniTime >= 1.0f) {
			m_fAniTime = 1.0f;

			m_fDecreaseLinePercent -= m_fStartPercent - m_fDestPercent;
			if( m_fDestPercent == 0.0f) {
				if( m_iLineCount == 0) {
					CloseMeter();
				}
				else {
					m_iLineCount--;
					m_bGaugeSetting = true;
					m_fDestPercent = 1.0f;
				}
			}

			m_fStartPercent = m_fDestPercent;// m_fDrawPercent는 m_fAniTime으로 밑의 선형보간에서 초기화 한다
			m_bGaugeChangeStart = false;
		}

		m_fDrawPercent = npLerp(m_fStartPercent, m_fDestPercent, m_fAniTime);
		m_fDrawPosX = -m_fGaugeHalfWidth * (1.0f - m_fDrawPercent);

	}
}

void pbHPMeter::OpenMeter() {
	m_Phase = PHASE_OPEN_HORIZON;
	m_fAniTime = 0.0f;
	m_fHorizonScale = 0.0f;
	m_fVerticalScale = 0.3f;
}

void pbHPMeter::CloseMeter() {
	m_Phase = PHASE_CLOSE_HORIZON;
	m_fAniTime = 0.0f;
	m_fHorizonScale = 1.0f;
	m_fVerticalScale = 1.0f;
}

void pbHPMeter::BindLineColor(int Line) {
	if( Line == 0)
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	else if( Line == 1)
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	else if( Line == 2)
		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	else if( Line == 3)
		glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	else if( Line == 4)
		glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
	else if( Line == 5)
		glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
	else if( Line == 6)
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void pbHPMeter::Draw() {
	if( m_Phase != PHASE_NONE) {
	glPushMatrix();
		glTranslatef(m_vPos[0], m_vPos[1],  0);
		glScalef(m_fHorizonScale, m_fVerticalScale, 1.0f);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		///------------Power Text--------------------
		glPushMatrix();
		m_pBaseDrawUnit->PreSettingDraw();
		m_pBaseDrawUnit->DrawThis();
		glPopMatrix();
		///------------게이지 텍스쳐 바인드--------------------
		glBindTexture(GL_TEXTURE_2D,  m_GaugeUVBindID );
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glVertexPointer(3, GL_FLOAT, 0, m_GaugeVertex);
		///-------------중복 라인 그리기
		glTexCoordPointer(2,GL_FLOAT, 0,  m_pUVPacket->texture);

		for(int i = 0; i < m_iLineCount ; i++) {
			BindLineColor(i);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
		BindLineColor(m_iLineCount);
		///------------게이지 텍스쳐 그리기--------------------
		glTexCoordPointer(2,GL_FLOAT, 0,  m_GaugeUV);
		glPushMatrix();
		m_GaugeUV[4] = m_GaugeUV[0] + m_GaugeUV_WidthPercent*m_fDrawPercent;
		m_GaugeUV[6] = m_GaugeUV[2] + m_GaugeUV_WidthPercent*m_fDrawPercent;
		glTranslatef( m_fDrawPosX, 0 ,  0);
		glScalef(m_fDrawPercent, 1.0f, 1.0f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glPopMatrix();

	glPopMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

void pbHPMeter::Update(float fTime) {
	if( m_Phase == PHASE_NONE) {

	}
	else if( m_Phase == PHASE_OPEN_HORIZON) {
		m_fAniTime += fTime;
		m_fHorizonScale = m_fAniTime;
		if(m_fAniTime > 1.0f) {
			m_fAniTime = 0.3f;
			m_Phase = PHASE_OPEN_VERTICAL;
			m_fHorizonScale = 1.0f;
		}
	}
	else if( m_Phase == PHASE_OPEN_VERTICAL) {
		m_fAniTime += 2*fTime;
		m_fVerticalScale = m_fAniTime;
		if(m_fAniTime > 1.0f) {
			m_fAniTime = 0.0f;
			m_Phase = PHASE_GAUGING;
			m_fVerticalScale = 1.0f;

			pbBoss::GetInstance()->SetBattlePhase(true	);
		}
	}
	else if( m_Phase == PHASE_GAUGING) {
		AnimationGauge(fTime);
	}
	else if( m_Phase == PHASE_CLOSE_HORIZON) {
		m_fAniTime += fTime;
		m_fHorizonScale = (1.0f - m_fAniTime );
		if(m_fAniTime > 1.0f) {
			m_fAniTime = 0.0f;
			m_Phase = PHASE_NONE;
			m_fHorizonScale = 0.0f;
		}
	}

}
