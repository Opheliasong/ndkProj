
#include "pbBoss.h"

//-------------------------------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------- pbBoss------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------------------------------//
pbBoss* pbBoss::SingleObject = NULL;

pbBoss::pbBoss() {
	m_bBattlePhase = false;
	m_bBossAlive = true;

	m_pMarionette = NULL;
	m_pBodyDrawUnit = NULL;
	m_pHPMeter = NULL;
	m_pTargetStamp = NULL;

	m_fpTouchFunc = NULL;
}

pbBoss::~pbBoss() {

}

void pbBoss::Create() {
	if( SingleObject == NULL ) {
		SingleObject = new pbBoss();

		SingleObject->m_pMarionette = new pbMarionette();
		SingleObject->m_pBodyDrawUnit = new pbBasicDrawUnit();
		SingleObject->m_pHPMeter = new pbHPMeter();
		SingleObject->m_pTargetStamp = new pbTargetStamp();
	}
}

void pbBoss::LoadData() {
	m_pMarionette->AddLineMoveState(APPROACHING, -200, 0, &(pbBoss::ApproachingCondition));
	m_pMarionette->AddLineMoveState(WEAVING_UP,0, 30, &(pbBoss::WeavingUpCondition));
	m_pMarionette->AddLineMoveState(WEAVING_DOWN,0, -30, &(pbBoss::WeavingDownCondition));
	m_pMarionette->AddZigZagMoveState(WALKOUT, 0, -200, 10, 80, &(pbBoss::WalkOutCondition));

	m_pBodyDrawUnit->SetTextureTAG("run");
	m_pBodyDrawUnit->SetSize(200, 200);

	m_pMarionette->SelectMoveState(NONE);
	m_pMarionette->SetMovePause(false);

	m_bBattlePhase = false;
	m_bBossAlive = true;

	m_pHPMeter->Initialize(100, 20);
	m_pHPMeter->SetTag("run", "run", 600, 30);
	m_pHPMeter->SetPos(400, 30);

	m_pTargetStamp->SetTag("ci", m_pBodyDrawUnit->getWidth() * 1.2f , m_pBodyDrawUnit->getHeight() * 1.2f );
	m_pTargetStamp->SetRotateValue(1.0f);
	m_pTargetStamp->SetLimitTime(5.0f);
	m_pTargetStamp->GetTrigger()->SetTriggerCondition(3.0f, 1.0f);

	SetTouchFunc(&(pbBoss::Game_TouchFunc));
	TouchLayer::GetInstance().RegistedObserver(this);
}

void pbBoss::SetState(int State) {
	m_pMarionette->SelectMoveState(State);
}

void pbBoss::PreSettingDraw() {
	glPushMatrix();
		m_pMarionette->Translate();
}

void pbBoss::DrawThis() {
		glPushMatrix();
		m_pBodyDrawUnit->PreSettingDraw();
		glColor4f(1.0f , 0.0f, 0.0f, 1.0f);	//임시
		m_pBodyDrawUnit->DrawThis();
		glColor4f(1.0f , 1.0f, 1.0f, 1.0f);
		glPopMatrix();

		m_pTargetStamp->Draw();
	glPopMatrix();


	m_pHPMeter->Draw();
}


void pbBoss::Update(float fTime) {
	int m_MarionetteState = m_pMarionette->GetState();

	m_pMarionette->MoveUpdate(fTime);

	if( m_MarionetteState == APPROACHING) {
		if( m_pMarionette->GetActionCondition() ) {
			m_pMarionette->SelectMoveState(WEAVING_UP);
			m_bBattlePhase = true;
			GetHPMeter()->OpenMeter();
			m_pTargetStamp->StampingStart();
		}
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
			m_pMarionette->SelectMoveState(DIE);
			m_bBossAlive = false;
			pbSceneManager::getInstance().GetCurrentScene()->GetStageTrigger()->ActivateIDState(pbBoss::DIE);
			LOGE("CHANGE TO DIE");
		}
	}

	m_pHPMeter->Update(fTime);

	m_pTargetStamp->Update(fTime);
}


void pbBoss::ClearDataStore() {
	m_pMarionette->ClearDataStore();
	//pbSceneManager::getInstance().RemoveRenderToCurrentScene(this);
	TouchLayer::GetInstance().RemovedObserver(this);

	LOGI("pbBoss::ClearDataStore");
}

void pbBoss::Release() {
	if( SingleObject != NULL ) {
		delete SingleObject->m_pMarionette;
		SingleObject->m_pMarionette = NULL;

		delete SingleObject->m_pBodyDrawUnit;
		SingleObject->m_pBodyDrawUnit = NULL;

		delete SingleObject->m_pHPMeter;

		delete SingleObject->m_pTargetStamp;

		delete SingleObject;
		SingleObject = NULL;
	}
}

////------------------------------터치----------------------------------------------//
void pbBoss::notify() {
	if (TouchLayer::GetInstance().touchFlag == TOUCHFLAGS::TAPDOWN) {
		//		LOGE("[DEBUG]pbCharacter:: TAPDOWN");
		int x = TouchLayer::GetInstance().pointX;
		int y = TouchLayer::GetInstance().pointY;
		//		LOGfloatString("X", x);
		//		LOGfloatString("Y", y);

		int HalfWidth = m_pBodyDrawUnit->getWidth() / 2;
		int HalfHeight = m_pBodyDrawUnit->getHeight() / 2;

		float* pV2Pos = m_pMarionette->GetV2Pos();

		int left = pV2Pos[0] - HalfWidth;
		int right = pV2Pos[0] + HalfWidth;
		int top = pV2Pos[1] + HalfHeight;
		int bottom = pV2Pos[1] - HalfHeight;

		if (x >= left && x <= right) {
			if (y >= bottom && y <= top) {
				if (m_fpTouchFunc != NULL)
					(*m_fpTouchFunc)();
				LOGE("[DEBUG]pbBoss:: Touched");
			}
		}
	}
}

void pbBoss::Game_TouchFunc() {
	if( GetTargetStamp()->IsTargetOn() ){
		GetTargetStamp()->ReleaseTarget();
		if( pbBoss::GetInstance()->IsBattlePhase() ) {
			pbBoss::GetInstance()->DecreaseHP(50.0f);
			pbStageValue::IncreaseScore(1234);
		}
/*				pbEffectManager::GetInstance()->AddHomingMissileEffect(m_pMarionette->GetV2Pos()[0], m_pMarionette->GetV2Pos()[1], pbBoss::GetMarionette()->GetV2Pos()[0], pbBoss::GetMarionette()->GetV2Pos()[1], "run", 40, 40, 0.5f ,
						5.0f, &(pbBoss::DecreaseHP));*/
	}
}

///---------------마리오네트 컨디션------------------------------//

bool pbBoss::ApproachingCondition(float* pV2Pos) {
	if(pV2Pos[0] < GetInstance()->m_vConditionPos[0]) {
		GetMarionette()->SetPosX(GetInstance()->m_vConditionPos[0]);
		return true;
	}

	return false;
}
bool pbBoss::WeavingUpCondition(float* pV2Pos) {
	if(pV2Pos[1] > GetInstance()->m_vConditionPos[1] + 40) {
		GetMarionette()->SetPosY(GetInstance()->m_vConditionPos[1] + 40);
		return true;
	}
	return false;
}
bool pbBoss::WeavingDownCondition(float* pV2Pos) {
	if(pV2Pos[1] < GetInstance()->m_vConditionPos[1] - 40) {
		GetMarionette()->SetPosY(GetInstance()->m_vConditionPos[1] - 40);
		return true;
	}
	return false;
}
bool pbBoss::WalkOutCondition(float* pV2Pos) {
	if(pV2Pos[1] < -300) {
		GetMarionette()->SetPosY(-300);
		return true;
	}
	return false;
}


/////---------------스테이지 트리거 함수------------------------------//
void pbBoss::Approaching() {
	GetMarionette()->SelectMoveState(APPROACHING);
	GetMarionette()->SetMovePause(false);

	LOGE("pbBoss::Approaching() CHANGE TO APPROACHING");
}

void pbBoss::PostDieProcess() {
	GetMarionette()->SetMovePause(true);
//	pbSceneManager::getInstance().RemoveRenderToCurrentScene(GetInstance());
	pbSceneManager::getInstance().GetCurrentScene()->GetStageTrigger()->ActivateIDState(pbCharacter::WALKOUT);
}

/////---------------HP감소 정적함수------------------------------//
void pbBoss::DecreaseHP(float fDamage){
	if( GetInstance() != NULL) {
		GetHPMeter()->DecreaseHP(fDamage);

		if( GetHPMeter()->IsDied()) {
			GetMarionette()->SelectMoveState(WALKOUT);
			LOGE("CHANGE TO WALKOUT");
			GetInstance()->SetBattlePhase(false);
			GetTargetStamp()->StampingStop();
		}

		LOGfloatString("CurrentHP :", GetHPMeter()->GetTotalHP());
	}
}

