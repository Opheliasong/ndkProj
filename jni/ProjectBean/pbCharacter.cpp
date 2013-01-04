#include "pbCharacter.h"

using namespace projectBean;


pbCharacter* pbCharacter::SingleObject = NULL;

pbCharacter::pbCharacter(): m_fLifeRotate(0.0f){
	m_Color.Init(1.0f, 1.0f, 1.0f, 1.0f);
//	m_fLerp = 0.0f;
	m_BodyVertexIndex = 0;
	m_BodyUVIndex = 0;

	m_vBodyPos[0] = 0.0f;
	m_vBodyPos[1] = 0.0f;

	m_fBodyWidth = 0.0f;
	m_fBodyHeight = 0.0f;

	m_LifeVertexIndex = 0;
	m_LifeUVIndex = 0;

	m_fLifePosX = 0.0f;

//	m_ppfFollowLineVertexPointer = NULL;

	m_iFeverEffectMode = FEVER_NONE;
	m_fFeverEffectDistance = 0.0f;
	m_bFeverReady = false;
	m_fFeverTime = 0.0f;
	m_fFeverDestDistance = 0.0f;
	m_fFeverTargetTime = 0.0f;
	m_fEffectScale = 3.0f;

	m_BodyVertexIndex = 0; //�ٵ��� ���ؽ����� index
	m_BodyUVIndex = 0; //�ٵ��� UV ���� index

	m_EffectVertexIndex = 0;
	m_EffectUVIndex = 0; //����Ʈ
}
pbCharacter::~pbCharacter(){

}

void pbCharacter::SetVertexIndex(GLuint BodyIndex, GLuint LifeIndex, GLuint EffectIndex){
	m_BodyVertexIndex = BodyIndex;
	m_LifeVertexIndex = LifeIndex;
	m_EffectVertexIndex = EffectIndex;
}
void pbCharacter::SetUVIndex(GLuint BodyIndex, GLuint LifeIndex, GLuint EffectIndex){
	m_BodyUVIndex = BodyIndex;
	m_LifeUVIndex = LifeIndex;
	m_EffectUVIndex = EffectIndex;
}

void pbCharacter::Create(){
	//---------------------���ʷ����� ��� ���-----------------------//
	if( SingleObject == NULL){
		SingleObject = new pbCharacter();

		LOGI("CHARACTER Create Complete");

		return;
	}

	LOGE("CHARACTER Create Failed");
}

void pbCharacter::LoadData() {
//	pbRenderProcess::RegistRenderCharacter(this);
//	pbTouchLayer::registerObserver(this);

	SetVertexIndex(5 ,6, 1);
	SetUVIndex(21, 22, 19);

	m_fBodyWidth = pbDataStorage::GetVertexWidth(m_BodyVertexIndex);
	m_fBodyHeight = pbDataStorage::GetVertexHeight(m_BodyVertexIndex);

	m_TouchArea.setTouchArea(m_fBodyWidth*1.2f, m_fBodyHeight*1.2f );

	m_fLifePosX = m_fBodyWidth*0.43f;

	SetPos(72.0f, 240.0f);
}

void pbCharacter::Draw(){
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_TEXTURE_2D);	//2D

		glColor4f(1.0f, 1.0f,1.0f, m_Color.A);

		glPushMatrix();

		pbDataStorage::BindVertexAndTexture(m_BodyVertexIndex, m_BodyUVIndex);
		glTranslatef(m_vBodyPos[0] , m_vBodyPos[1], 0.f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		pbDataStorage::BindVertexAndTexture(m_LifeVertexIndex, m_LifeUVIndex);
		for(int i = 0; i < pbGlobalInGameVariable::NumLife ; i++)
		{
			glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
			//�ؽ�ó ���ε�
			glPushMatrix();
			glRotatef((float(i)*120.0f) + m_fLifeRotate*360.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(m_fLifePosX + m_fFeverEffectDistance, 0.0f, 0.f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glPopMatrix();
		}

	/*	if( m_bFeverReady) {
			pbDataStorage::BindVertexAndTexture(m_EffectVertexIndex, m_EffectUVIndex);
			glPushMatrix();
			glRotatef(m_fLifeRotate*360.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef( 0.0f, 0.0f, 0.f);
			glScalef(m_fEffectScale, m_fEffectScale, 1.0f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glPopMatrix();
		}*/

		glPopMatrix();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);	//2D
}

void pbCharacter::Update(float fTime){
	static float fPartOfLine, fWholeOfLine = 0.0f;

	m_fLifeRotate += (pbGlobalInGameVariable::fWorldMoveSpeed/400)*fTime;

//	fPartOfLine = m_fBodyPosX -  m_ppfFollowLineVertexPointer[0][0];
//	fWholeOfLine = m_ppfFollowLineVertexPointer[1][0] - m_ppfFollowLineVertexPointer[0][0];

//	m_fLerp = fPartOfLine/fWholeOfLine;

//	if( m_fLerp < 0.0f)
//		m_fLerp = 0.0f;

//	m_fBodyPosY = Lerp(m_ppfFollowLineVertexPointer[1][1], m_ppfFollowLineVertexPointer[0][1], 1.0f - m_fLerp);

	m_TouchArea.movePositon(m_vBodyPos[0], m_vBodyPos[1]);

	//--------------------ī�޶� ����ũ-------------------------------//
/*	if( pbGlobalInGameVariable::bCameraShake ) {
		pbGlobalInGameVariable::fCameraShakeTime += fTime;

		float WaveFrequency = 20.0f;// * (1/(pbGlobalInGameVariable::fCameraShakeTime*pbGlobalInGameVariable::fCameraShakeTime));
		float WaveLength =  1/( (pbGlobalInGameVariable::fCameraShakeTime*pbGlobalInGameVariable::fCameraShakeTime) * 30 );
		CameraShake(&pbGlobalInGameVariable::fCameraShakeX, &pbGlobalInGameVariable::fCameraShakeY, WaveFrequency , WaveLength, pbGlobalInGameVariable::fCameraShakeTime);

		if( pbGlobalInGameVariable::fCameraShakeTime >= 2.0f) {
			pbGlobalInGameVariable::bCameraShake = false;
			pbGlobalInGameVariable::fCameraShakeTime = 0.0f;
		}

	}*/

	///---------------�ǹ� ����Ʈ------------//
	if( m_bFeverReady) {
		//---�ǹ� Ÿ�� ����Ʈ---/
		m_fFeverTargetTime += (1/0.5)*fTime;
		if( m_fFeverTargetTime > 1.0f ) {
			m_fFeverTargetTime = 1.0f;
		}
		m_fEffectScale = Lerp(3.0f, 1.6f, m_fFeverTargetTime );
		//---�ǹ� ��� ����Ʈ---/
		if(m_iFeverEffectMode == FEVER_EXPAND) {
			m_fFeverTime += (1/0.2f)*fTime;
			m_Color.G = 1.0f - m_fFeverTime*0.5f;
			m_Color.B = m_Color.G;
			m_fFeverEffectDistance = Lerp(-10, FEVER_DISTANCE_READY, m_fFeverTime);

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
			m_fFeverEffectDistance = Lerp(FEVER_DISTANCE_READY, -10, m_fFeverTime);

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
			m_fFeverEffectDistance = Lerp(0, FEVER_DISTANCE_EXPLOSION, m_fFeverTime);

			if( m_fFeverTime > 1.0f ) {
				m_fFeverTime = 0.0f;
				m_fFeverEffectDistance = FEVER_DISTANCE_EXPLOSION;
				m_iFeverEffectMode = FEVER_RETURN;
			}
		}
		else if(m_iFeverEffectMode == FEVER_RETURN) {
			m_fFeverTime += (1/0.7)*fTime;
			m_fFeverEffectDistance = Lerp(FEVER_DISTANCE_EXPLOSION, 0, m_fFeverTime);

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

//	pbEffectProcess::GetInstance()->AddFeverAvailableEffect(400, 320, 23, 73, 0.7f );
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
	pbTouchLayer::removeObserver(this);
	LOGI("pbCharacter::ClearDataStore");
}

void pbCharacter::Release(){
	if( SingleObject != NULL) {
		SingleObject->ClearDataStore();

		delete SingleObject;
		SingleObject = NULL;

		LOGI("pbCharacter::Release");
	}

}

void pbCharacter::notify(){
	switch (TouchLayer::GetInstance().touchFlag) {
		case projectBean::TOUCHSTATUS::TAPDOWN: {

			if (	CompareTouchPointInArea(TouchLayer::GetInstance().pointX, TouchLayer::GetInstance().pointY)	) {
				if( pbComboManager::GetInstance()->FeverOn() ) {
					FeverEffectCancle();	//
					FeverEffectOn();
				}
			}//end if
			break;
		}//end case

	}//end switch
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

