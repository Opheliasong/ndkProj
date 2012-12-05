/*
 * pbComboManager.cpp
 *
 *  Created on: 2012. 12. 4.
 *      Author: NitroSoft
 */

#include "pbComboManager.h"


//--------------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------�޺� �Ŵ���------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------------------------//
pbComboManager* pbComboManager::SingleObject = NULL;

pbComboManager::pbComboManager(){
	SingleObject = NULL;

	m_iCombo = 0;
	m_iFever = 0;
	m_iNextFeverCombo = 2;

	m_pTextDrawUnit = NULL;
	m_pFeverTextDrawUnit = NULL;

	m_PlacementWidth = 0.0f;
	m_fTextPlacementWidth = 0.0f;
	m_CurrentDigits = 1;
}

pbComboManager::~pbComboManager() {

}

void pbComboManager::Create() {

	if( SingleObject == NULL)
	{
		SingleObject = new pbComboManager();

		SingleObject->m_pTextDrawUnit = new pbBasicDrawUnit();
		SingleObject->m_pFeverTextDrawUnit = new pbBasicDrawUnit();

		LOGI("pbComboManager::Create() Complete");

		return ;
	}

	LOGE("pbComboManager::Create() Failed");
}

void pbComboManager::LoadData() {
	DataReset();

	SetTextTag("ci", "ci", 150, 35);
	SetNumberTag("run", "run", 25, 35);
	SetPos(440, 410);
}


void pbComboManager::DataReset(){
	for(int i = 0; i < MAX_DIGITS; i++)
		m_DigitsNumber[i] = 0;

	m_iFever = 0;
	m_iCombo = 0;
	m_CurrentDigits = 1;
	m_iNextFeverCombo = 2;
}

void pbComboManager::SetTextTag(screenplayTag NormalTextTag, screenplayTag FeverTextTag, float fWidth, float fHeight){
	m_pTextDrawUnit->SetTextureTAG(NormalTextTag);
	m_pTextDrawUnit->SetSize(fWidth, fHeight);

	m_pFeverTextDrawUnit->SetTextureTAG(FeverTextTag);
	m_pFeverTextDrawUnit->SetSize(fWidth, fHeight);

	m_fTextPlacementWidth = fWidth/2;

}

void pbComboManager::SetNumberTag(screenplayTag NormalNumberTag, screenplayTag FeverNumberTag, float fWidth, float fHeight){
	SetVertexByCenter(m_NumberVertex, fWidth, fHeight);
	m_PlacementWidth = fWidth;

	//Normal Number Setting
	sprite* pSprite = npContainerDAO::GetInstance().getSpriteByTAG(NormalNumberTag);
	for (int i = 0; i < NUMBERING; i++) {
		int index = pSprite->currentScreenplay->getKernel();
		TextureAtlasIter textureAtlasIterator =
				npAtlasMap::getInstance().FrameContainer.find(index);
		UVPacket* uvPacket = &textureAtlasIterator->second;

		m_NumberUVPacket[i] = uvPacket;

		pSprite->ReadyForNextScreenplay();
	}

	//Fever Number Setting
	pSprite = npContainerDAO::GetInstance().getSpriteByTAG(FeverNumberTag);
	for (int i = 0; i < NUMBERING; i++) {
		int index = pSprite->currentScreenplay->getKernel();
		TextureAtlasIter textureAtlasIterator =
				npAtlasMap::getInstance().FrameContainer.find(index);
		UVPacket* uvPacket = &textureAtlasIterator->second;

		m_FeverNumberUVPacket[i] = uvPacket;

		pSprite->ReadyForNextScreenplay();
	}
}

void pbComboManager::PreSettingDraw() {
	glPushMatrix();
	glTranslatef(m_vPos[0], m_vPos[1], 0);
}

void pbComboManager::DrawThis(){
		glPushMatrix();
		///---------text----------------//
		pbBasicDrawUnit* pDrawUnit;
		if( m_iCombo >= m_iNextFeverCombo) {
			pDrawUnit = m_pTextDrawUnit;
			glScalef(1.05f, 1.1f, 1.0f);
		}
		else
			pDrawUnit = m_pFeverTextDrawUnit;

		pDrawUnit->PreSettingDraw();
		glTranslatef( -m_fTextPlacementWidth - 10,  0.0f,  0);
		pDrawUnit->DrawThis();
		glPopMatrix();


		//number
		glVertexPointer(3, GL_FLOAT, 0, m_NumberVertex);

		int count = m_CurrentDigits - 1;
		for(int i = 0 ; i < m_CurrentDigits; ++i)		{
			glPushMatrix();
			UVPacket* UV;
			if( m_iCombo >= m_iNextFeverCombo) {
				UV = m_FeverNumberUVPacket[m_DigitsNumber[count]];
				glScalef(1.05f, 1.1f, 1.0f);
			}
			else
				UV = m_NumberUVPacket[m_DigitsNumber[count]];

			glBindTexture(GL_TEXTURE_2D,  UV->bindTextureID );
			glTexCoordPointer(2,GL_FLOAT, 0,  UV->texture);

			//�ؽ�ó ���ε�
			glTranslatef( ((float)i)*m_PlacementWidth, 0,  0);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glPopMatrix();

			count--;
		}

	glPopMatrix();
}

void pbComboManager::IncreaseCombo(int Score) {
	m_iCombo++;
//	pbEffectProcess::pEffectProcessor->AddEffect(EFFECTTYPE::COMBO, 400, 240, 1.0f, m_iCombo);

	pbStageValue::m_GettingScore += Score * (m_iFever+1);

	int count = 0;
	int DigitsNumber = m_iCombo;
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

/*	if( (m_iCombo >= m_iNextFeverCombo) && !pbCharacter::GetInstance()->GetFeverReady() )  {
		pbCharacter::GetInstance()->FeverEffectReady();
	}*/
}

bool pbComboManager::FeverOn() {
	if( pbStageValue::IsMaximumGauge() ) {
		pbStageValue::ResetFeverGauge();
/*	if( m_iCombo >= m_iNextFeverCombo) {
		if( m_iNextFeverCombo < 32) {
			m_iNextFeverCombo = m_iNextFeverCombo*2;
			//�ǹ� �ӵ�
			if(m_iFever == 1)
				pbStageValue::m_fStageMoveSpeed *= 1.20f;
			else if(m_iFever == 2)
				pbStageValue::m_fStageMoveSpeed  *= 1.18f;
			else if(m_iFever == 3)
				pbStageValue::m_fStageMoveSpeed  *= 1.171f;
			else if(m_iFever == 4)
				pbStageValue::m_fStageMoveSpeed  *= 1.149f;
			else if(m_iFever == 5)
				pbStageValue::m_fStageMoveSpeed  *= 1.134f;

		}
		else if( m_iNextFeverCombo >= 32 ) {
			pbStageValue::m_fStageMoveSpeed  *= 1.11f;
		}
		m_iCombo = 0;
		m_iFever++;


		for(int i = 0; i < MAX_DIGITS; i++)
			m_DigitsNumber[i] = 0;

		m_CurrentDigits = 1;

		//�ǹ� ����Ʈ �۵�
//		npAudioSystem::playEffect(3);
//		pbUserData::GetInstance().UsingAbilityPoint(-100*m_iFever);
		pbEffectManager::GetInstance()->AddStepUpEffect(400, 220, "ci", 800, 184, "run", 131, 36, "ci", 800, 36, "run", 164, 45);
		*/
		return true;
	}
	else {
		//�ǹ� ����Ʈ �۵� �Ұ�
//		npAudioSystem::playEffect(5);
		pbEffectManager::GetInstance()->AddStickerEffect(pbCharacter::GetInstance()->GetPos()[0], pbCharacter::GetInstance()->GetPos()[1], "ci", 200, 200, 0.3f);
		return false;
	}

	return false;
}

void pbComboManager::ResetCombo() {
	pbStageValue::m_fStageMoveSpeed  = WORLD_MOVESPEED;

//	npAudioSystem::playEffect(4);
	DataReset();

//	pbCharacter::GetInstance()->FeverEffectCancle();

	pbEffectManager::GetInstance()->AddMissEffect();
}

void pbComboManager::ClearDataStore() {
	pbStageValue::m_fStageMoveSpeed  = WORLD_MOVESPEED;
	DataReset();
//	pbCharacter::GetInstance()->FeverEffectCancle();

	//	npAudioSystem::playEffect(4);
	LOGI("pbComboManager ClearDataStore");
}

void pbComboManager::Release() {
	if( SingleObject != NULL)	{
		SingleObject->ClearDataStore();
		delete SingleObject->m_pTextDrawUnit;
		delete SingleObject->m_pFeverTextDrawUnit;
		delete SingleObject;

		SingleObject = NULL;

		LOGI("pbComboManager Release");
	}

}
