#include "pbCharacter.h"

using namespace projectBean;


pbCharacter* pbCharacter::SingleObject = NULL;

pbCharacter::pbCharacter(): m_fLifeRotate(0.0f){
	m_Color.Init(1.0f, 1.0f, 1.0f, 1.0f);

	m_vBodyPos[0] = 0.0f;
	m_vBodyPos[1] = 0.0f;

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

	COLOR_RGBA m_Color;

	npV2Vector m_vBodyPos;

	float m_fLifeRotate;
	float m_fLifePosX;

	enum { FEVER_NONE = 0, FEVER_EXPAND, FEVER_RETURN , FEVER_DISTANCE_READY = 10, FEVER_DISTANCE_EXPLOSION = 100};
	int m_iFeverEffectMode;
	float m_fFeverEffectDistance;
	float m_fFeverTime;
	float m_fFeverDestDistance;
	bool m_bFeverReady;

	GLuint m_EffectVertexIndex;
	GLuint m_EffectUVIndex;

	float m_fFeverTargetTime;
	float m_fEffectScale;
}
pbCharacter::~pbCharacter(){

}

void pbCharacter::Create(){
	//---------------------���ʷ����� ��� ���-----------------------//
	if( SingleObject == NULL){
		SingleObject = new pbCharacter();

		SingleObject->m_pBodyDrawUnit = new pbBasicDrawUnit();
		SingleObject->m_pSatelliteDrawUnit = new pbBasicDrawUnit();

		LOGI("CHARACTER Create Complete");

		return;
	}

	LOGE("CHARACTER Create Failed");
}

void pbCharacter::LoadData(sceneTag RegistSceneTag) {
	TouchLayer::GetInstance().RegistedObserver(this);

	m_RegistSceneTag.clear();
	m_RegistSceneTag.append(RegistSceneTag);

	m_pBodyDrawUnit->SetTextureTAG("run");
	m_pBodyDrawUnit->SetSize(104, 110);
	m_pSatelliteDrawUnit->SetTextureTAG("ci");
	m_pSatelliteDrawUnit->SetSize(28, 28);

	m_fLifePosX = 104*0.43f;

	SetPos(72.0f, 240.0f);

	LOGI("pbCharacter::LoadData complete");
}

void pbCharacter::PreSettingDraw() {
	glColor4f(1.0f, 1.0f,1.0f, m_Color.A);
	glPushMatrix();
		glTranslatef(m_vBodyPos[0] , m_vBodyPos[1], 0.f);
}
void pbCharacter::DrawThis() {
		m_pBodyDrawUnit->PreSettingDraw();
		m_pBodyDrawUnit->DrawThis();

		m_pSatelliteDrawUnit->PreSettingDraw();
		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		for(int i = 0; i < pbGlobalInGameVariable::NumLife ; i++)
		{
			glPushMatrix();
			glRotatef((float(i)*120.0f) + m_fLifeRotate*360.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(m_fLifePosX + m_fFeverEffectDistance, 0.0f, 0.f);
			m_pSatelliteDrawUnit->DrawThis();
		}
		glPopMatrix();

	glPopMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void pbCharacter::Update(float fTime){
	static float fPartOfLine, fWholeOfLine = 0.0f;

	m_fLifeRotate += (pbGlobalInGameVariable::fWorldMoveSpeed/400)*fTime;

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
}


void pbCharacter::DecreaseLife(){
	if( pbGlobalInGameVariable::NumLife > 0)
		pbGlobalInGameVariable::NumLife--;

//	pbGlobalInGameVariable::bCameraShake = true;
}

void pbCharacter::FeverEffectOn() {
	m_iFeverEffectMode = FEVER_EXPAND;
	m_fFeverEffectDistance = 0.0f;
	m_fFeverDestDistance = FEVER_DISTANCE_EXPLOSION;

/*	if( pbBoss::GetInstance()->IsBattlePhase() )
		pbEffectProcess::GetInstance()->AddHomingMissileEffect(m_vBodyPos[0], m_vBodyPos[1], pbBoss::GetMarionette()->GetV2Pos()[0], pbBoss::GetMarionette()->GetV2Pos()[1], 1 , 2.0f ,
				pbComboManager::GetInstance()->GetFever()*10.0f, &(pbBoss::DecreaseHP));*/
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
	TouchLayer::GetInstance().RemovedObserver(this);

	LOGI("pbCharacter::ClearDataStore");
}

void pbCharacter::Release(){
	if( SingleObject != NULL) {
		SingleObject->ClearDataStore();

		delete SingleObject->m_pBodyDrawUnit;
		delete SingleObject->m_pSatelliteDrawUnit;

		delete SingleObject;
		SingleObject = NULL;

		LOGI("pbCharacter::Release");
	}

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

			int left = m_vBodyPos[0] - HalfWidth;
			int right = m_vBodyPos[0] + HalfWidth;
			int top = m_vBodyPos[1] + HalfHeight;
			int bottom = m_vBodyPos[1] - HalfHeight;

			if (x >= left && x <= right) {
				if (y >= bottom && y <= top) {
/*					if( pbComboManager::GetInstance()->FeverOn() ) {
						FeverEffectCancle();	//
						FeverEffectOn();
					}*/
					LOGE("[DEBUG]pbCharacter:: Touched");
				}
			}
		}
}

//--------------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------�޺� �Ŵ���------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------------------------//
/*pbComboManager* pbComboManager::SingleObject = NULL;

pbComboManager::pbComboManager(){
	SingleObject = NULL;

	m_iCombo = 0;
	m_iFever = 0;
	m_iNextFeverCombo = 2;
}

pbComboManager::~pbComboManager() {

}

void pbComboManager::Create() {

	if( SingleObject == NULL)
	{
		SingleObject = new pbComboManager();

		LOGI("pbComboManager::Create() Complete");

		return ;
	}

	LOGE("pbComboManager::Create() Failed");
}

void pbComboManager::LoadData() {
	DataReset();

	//SetVertexIndex(18);
	GetBaseDrawUnit()->SetSizeWH(213,43);
	GetBaseDrawUnit()->SetUVIndex(45);
	GetBaseDrawUnit()->SetV2Pos(445, 400);
	pbRenderProcess::RegistRenderUI(this);

}


void pbComboManager::DataReset()
{
	for(int i = 0; i < MAX_DIGITS; i++)
		m_DigitsNumber[i] = 0;

	m_iFever = 0;
	m_iCombo = 0;
	m_CurrentDigits = 1;
	m_iNextFeverCombo = 2;
}

void pbComboManager::SetVertexIndex(GLuint BodyIndex)
{
//	m_BodyVertexIndex =BodyIndex;

	m_FeverBodyVertexIndex = BodyIndex +1;
	m_NumberVertexIndex = BodyIndex + 2;
	m_FeverNumberVertexIndex = BodyIndex + 3;

	float width = pbDataStorage::GetVertexWidth(m_NumberVertexIndex);

	m_PlacementWidth = width;

	width = pbDataStorage::GetVertexWidth(m_FeverBodyVertexIndex);
	m_fTextPlacementWidth = width/2;

}

void pbComboManager::SetUVIndex(GLuint StartNumberUVIndex)
{
	for(int i = 0; i < NUMBERING; i++)
	{
		m_NumberUVIndex[i] = (StartNumberUVIndex + 2) + i;
		m_FeverNumberUVIndex[i] = (StartNumberUVIndex + 2 + 10) +i;
	}

//	m_BodyUVIndex = StartNumberUVIndex;
	m_FeverBodyUVIndex = StartNumberUVIndex + 1;
}

void pbComboManager::Draw() {
	glPushMatrix();
	glTranslatef(m_vPos[0], m_vPos[1], 0.0f);
	//�޺� �ؽ�Ʈ
		glPushMatrix();
		if( m_iCombo >= m_iNextFeverCombo)
			pbDataStorage::BindVertexAndTexture(m_FeverBodyVertexIndex, m_FeverBodyUVIndex);
		else
			pbDataStorage::BindVertexAndTexture(m_BodyVertexIndex, m_BodyUVIndex);

		//�ؽ�ó ���ε�
		glTranslatef(-35 - m_fTextPlacementWidth, 0,  0);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glPopMatrix();

		//���̺?�� ���̵�� ã�´�
		int count = m_CurrentDigits - 1;
		for(int i = 0 ; i < m_CurrentDigits; ++i)		{
			glPushMatrix();
			if( m_iCombo >= m_iNextFeverCombo)
				pbDataStorage::BindVertexAndTexture(m_FeverNumberVertexIndex, m_FeverNumberUVIndex[m_DigitsNumber[count]]);
			else
				pbDataStorage::BindVertexAndTexture(m_NumberVertexIndex, m_NumberUVIndex[m_DigitsNumber[count]]);
			//�ؽ�ó ���ε�
			glTranslatef( ((float)i)*m_PlacementWidth, 0,  0);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glPopMatrix();

			count--;
		}
	glPopMatrix();
}

void pbComboManager::Update(float fTime) {

}

void pbComboManager::IncreaseCombo(int Score) {
	m_iCombo++;
//	pbEffectProcess::pEffectProcessor->AddEffect(EFFECTTYPE::COMBO, 400, 240, 1.0f, m_iCombo);

	pbGlobalInGameVariable::PlusScore += Score * (m_iFever+1);

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
				pbGlobalInGameVariable::fWorldMoveSpeed *= 1.20f;
			else if(m_iFever == 2)
				pbGlobalInGameVariable::fWorldMoveSpeed *= 1.18f;
			else if(m_iFever == 3)
				pbGlobalInGameVariable::fWorldMoveSpeed *= 1.171f;
			else if(m_iFever == 4)
				pbGlobalInGameVariable::fWorldMoveSpeed *= 1.149f;
			else if(m_iFever == 5)
				pbGlobalInGameVariable::fWorldMoveSpeed *= 1.134f;

		}
		else if( m_iNextFeverCombo >= 32 ) {
			pbGlobalInGameVariable::fWorldMoveSpeed *= 1.11f;
		}
		m_iCombo = 0;
		m_iFever++;


		for(int i = 0; i < MAX_DIGITS; i++)
			m_DigitsNumber[i] = 0;

		m_CurrentDigits = 1;
		//�ǹ� ����Ʈ �۵�
		npAudioSystem::playEffect(3);
		pbUserData::SingleObject->UsingAbilityPoint(-100*m_iFever);
		pbEffectProcess::GetInstance()->AddStepUpEffect(400, 220, 22, 67, 1.0f);
		return true;
	}
	else {
		//�ǹ� ����Ʈ �۵� �Ұ�
		npAudioSystem::playEffect(5);
		pbEffectProcess::GetInstance()->AddStickerEffect(pbCharacter::GetInstance()->GetPosX(), pbCharacter::GetInstance()->GetPosY(), 43, 3.0f);
		return false;
	}

	return false;
}

void pbComboManager::ResetCombo() {
	pbGlobalInGameVariable::fWorldMoveSpeed = WORLD_MOVESPEED;

	npAudioSystem::playEffect(4);
	 DataReset();

	 pbCharacter::GetInstance()->FeverEffectCancle();

	 pbEffectProcess::GetInstance()->AddMissEffect();
}

void pbComboManager::ClearDataStore() {
	pbRenderProcess::RemoveRenderUI(this);
	LOGI("pbComboManager ClearDataStore");
}

void pbComboManager::Release() {
	if( SingleObject != NULL)	{
		SingleObject->ClearDataStore();
		delete SingleObject;

		SingleObject = NULL;

		LOGI("pbComboManager Release");
	}

}*/

