
#include "pbBoss.h"
//---------------------------------------------- pbBoss------------------------------------------------------------------------------------//
pbBoss* pbBoss::SingleObject = NULL;

pbBoss::pbBoss() {
	m_bBattlePhase = false;
	m_bBossAlive = true;

	m_fHP = 100.f;
	m_pMarionette = NULL;
	m_pBodyDrawUnit = NULL;
}

pbBoss::~pbBoss() {

}

void pbBoss::Create() {
	if( SingleObject == NULL ) {
		SingleObject = new pbBoss();

		SingleObject->m_pMarionette = new pbMarionette();
		SingleObject->m_pBodyDrawUnit = new pbBasicDrawUnit();
	}
}

void pbBoss::LoadData() {
	m_pMarionette->AddLineMoveState(APPROACHING, -200, 0, &(pbBoss::ApproachingCondition));
	m_pMarionette->AddLineMoveState(WEAVING_UP,0, 30, &(pbBoss::WeavingUpCondition));
	m_pMarionette->AddLineMoveState(WEAVING_DOWN,0, -30, &(pbBoss::WeavingDownCondition));
	m_pMarionette->AddZigZagMoveState(WALKOUT, 0, -200, 10, 80, &(pbBoss::WalkOutCondition));

	m_pBodyDrawUnit->SetTextureTAG("run");
	m_pBodyDrawUnit->SetSize(400, 400);

	m_pMarionette->SelectMoveState(NONE);
	m_pMarionette->SetMovePause(false);

	m_bBattlePhase = false;
	m_bBossAlive = true;
	m_fHP = 100.f;
}

void pbBoss::Update(float fTime) {
	int m_MarionetteState = m_pMarionette->GetState();

	if( m_MarionetteState == APPROACHING) {
		if( m_pMarionette->GetActionCondition() ) {
			m_pMarionette->SelectMoveState(WEAVING_UP);
			m_bBattlePhase = true;
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

			m_pMarionette->SetMovePause(true);
			m_bBossAlive = false;

			pbSceneManager::getInstance().RemoveRenderToCurrentScene(this);

			pbSceneManager::getInstance().GetCurrentScene()->GetStageTrigger()->ActivateIDState(pbCharacter::WALKOUT);
			LOGE("CHANGE TO DIE");
		}
	}

	m_pMarionette->MoveUpdate(fTime);

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
		GetMarionette()->SetPosX(-300);
		return true;
	}
	return false;
}



void pbBoss::PreSettingDraw() {
	glPushMatrix();
		m_pMarionette->Translate();
		m_pBodyDrawUnit->PreSettingDraw();
}

void pbBoss::DrawThis() {
		glColor4f(1.0f , 0.0f, 0.0f, 1.0f);
		m_pBodyDrawUnit->DrawThis();
		glColor4f(1.0f , 1.0f, 1.0f, 1.0f);
	glPopMatrix();
}

void pbBoss::Approaching() {
	GetInstance()->m_pMarionette->SelectMoveState(APPROACHING);
	GetInstance()->m_pMarionette->SetMovePause(false);
	LOGE("pbBoss::Approaching() CHANGE TO APPROACHING");
}

void pbBoss::DecreaseHP(float fDamage){
	if( GetInstance() != NULL) {
		GetInstance()->m_fHP -= fDamage;

		if( GetInstance()->m_fHP < 0) {
			GetInstance()->m_pMarionette->SelectMoveState(WALKOUT);
			LOGE("CHANGE TO WALKOUT");
			GetInstance()->SetBattlePhase(false);
//			pbNoteProcessor::GetNoteDropper()->SetGenerateNote(false);
		}

		LOGE("DAMAGE :");
		LOGfloatString("CurrentHP :", GetInstance()->m_fHP);
	}
}

void pbBoss::ClearDataStore() {
	m_pMarionette->ClearDataStore();
	pbSceneManager::getInstance().RemoveRenderToCurrentScene(this);

	LOGI("pbBoss::ClearDataStore");
}

void pbBoss::SetState(int State) {
	m_pMarionette->SelectMoveState(State);
}

void pbBoss::Release() {
	if( SingleObject != NULL ) {
		delete SingleObject->m_pMarionette;
		SingleObject->m_pMarionette = NULL;

		delete SingleObject->m_pBodyDrawUnit;
		SingleObject->m_pBodyDrawUnit = NULL;

		delete SingleObject;
		SingleObject = NULL;
	}
}
