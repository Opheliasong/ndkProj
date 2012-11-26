
#include "pbBoss.h"

//-----------------------------------------------------������Ʈ ����-----------------------------------------------------------------------------//
float* pbLineMover::GetV2PosByTime(float fAccumulateTime) {
	m_vPos[0] = m_vStartPos[0] + m_vDir[0] * fAccumulateTime;
	m_vPos[1] = m_vStartPos[1] + m_vDir[1] * fAccumulateTime;

	return m_vPos;
}

float* pbZigZagMover::GetV2PosByTime(float fAccumulateTime) {
	m_vPos[0] = m_vStartPos[0] + m_vDir[0]* fAccumulateTime + ( m_fAmplitudeX * sin( m_fCycle * fAccumulateTime )  );
	m_vPos[1] = m_vStartPos[1] + m_vDir[1]* fAccumulateTime + ( m_fAmplitudeY * sin( m_fCycle * fAccumulateTime )  );

	return m_vPos;
}


//-----------------------------------------------------��������Ʈ-----------------------------------------------------------------------------//

pbMarionette::pbMarionette() {
	m_vPos[0] = 0;
	m_vPos[1] = 0;
	m_pCurrentMover = NULL;
	m_pMoverHead = new pbObjectMoverList;
	m_pMoverHead->setHeader();
	m_CurrentState = -1;
	m_fAccumulateTime = 0.0f;
	m_bUpdatePause = false;
}
pbMarionette::~pbMarionette() {
	ClearDataStore();
	delete m_pMoverHead;
}

void pbMarionette::AddLineMoveState(int ID, float dirX, float dirY){
	pbObjectMover* createMover = new pbLineMover(ID, dirX, dirY);

	pbObjectMoverList* pTargetNode =  pbObjectMoverList::makeLinkNode(createMover);
	pbObjectMoverList::addTail(pTargetNode, m_pMoverHead);
}

void pbMarionette::AddZigZagMoveState(int ID, float dirX, float dirY, float fAmplitude, float fCycle){
	pbObjectMover* createMover = new pbZigZagMover(ID, dirX, dirY, fAmplitude, fCycle);

	pbObjectMoverList* pTargetNode =  pbObjectMoverList::makeLinkNode(createMover);
	pbObjectMoverList::addTail(pTargetNode, m_pMoverHead);
}

void pbMarionette::SelectMoveState(int ID) {
	pbObjectMoverList* iterator;
	pbObjectMoverList* head = m_pMoverHead;
	iterator = head->getNext();
	while (iterator != head) {
		pbObjectMover* pkernel = iterator->getKernel();
		iterator = iterator->getNext();

		if( pkernel->GetID() == ID ) {
			m_pCurrentMover = pkernel;
			m_pCurrentMover->SetStartPos(m_vPos[0], m_vPos[1]);
			m_CurrentState = ID;
			m_fAccumulateTime = 0.0f;
			return;
		}

	}

	//ID�� ������ NULL
	m_pCurrentMover = NULL;
	m_CurrentState = ID;
	m_fAccumulateTime = 0.0f;
}

void pbMarionette::DeleteMoveState(int ID) {
	pbObjectMoverList* iterator;
	pbObjectMoverList* head = m_pMoverHead;
	iterator = head->getNext();
	while (iterator != head) {
		pbObjectMover* pkernel = iterator->getKernel();
		iterator = iterator->getNext();

		if( pkernel->GetID() == ID ) {
			pbObjectMoverList::destroyNode(iterator);
			delete pkernel;
			return;
		}

	}
}

void pbMarionette::ClearDataStore() {
	LinkNodeDeleteAllKernel(pbObjectMover*, m_pMoverHead);
	pbObjectMoverList::clearList(m_pMoverHead);
}

void pbMarionette::MoveUpdate(float fTime) {
	if( !m_bUpdatePause ) {
		if( m_pCurrentMover != NULL) {
			m_fAccumulateTime += fTime;
			float* pMoverV2Pos =  m_pCurrentMover->GetV2PosByTime(m_fAccumulateTime);
			m_vPos[0] = pMoverV2Pos[0];
			m_vPos[1] = pMoverV2Pos[1];

//			LOGE("DEBUG pbMarionette::MoveUpdate ");
		}
		else
			LOGE("pbMarionette::Translate() CurrentState is NULL");
	}
}

void pbMarionette::Translate() {
	glTranslatef(m_vPos[0], m_vPos[1], 0);
}

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
	m_pMarionette->AddLineMoveState(APPROACHING, pbGlobalInGameVariable::fWorldMoveDir*200, 0);
	m_pMarionette->AddLineMoveState(WEAVING_UP,0, 30);
	m_pMarionette->AddLineMoveState(WEAVING_DOWN,0, -30);
	m_pMarionette->AddZigZagMoveState(WALKOUT, 0, -200, 10, 80);

	m_pBodyDrawUnit->SetTextureTAG("run");
	m_pBodyDrawUnit->SetSize(400, 400);

	m_pMarionette->SetPosX(1400);
	m_pMarionette->SetPosY(240);

	m_pMarionette->SelectMoveState(APPROACHING);
	LOGE("CHANGE TO APPROACHING");

}

void pbBoss::Update(float fTime) {
	int m_BossState = m_pMarionette->GetState();

	if( m_BossState == APPROACHING) {
		if(m_pMarionette->GetV2Pos()[0] < 800) {

			m_pMarionette->SetPosX(800);
			m_pMarionette->SelectMoveState(WEAVING_UP);
//			LOGE("CHANGE TO WEAVING_UP");

			m_bBattlePhase = true;
			//pbNoteProcessor::GetNoteDropper()->SetGenerateNote(true);
		}

	}
	else if( m_BossState == WEAVING_UP) {
		if(m_pMarionette->GetV2Pos()[1] > 280) {

			m_pMarionette->SelectMoveState(WEAVING_DOWN);
//			LOGE("CHANGE TO WEAVING_DOWN");
		}
	}
	else if( m_BossState == WEAVING_DOWN) {
		if(m_pMarionette->GetV2Pos()[1] < 200) {
			m_pMarionette->SelectMoveState(WEAVING_UP);

//			LOGE("CHANGE TO WEAVING_UP");
		}
	}
	else if( m_BossState == WALKOUT) {
		if(m_pMarionette->GetV2Pos()[1] < -300) {
			m_pMarionette->SelectMoveState(DIE);

			m_pMarionette->SetMovePause(true);
			m_bBossAlive = false;

			pbSceneManager::getInstance().RemoveRenderToCurrentScene(this);
			LOGE("CHANGE TO DIE");
		}
	}


	m_pMarionette->MoveUpdate(fTime);

}

void pbBoss::PreSettingDraw() {
	m_pMarionette->Translate();

	m_pBodyDrawUnit->PreSettingDraw();
}

void pbBoss::DrawThis() {
	glColor4f(1.0f , 0.0f, 0.0f, 1.0f);
	m_pBodyDrawUnit->DrawThis();
	glColor4f(1.0f , 1.0f, 1.0f, 1.0f);
}

void pbBoss::DecreaseHP(float fDamage){
	if( GetInstance() != NULL) {
		GetInstance()->m_fHP -= fDamage;

		if( GetInstance()->m_fHP < 0) {
			GetInstance()->m_pMarionette->SelectMoveState(WALKOUT);
			LOGE("CHANGE TO WALKOUT");

//			pbNoteProcessor::GetNoteDropper()->SetGenerateNote(false);
		}

		LOGE("DAMAGE :");
		LOGfloatString("CurrentHP :", GetInstance()->m_fHP);
	}
}

void pbBoss::ClearDataStore() {
	m_bBattlePhase = false;
	m_pMarionette->ClearDataStore();
	pbSceneManager::getInstance().RemoveRenderToCurrentScene(this);

	LOGI("pbBoss::ClearDataStore");
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
