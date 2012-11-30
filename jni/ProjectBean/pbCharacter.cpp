#include "pbCharacter.h"

using namespace projectBean;


pbCharacter* pbCharacter::SingleObject = NULL;

pbCharacter::pbCharacter(): m_fLifeRotate(0.0f){
	m_Color.Init(1.0f, 1.0f, 1.0f, 1.0f);

	m_fLifePosX = 0.0f;

	m_iFeverEffectMode = FEVER_NONE;
	m_fFeverEffectDistance = 0.0f;
	m_bFeverReady = false;
	m_fFeverTime = 0.0f;
	m_fFeverDestDistance = 0.0f;
	m_fFeverTargetTime = 0.0f;
	m_fEffectScale = 3.0f;

	m_EffectVertexIndex = 0;
	m_EffectUVIndex = 0; //����Ʈ

	m_RegistSceneTag.reserve(10);

	m_pBodyDrawUnit = NULL;
	m_pSatelliteDrawUnit = NULL;

	m_pMarionette = NULL;

	m_fLifeRotate = 0.0f;

	m_fpTouchFunc = NULL;

}
pbCharacter::~pbCharacter(){

}

void pbCharacter::Create(){
	//---------------------���ʷ����� ��� ���-----------------------//
	if( SingleObject == NULL){
		SingleObject = new pbCharacter();

		SingleObject->m_pBodyDrawUnit = new pbBasicDrawUnit();
		SingleObject->m_pSatelliteDrawUnit = new pbBasicDrawUnit();
		SingleObject->m_pMarionette = new pbMarionette();
		LOGI("CHARACTER Create Complete");

		return;
	}

	LOGE("CHARACTER Create Failed");
}

void pbCharacter::LoadData(sceneTag RegistSceneTag) {
	TouchLayer::GetInstance().RegistedObserver(this);
	m_fpTouchFunc = NULL;
	m_RegistSceneTag.clear();
	m_RegistSceneTag.append(RegistSceneTag);

	m_pBodyDrawUnit->SetTextureTAG("run");
	m_pBodyDrawUnit->SetSize(104, 110);
	m_pSatelliteDrawUnit->SetTextureTAG("run");
	m_pSatelliteDrawUnit->SetSize(28, 28);

	m_fLifePosX = 104*0.43f;

	m_pMarionette->AddLineMoveState(APPEARED, 200, 0, &(pbCharacter::AppearedCondition));
	m_pMarionette->AddLineMoveState(WEAVING_UP,0, 15, &(pbCharacter::WeavingUpCondition));
	m_pMarionette->AddLineMoveState(WEAVING_DOWN,0, -15, &(pbCharacter::WeavingDownCondition));
	m_pMarionette->AddLineMoveState(WALKOUT, WALKOUT_SPEED, 0, &(pbCharacter::WalkOutCondition));

	m_pMarionette->SelectMoveState(NONE);
	m_pMarionette->SetMovePause(false);

	LOGI("pbCharacter::LoadData complete");
}

void pbCharacter::PreSettingDraw() {
	glColor4f(1.0f, 1.0f,1.0f, m_Color.A);
	glPushMatrix();
		m_pMarionette->Translate();
}
void pbCharacter::DrawThis() {
		glPushMatrix();
		m_pBodyDrawUnit->PreSettingDraw();
		m_pBodyDrawUnit->DrawThis();
		glPopMatrix();


		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		m_pSatelliteDrawUnit->PreSettingDraw();
		for(int i = 0; i < pbStageValue::m_iNumLife; i++)
		{
			glPushMatrix();
			glRotatef((float(i)*120.0f) + m_fLifeRotate*360.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(m_fLifePosX + m_fFeverEffectDistance, 0.0f, 0.f);
			m_pSatelliteDrawUnit->DrawThis();
			glPopMatrix();
		}


	glPopMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void pbCharacter::Update(float fTime){
	static float fPartOfLine, fWholeOfLine = 0.0f;

	m_fLifeRotate += 360*fTime;

	///---------------�ǹ� ����Ʈ------------//
	if( m_bFeverReady) {
		//---�ǹ� Ÿ�� ����Ʈ---/
		m_fFeverTargetTime += (1/0.5)*fTime;
		if( m_fFeverTargetTime > 1.0f ) {
			m_fFeverTargetTime = 1.0f;
		}
		m_fEffectScale = npLerp(3.0f, 1.6f, m_fFeverTargetTime );
		//---�ǹ� ��� ����Ʈ---/
		if(m_iFeverEffectMode == FEVER_EXPAND) {
			m_fFeverTime += (1/0.2f)*fTime;
			m_Color.G = 1.0f - m_fFeverTime*0.5f;
			m_Color.B = m_Color.G;
			m_fFeverEffectDistance = npLerp(-10, FEVER_DISTANCE_READY, m_fFeverTime);

			if( m_fFeverTime > 1.0f ) {
				m_fFeverTime = 0.0f;
				m_fFeverEffectDistance = FEVER_DISTANCE_READY;
				m_iFeverEffectMode = FEVER_RETURN;
			}
		}
		else if(m_iFeverEffectMode == FEVER_RETURN) {
			m_fFeverTime += (1/0.2f)*fTime;
			m_Color.G = 0.5f + m_fFeverTime*0.5f;
			m_Color.B = m_Color.G;
			m_fFeverEffectDistance = npLerp(FEVER_DISTANCE_READY, -10, m_fFeverTime);

			if( m_fFeverTime > 1.0f ) {
				m_fFeverTime = 0.0f;
				m_fFeverEffectDistance = -10;
				m_iFeverEffectMode = FEVER_EXPAND;
			}
		}

	}
	else {
		// �ǹ� �۵� ����Ʈ
		if(m_iFeverEffectMode == FEVER_EXPAND) {
			m_fFeverTime += (1/0.7)*fTime;
			m_fFeverEffectDistance = npLerp(0, FEVER_DISTANCE_EXPLOSION, m_fFeverTime);

			if( m_fFeverTime > 1.0f ) {
				m_fFeverTime = 0.0f;
				m_fFeverEffectDistance = FEVER_DISTANCE_EXPLOSION;
				m_iFeverEffectMode = FEVER_RETURN;
			}
		}
		else if(m_iFeverEffectMode == FEVER_RETURN) {
			m_fFeverTime += (1/0.7)*fTime;
			m_fFeverEffectDistance = npLerp(FEVER_DISTANCE_EXPLOSION, 0, m_fFeverTime);

			if( m_fFeverTime > 1.0f ) {
				m_fFeverTime = 0.0f;
				m_fFeverEffectDistance = 0;

				m_iFeverEffectMode = FEVER_NONE;

			}
		}
	}

	//------------------------Marionette--------------------------------//
	int m_MarionetteState = m_pMarionette->GetState();

	if( m_MarionetteState == APPEARED) {
		if( m_pMarionette->GetActionCondition() )
			m_pMarionette->SelectMoveState(WEAVING_UP);
	}
	else if( m_MarionetteState == WEAVING_UP) {
		if( m_pMarionette->GetActionCondition() )
			m_pMarionette->SelectMoveState(WEAVING_DOWN);
	}
	else if( m_MarionetteState == WEAVING_DOWN) {
		if( m_pMarionette->GetActionCondition() )
			m_pMarionette->SelectMoveState(WEAVING_UP);
	}
	else if( m_MarionetteState == WALKOUT) {
		if( m_pMarionette->GetActionCondition() ) {
			m_pMarionette->SelectMoveState(NONE);
			m_pMarionette->SetMovePause(true);

			pbSceneNavigator::GetInstance().SearchAndReadyToMoveScene(SCENESTATE::ACTION_FOWARD);
		}
	}

	m_pMarionette->MoveUpdate(fTime);
}

void pbCharacter::SetPos(float X, float Y){
	m_pMarionette->SetPosX(X);
	m_pMarionette->SetPosY(Y);
}

inline float pbCharacter::GetPosX() { return m_pMarionette->GetV2Pos()[0]; }
inline float pbCharacter::GetPosY() { return m_pMarionette->GetV2Pos()[1]; }

void pbCharacter::Appeared() {
	GetInstance()->m_pMarionette->SelectMoveState(APPEARED);
	GetInstance()->m_pMarionette->SetMovePause(false);

	LOGE("pbCharacter::Appeared() APPEARED");
}

void pbCharacter::WalkOut() {
	GetInstance()->m_pMarionette->SelectMoveState(WALKOUT);
	LOGE("pbCharacter::Appeared() WalkOut");
}

void pbCharacter::DecreaseLife(){
	if( pbStageValue::m_iNumLife> 0)
		pbStageValue::m_iNumLife--;
}

void pbCharacter::FeverEffectOn() {
	m_iFeverEffectMode = FEVER_EXPAND;
	m_fFeverEffectDistance = 0.0f;
	m_fFeverDestDistance = FEVER_DISTANCE_EXPLOSION;

	if( pbBoss::GetInstance()->IsBattlePhase() )
		pbEffectManager::GetInstance()->AddHomingMissileEffect(m_pMarionette->GetV2Pos()[0], m_pMarionette->GetV2Pos()[1], pbBoss::GetMarionette()->GetV2Pos()[0], pbBoss::GetMarionette()->GetV2Pos()[1], "run", 40, 40, 2.0f ,
				pbComboManager::GetInstance()->GetFever()*20.0f, &(pbBoss::DecreaseHP));
}

void pbCharacter::FeverEffectReady() {
	m_iFeverEffectMode = FEVER_EXPAND;
	m_fFeverEffectDistance = 0.0f;
	m_fFeverDestDistance = FEVER_DISTANCE_READY;

	m_bFeverReady = true;

	m_fFeverTargetTime = 0.0f;
	m_fEffectScale = 3.0f;

	LOGE("FEVER READY");
}

void pbCharacter::FeverEffectCancle() {
	m_iFeverEffectMode = FEVER_NONE;
	m_fFeverEffectDistance = 0.0f;
	m_fFeverDestDistance = 0.0f;

	m_Color.G = 1.0f;
	m_Color.B = m_Color.G;

	m_bFeverReady = false;
}

void pbCharacter::ClearDataStore() {
	m_pMarionette->ClearDataStore();
	TouchLayer::GetInstance().RemovedObserver(this);

	LOGI("pbCharacter::ClearDataStore");
}

void pbCharacter::Release(){
	if( SingleObject != NULL) {
		SingleObject->ClearDataStore();

		delete SingleObject->m_pBodyDrawUnit;
		delete SingleObject->m_pSatelliteDrawUnit;
		delete SingleObject->m_pMarionette;

		delete SingleObject;
		SingleObject = NULL;

		LOGI("pbCharacter::Release");
	}

}

void pbCharacter::PlayGame_TouchFunc() {
	pbComboManager::GetInstance()->IncreaseCombo(10);
	if( pbComboManager::GetInstance()->FeverOn() ) {
		pbCharacter::GetInstance()->FeverEffectCancle();
		pbCharacter::GetInstance()->FeverEffectOn();
	}
}
void pbCharacter::Result_TouchFunc() {
	pbSceneManager::getInstance().GetCurrentScene()->GetStageTrigger()->ActivateIDState(pbCharacter::WALKOUT);
}

void pbCharacter::notify(){
	if(TouchLayer::GetInstance().touchFlag == TOUCHFLAGS::TAPDOWN) {
	//		LOGE("[DEBUG]pbCharacter:: TAPDOWN");
			int x = TouchLayer::GetInstance().pointX;
			int y = TouchLayer::GetInstance().pointY;
	//		LOGfloatString("X", x);
	//		LOGfloatString("Y", y);

			int HalfWidth = m_pBodyDrawUnit->getWidth()/2;
			int HalfHeight =m_pBodyDrawUnit->getHeight()/2;

			float* pV2Pos = m_pMarionette->GetV2Pos();

			int left = pV2Pos[0] - HalfWidth;
			int right = pV2Pos[0] + HalfWidth;
			int top = pV2Pos[1] + HalfHeight;
			int bottom = pV2Pos[1] - HalfHeight;

			if (x >= left && x <= right) {
				if (y >= bottom && y <= top) {
					if( m_fpTouchFunc != NULL )
						(*m_fpTouchFunc)();
					LOGE("[DEBUG]pbCharacter:: Touched");
				}
			}
		}
}
///---------------마리오네트 컨디션------------------------------//

bool pbCharacter::AppearedCondition(float* pV2Pos) {
	if(pV2Pos[0] > GetInstance()->m_vConditionPos[0]) {
		GetMarionette()->SetPosX(GetInstance()->m_vConditionPos[0]);
		return true;
	}

	return false;
}
bool pbCharacter::WeavingUpCondition(float* pV2Pos) {
	if(pV2Pos[1] > GetInstance()->m_vConditionPos[1] + 10) {
		GetMarionette()->SetPosY(GetInstance()->m_vConditionPos[1] + 10);
		return true;
	}
	return false;
}
bool pbCharacter::WeavingDownCondition(float* pV2Pos) {
	if(pV2Pos[1] < GetInstance()->m_vConditionPos[1] - 10) {
		GetMarionette()->SetPosY(GetInstance()->m_vConditionPos[1] - 10);
		return true;
	}
	return false;
}
bool pbCharacter::WalkOutCondition(float* pV2Pos) {
	if(pV2Pos[0] > (float)WALKOUT_DEST_POS) {
		GetMarionette()->SetPosX((float)WALKOUT_DEST_POS);
		return true;
	}
	else 	if(pV2Pos[0] > (float)WALKOUT_DEST_POS - 200) {
		if( !pbSceneManager::getInstance().GetCurrentScene()->IsFadeOutStart() ) {
			float Time = ((float)WALKOUT_DEST_POS  - GetMarionette()->GetV2Pos()[0])/(float)WALKOUT_SPEED;
			pbSceneManager::getInstance().GetCurrentScene()->StartFadeOut(Time);
		}
	}
	return false;
}

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

	SetTextTag("ci", "ci", 213, 43);
	SetNumberTag("run", "run", 40, 43);
	SetPos(400, 400);
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

	if( (m_iCombo >= m_iNextFeverCombo) && !pbCharacter::GetInstance()->GetFeverReady() )  {
		pbCharacter::GetInstance()->FeverEffectReady();
	}
}

bool pbComboManager::FeverOn() {
	if( m_iCombo >= m_iNextFeverCombo) {
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
		return true;
	}
	else {
		//�ǹ� ����Ʈ �۵� �Ұ�
//		npAudioSystem::playEffect(5);
		pbEffectManager::GetInstance()->AddStickerEffect(pbCharacter::GetInstance()->GetPosX(), pbCharacter::GetInstance()->GetPosY(), "ci", 200, 200, 0.3f);
		return false;
	}

	return false;
}

void pbComboManager::ResetCombo() {
	pbStageValue::m_fStageMoveSpeed  = WORLD_MOVESPEED;

//	npAudioSystem::playEffect(4);
	DataReset();

	pbCharacter::GetInstance()->FeverEffectCancle();

	pbEffectManager::GetInstance()->AddMissEffect();
}

void pbComboManager::ClearDataStore() {
	pbStageValue::m_fStageMoveSpeed  = WORLD_MOVESPEED;
	DataReset();
	pbCharacter::GetInstance()->FeverEffectCancle();

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

