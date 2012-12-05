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
///////---------------------------------------------------------------------Get&Set-----------------------------------------------------------------------------------------------//
void pbCharacter::SetPos(float X, float Y){
	m_pMarionette->SetPosX(X);
	m_pMarionette->SetPosY(Y);
}

float* pbCharacter::GetPos() { return m_pMarionette->GetV2Pos(); }
/*inline float pbCharacter::GetPosX() { return m_pMarionette->GetV2Pos()[0]; }
inline float pbCharacter::GetPosY() { return m_pMarionette->GetV2Pos()[1]; }*/

float pbCharacter::GetHeight() {return m_pBodyDrawUnit->getHeight(); }

///////---------------------------------------------------------------------Update-----------------------------------------------------------------------------------------------//
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
			m_pMarionette->SelectMoveState(NEXT_SCENE);
			m_pMarionette->SetMovePause(true);
		}
	}
	else if( m_MarionetteState == NEXT_SCENE) {
		if( m_pMarionette->GetActionCondition() ) {
			m_pMarionette->SelectMoveState(NONE);

			pbSceneNavigator::GetInstance().SearchAndReadyToMoveScene(SCENESTATE::ACTION_FOWARD);
		}
	}

	m_pMarionette->MoveUpdate(fTime);
}

///////---------------------------------------------------------------------StageTrigger-----------------------------------------------------------------------------------------------//
void pbCharacter::Appeared() {
	GetInstance()->m_pMarionette->SelectMoveState(APPEARED);
	GetInstance()->m_pMarionette->SetMovePause(false);

	LOGE("pbCharacter::Appeared() APPEARED");
}

void pbCharacter::WalkOut() {
	GetInstance()->m_pMarionette->SelectMoveState(WALKOUT);
	LOGE("pbCharacter::Appeared() WalkOut");
}

///////---------------------------------------------------------------------Fever-----------------------------------------------------------------------------------------------//
void pbCharacter::FeverEffectOn() {
	m_iFeverEffectMode = FEVER_EXPAND;
	m_fFeverEffectDistance = 0.0f;
	m_fFeverDestDistance = FEVER_DISTANCE_EXPLOSION;

	if( pbBoss::GetInstance()->IsBattlePhase() )
		pbEffectManager::GetInstance()->AddHomingMissileEffect(m_pMarionette->GetV2Pos()[0], m_pMarionette->GetV2Pos()[1], pbBoss::GetMarionette()->GetV2Pos()[0], pbBoss::GetMarionette()->GetV2Pos()[1], "run", 40, 40, 0.5f ,
				5.0f, &(pbBoss::DecreaseHP));
}

/*void pbCharacter::FeverEffectReady() {
	m_iFeverEffectMode = FEVER_EXPAND;
	m_fFeverEffectDistance = 0.0f;
	m_fFeverDestDistance = FEVER_DISTANCE_READY;

	m_bFeverReady = true;

	m_fFeverTargetTime = 0.0f;
	m_fEffectScale = 3.0f;

	LOGE("FEVER READY");
}*/

/*void pbCharacter::FeverEffectCancle() {
	m_iFeverEffectMode = FEVER_NONE;
	m_fFeverEffectDistance = 0.0f;
	m_fFeverDestDistance = 0.0f;

	m_Color.G = 1.0f;
	m_Color.B = m_Color.G;

	m_bFeverReady = false;
}*/
///////---------------------------------------------------------------------Touch---------------------------------------------------------------------------------------------//

void pbCharacter::PlayGame_TouchFunc() {
	if( pbComboManager::GetInstance()->FeverOn() ) {
	//	pbCharacter::GetInstance()->FeverEffectCancle();
		pbCharacter::GetInstance()->FeverEffectOn();
	}
}
//결과 화면 터치 함수
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

///////---------------------------------------------------------------------Touch---------------------------------------------------------------------------------------------//
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

///////---------------------------------------------------------------------Base---------------------------------------------------------------------------------------------//
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


