/*
 * pbResultViewer.cpp
 *
 *  Created on: 2012. 11. 29.
 *      Author: NitroSoft
 */

#include "pbResultViewer.h"
pbNumberRoulette::pbNumberRoulette() {
	m_RealNumber = 0;
	m_RouletteNumber = 0;
	m_LoopCount = 0;
	m_bRouletteStop = true;
}
pbNumberRoulette::~pbNumberRoulette() {

}
void pbNumberRoulette::InitRoulette(int RealNumber, int LoopCount) {
	m_RealNumber = RealNumber;
	m_RouletteNumber = RealNumber;
	IncreaseRouletteNumber();
	m_LoopCount = LoopCount;
	m_bRouletteStop = false;
}

void pbNumberRoulette::IncreaseRouletteNumber() {
	m_RouletteNumber++;
	if( m_RouletteNumber > 9)
		m_RouletteNumber = 0;

}

int pbNumberRoulette::ReturnRouletteNumber() {
	if( !m_bRouletteStop ) {
		IncreaseRouletteNumber();
		if( m_RouletteNumber == m_RealNumber) {
			m_LoopCount--;
			if(m_LoopCount == 0) {
				m_bRouletteStop = true;
			}
		}
	}

	//LOGfloatString("RouletteNumber", m_RouletteNumber);
	return m_RouletteNumber;
}

////------------------------------------------------------------------pbScoreView--------------------------------------------------------------------//

pbScoreView::pbScoreView(TAGDATA& NameTagData, TAGDATA& NumberTagData,  int Score)
{
	m_pNameDrawUnit = new pbBasicDrawUnit();
	m_pNameDrawUnit->SetTextureTAG(NameTagData.Tag);
	m_pNameDrawUnit->SetSize(NameTagData.fWidth, NameTagData.fHeight);

	m_State =STATE_NONE;
	m_CurrentDigits = 0;
	m_TotalDigits = 1;
	for(int i = 0; i < MAX_DIGITS; i++)
		m_DigitsNumber[i] = 0;

	m_fTextPlacementWidth = NameTagData.fWidth/2;
	m_PlacementWidth = NumberTagData.fWidth;

	SetScore(Score);

	//Number Setting
	SetVertexByCenter(m_ScoreVertex, NumberTagData.fWidth, NumberTagData.fHeight);

	sprite* pSprite = npContainerDAO::GetInstance().getSpriteByTAG(NumberTagData.Tag);
	for (int i = 0; i < NUMBERING; i++) {
		int index = pSprite->currentScreenplay->getKernel();
		TextureAtlasIter textureAtlasIterator =
				npAtlasMap::getInstance().FrameContainer.find(index);
		UVPacket* uvPacket = &textureAtlasIterator->second;

		m_ScoreUVPacket[i] = uvPacket;

		pSprite->ReadyForNextScreenplay();
	}


	///----------------룰렛--------------------//
	m_pRoulette = new pbNumberRoulette();
	m_bViewEnd = false;
	m_fAniTime = 0.0f;
}
pbScoreView::~pbScoreView() {
	delete m_pNameDrawUnit;
}


void pbScoreView::Draw() {
	glPushMatrix();
	glTranslatef(m_vPos[0], m_vPos[1], 0);
		///---------text----------------//
		glPushMatrix();
		m_pNameDrawUnit->PreSettingDraw();
		glTranslatef( -m_fTextPlacementWidth,  0.0f,  0);
		m_pNameDrawUnit->DrawThis();
		glPopMatrix();

		//number
		glVertexPointer(3, GL_FLOAT, 0, m_ScoreVertex);
		glTranslatef( m_PlacementWidth, 0,  0);
		int reversePlaceCount = m_TotalDigits-1;
		for(int i = 0 ; i < m_CurrentDigits; ++i)		{
			glPushMatrix();
			UVPacket* UV = m_ScoreUVPacket[m_DigitsNumber[i]];
			glBindTexture(GL_TEXTURE_2D,  UV->bindTextureID );
			glTexCoordPointer(2,GL_FLOAT, 0,  UV->texture);

			glTranslatef( ((float)reversePlaceCount)*m_PlacementWidth, 0,  0);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glPopMatrix();

			reversePlaceCount--;
		}

	glPopMatrix();
}

void pbScoreView::SetScore(int Score) {
	m_iScore = Score;
	int count = 0;
	int DigitsNumber = Score;
	while(1)
	{
		if( count < MAX_DIGITS) {
			m_DigitsNumber[count++] = DigitsNumber%10;
			DigitsNumber /= 10;

			if( DigitsNumber == 0 )
				break;
		}
		else
			break;
	}

	m_TotalDigits = count;
	m_CurrentDigits = 0;
	m_bViewEnd = false;
	m_fAniTime = 0.0f;
}

void pbScoreView::Update(float fTime) {
	if( !m_bViewEnd ) {
		if( m_pRoulette->IsRouletteStop() ) {
			if( m_CurrentDigits < m_TotalDigits  ) {
				m_CurrentDigits++;
				m_pRoulette->InitRoulette(m_DigitsNumber[m_CurrentDigits -1], 1);
			}
			else {
				m_bViewEnd = true;
			}
		}
		else {

			m_fAniTime += fTime;
			if( m_fAniTime >= 0.02f) {
				m_fAniTime = m_fAniTime - 0.02f;
				m_DigitsNumber[m_CurrentDigits -1] = m_pRoulette->ReturnRouletteNumber();
			}
		}
	}
}


////------------------------------------------------------------------pbResultViewer--------------------------------------------------------------------//

pbResultViewer::pbResultViewer() {
	m_ScoreViewVector.reserve(5);
	m_CurrentViewIndex = 0;
	m_TotalViewCount = 0;
	m_bUpdateEnd = false;
}
pbResultViewer::~pbResultViewer() {
	ClearDataStore();
}

void pbResultViewer::PushBackScoreView(float X, float Y, TAGDATA& NameTagData, TAGDATA& NumberTagData, int Score) {
	pbScoreView* pCreateView = new pbScoreView(NameTagData, NumberTagData, Score);
	pCreateView->SetPos(X, Y);
	m_ScoreViewVector.push_back(pCreateView);
//	ScoreViewVector::iterator Iter = m_ScoreViewVector.begin();
//	m_ScoreViewVector.insert(Iter, CreateView);
	m_TotalViewCount++;
}

void pbResultViewer::PreSettingDraw() {
	glPushMatrix();
	glTranslatef(m_vPos[0], m_vPos[1], 0);
}

void pbResultViewer::DrawThis() {
	ScoreViewVector::iterator Iter;
	for(int i = 0 ; i  <= m_CurrentViewIndex; i++ ) {
		pbScoreView* View = m_ScoreViewVector[i];
		View->Draw();
	}

	glPopMatrix();
}

void pbResultViewer::Update(float fTime) {
/*	ScoreViewVector::iterator Iter = m_ScoreViewVector.begin();
	for( ;Iter != m_ScoreViewVector.end(); Iter++ ) {
		(*Iter)->Update(fTime);
		if( (*Iter)->IsScoreViewEnd() )
	}*/
	if( !m_bUpdateEnd ) {
		pbScoreView* View = m_ScoreViewVector[m_CurrentViewIndex];
		View->Update(fTime);
		if( View->IsScoreViewEnd() ) {
			m_CurrentViewIndex++;
			if( m_CurrentViewIndex == m_TotalViewCount) {
				m_bUpdateEnd = true;
				m_CurrentViewIndex = m_TotalViewCount-1;
			}
		}
	}

}

void pbResultViewer::ClearDataStore() {
	m_CurrentViewIndex = 0;
	m_TotalViewCount = 0;
	m_bUpdateEnd = false;

	ScoreViewVector::iterator Iter = m_ScoreViewVector.begin();
	for( ;Iter != m_ScoreViewVector.end(); Iter++ ) {
		ScoreViewVector::iterator NextIter = Iter+1;
		delete (*Iter);
		Iter = NextIter;
	}
	m_ScoreViewVector.clear();
}

