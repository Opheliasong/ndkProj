/*
 * pbResultViewer.cpp
 *
 *  Created on: 2012. 11. 29.
 *      Author: NitroSoft
 */

#include "pbResultViewer.h"


pbScoreView::pbScoreView(screenplayTag NameTag, float fNameWidth, float fNameHeight,
											screenplayTag NumberZeroTag,	float fNumberWidth, float fNumberHeight, int Score)
{
	m_pNameDrawUnit = new pbBasicDrawUnit();
	m_pNameDrawUnit->SetTextureTAG(NameTag);
	m_pNameDrawUnit->SetSize(fNameWidth, fNameHeight);

	m_State =STATE_NONE;
	m_CurrentDigits = 1;
	for(int i = 0; i < MAX_DIGITS; i++)
		m_DigitsNumber[i] = 0;

	m_fTextPlacementWidth = fNameWidth/2;
	m_PlacementWidth = fNumberWidth;

	UpdateScore(Score);

	//Number Setting
	SetVertexByCenter(m_ScoreVertex, fNumberWidth, fNumberHeight);

	sprite* pSprite = npContainerDAO::GetInstance().getSpriteByTAG(NumberZeroTag);
	for (int i = 0; i < NUMBERING; i++) {
		int index = pSprite->currentScreenplay->getKernel();
		TextureAtlasIter textureAtlasIterator =
				npAtlasMap::getInstance().FrameContainer.find(index);
		UVPacket* uvPacket = &textureAtlasIterator->second;

		m_ScoreUVPacket[i] = uvPacket;

		pSprite->ReadyForNextScreenplay();
	}

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
		int count = m_CurrentDigits - 1;
		for(int i = 0 ; i < m_CurrentDigits; ++i)		{
			glPushMatrix();
			UVPacket* UV = m_ScoreUVPacket[m_DigitsNumber[count]];
			glBindTexture(GL_TEXTURE_2D,  UV->bindTextureID );
			glTexCoordPointer(2,GL_FLOAT, 0,  UV->texture);

			glTranslatef( ((float)i)*m_PlacementWidth, 0,  0);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glPopMatrix();

			count--;
		}

	glPopMatrix();
}

void pbScoreView::UpdateScore(int Score) {
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

	m_CurrentDigits = count;
}

void pbScoreView::Update(float fTime) {

}



pbResultViewer::pbResultViewer() {
	m_ScoreViewVector.reserve(5);
}
pbResultViewer::~pbResultViewer() {
	ClearDataStore();
}

void pbResultViewer::PushBackScoreView(float X, float Y, screenplayTag NameTag, float fNameWidth, float fNameHeight,
																							screenplayTag NumberZeroTag,	float fNumberWidth, float fNumberHeight, int Score) {
	pbScoreView* pCreateView = new pbScoreView(NameTag, fNameWidth, fNameHeight, NumberZeroTag, fNumberWidth, fNumberHeight, Score);
	pCreateView->SetPos(X, Y);
	m_ScoreViewVector.push_back(pCreateView);
//	ScoreViewVector::iterator Iter = m_ScoreViewVector.begin();
//	m_ScoreViewVector.insert(Iter, CreateView);
}

void pbResultViewer::PreSettingDraw() {
	glPushMatrix();
	glTranslatef(m_vPos[0], m_vPos[1], 0);
}

void pbResultViewer::DrawThis() {
	ScoreViewVector::iterator Iter = m_ScoreViewVector.begin();
	for( ;Iter != m_ScoreViewVector.end(); Iter++ ) {
		(*Iter)->Draw();
	}
	glPopMatrix();
}

void pbResultViewer::Update(float fTime) {
	ScoreViewVector::iterator Iter = m_ScoreViewVector.begin();
	for( ;Iter != m_ScoreViewVector.end(); Iter++ ) {
		(*Iter)->Update(fTime);
	}
}

void pbResultViewer::ClearDataStore() {
	ScoreViewVector::iterator Iter = m_ScoreViewVector.begin();
	for( ;Iter != m_ScoreViewVector.end(); Iter++ ) {
		ScoreViewVector::iterator NextIter = Iter+1;
		delete (*Iter);
		Iter = NextIter;
	}
	m_ScoreViewVector.clear();
}

