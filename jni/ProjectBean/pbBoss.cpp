
#include "pbBoss.h"

//---------------------------------------------- pbBossHpMeter------------------------------------------------------------------------------------//
pbHPMeter::pbHPMeter() {
	m_Phase = PHASE_NONE;
	m_vPos[0] = 0;
	m_vPos[1] = 0;

	m_bDie = false;
	m_fOneLineHP = 0;
	m_fTotalHP = 0;


	////---------------게이지 변수-------------------////
	//--게이지 컨트롤 변수--//
	m_bGaugeSetting = false;
	m_bGaugeChangeStart = false;
	m_fDecreaseLinePercent  = 0.0f;
	m_iLineCount = 0;
	//--선형보간 변수--//
	m_fAniSpeed = 0.0f;
	m_fAniTime = 0.0f;
	m_fDestPercent = 1.0f;
	m_fStartPercent = 1.0f;
	m_fDrawPercent = 1.0f;
	m_fDrawPosX = 0.0f;

	//--Drawing 관련 변수--//
	m_fHorizonScale = 0.0f;
	m_fVerticalScale = 0.0f;
	m_fGaugeHalfWidth = 0.0f;
	m_fGaugeHalfHeight = 0.0f;

	m_GaugeUVBindID = 0;

	m_GaugeUV_WidthPercent = 0.0f;

	m_pBaseDrawUnit = new pbBasicDrawUnit();
	m_pUVPacket = NULL;
}

pbHPMeter::~pbHPMeter() {
	delete m_pBaseDrawUnit;
}

void pbHPMeter::Initialize(float fTotalHP, float fOneLineHP) {
	m_Phase = PHASE_NONE;
	m_fHorizonScale = 0.0f;
	m_fVerticalScale = 0.0f;
	m_fAniSpeed = 0.0f;
	m_fAniTime = 0.0f;

	SetCurrentHP(fTotalHP, fOneLineHP);
}


void pbHPMeter::SetCurrentHP(float HP, float fOneLineHP) {
	m_bDie = false;
	m_fTotalHP = HP;
	m_fOneLineHP = fOneLineHP;
	m_iLineCount = 0;

	float fCurrentHP = HP;
	while(fCurrentHP > m_fOneLineHP) {
		m_iLineCount++;
		fCurrentHP -= m_fOneLineHP;
	}

	m_fDestPercent = fCurrentHP/m_fOneLineHP;
	m_fDrawPercent = m_fDestPercent;
	m_fStartPercent = m_fDrawPercent;

	m_fDecreaseLinePercent = 0.0f;
	m_bGaugeChangeStart = false;
	m_bGaugeSetting = false;
}

void pbHPMeter::SetTag(screenplayTag PanelTag, screenplayTag MeterTag, float fWidth, float fHeight) {
	m_pBaseDrawUnit->SetTextureTAG(PanelTag);
	m_pBaseDrawUnit->SetSize(fWidth, fHeight);

	//크기 설정
	SetVertexByCenter(m_GaugeVertex, fWidth, fHeight);
	m_fGaugeHalfWidth =  fWidth/2;
	m_fGaugeHalfHeight =  fHeight/2;

	//UV 추출
	sprite* pSprite = npContainerDAO::GetInstance().getSpriteByTAG(MeterTag);

	int index = pSprite->currentScreenplay->getKernel();
	TextureAtlasIter textureAtlasIterator =  npAtlasMap::getInstance().FrameContainer.find(index);
	UVPacket* uvPacket = &textureAtlasIterator->second;
	m_pUVPacket = uvPacket;
	m_GaugeUVBindID = uvPacket->bindTextureID;
	memcpy(m_GaugeUV, uvPacket->texture, sizeof(GLfloat)*8);

	m_GaugeUV_WidthPercent = m_GaugeUV[4] - m_GaugeUV[0];

	m_fDrawPosX = - m_fGaugeHalfWidth*(1.0f - m_fDrawPercent);
}

void pbHPMeter::DecreaseHP(float Damage) {
	m_fTotalHP -= Damage;

	if( m_fTotalHP <= 0 ) {
		m_bDie = true;
	}

	m_fDecreaseLinePercent += Damage/m_fOneLineHP;
	m_bGaugeSetting = true;
}

void pbHPMeter::AnimationGauge(float fTime) {
	//게이지 세팅
	if( m_bGaugeSetting ) {
		float fDestPrevision = m_fStartPercent - m_fDecreaseLinePercent;
		if( fDestPrevision <= 0.0f ) {
			m_fDestPercent = 0.0f;
		}
		else
			m_fDestPercent = fDestPrevision;

		m_bGaugeChangeStart = true;
		float fWholeOfDistance = m_fStartPercent - m_fDestPercent;
		float fPartOfDistance = m_fStartPercent - m_fDrawPercent;
		m_fAniTime = (fPartOfDistance/fWholeOfDistance);  // distance 0.5 -> 0.8 time == 0.4 percent      distance 1.0  ->    ? time == 0.6 percent
		m_fAniSpeed = (fWholeOfDistance)*2;	//곱해지는 양은 걸리는 시간과 같다

		m_bGaugeSetting = false;
	}

	// 게이지 선형 보간
	if( m_bGaugeChangeStart ) {
		m_fAniTime += (1.0f / m_fAniSpeed) * fTime;

		if (m_fAniTime >= 1.0f) {
			m_fAniTime = 1.0f;

			m_fDecreaseLinePercent -= m_fStartPercent - m_fDestPercent;
			if( m_fDestPercent == 0.0f) {
				if( m_iLineCount == 0) {
					CloseMeter();
				}
				else {
					m_iLineCount--;
					m_bGaugeSetting = true;
					m_fDestPercent = 1.0f;
				}
			}

			m_fStartPercent = m_fDestPercent;// m_fDrawPercent는 m_fAniTime으로 밑의 선형보간에서 초기화 한다
			m_bGaugeChangeStart = false;
		}

		m_fDrawPercent = npLerp(m_fStartPercent, m_fDestPercent, m_fAniTime);
		m_fDrawPosX = -m_fGaugeHalfWidth * (1.0f - m_fDrawPercent);

	}
}

void pbHPMeter::OpenMeter() {
	m_Phase = PHASE_OPEN_HORIZON;
	m_fAniTime = 0.0f;
	m_fHorizonScale = 0.0f;
	m_fVerticalScale = 0.3f;
}

void pbHPMeter::CloseMeter() {
	m_Phase = PHASE_CLOSE_HORIZON;
	m_fAniTime = 0.0f;
	m_fHorizonScale = 1.0f;
	m_fVerticalScale = 1.0f;

	pbSceneManager::getInstance().GetCurrentScene()->GetStageTrigger()->ActivateIDState(pbCharacter::WALKOUT);
	pbSceneManager::getInstance().GetCurrentScene()->GetStageTrigger()->ActivateIDState(pbBoss::DIE);
}

void pbHPMeter::BindLineColor(int Line) {
	if( Line == 0)
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	else if( Line == 1)
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	else if( Line == 2)
		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	else if( Line == 3)
		glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	else if( Line == 4)
		glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
	else if( Line == 5)
		glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
	else if( Line == 6)
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void pbHPMeter::Draw() {
	if( m_Phase != PHASE_NONE) {
	glPushMatrix();
		glTranslatef(m_vPos[0], m_vPos[1],  0);
		glScalef(m_fHorizonScale, m_fVerticalScale, 1.0f);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		///------------Power Text--------------------
		glPushMatrix();
		m_pBaseDrawUnit->PreSettingDraw();
		m_pBaseDrawUnit->DrawThis();
		glPopMatrix();
		///------------게이지 텍스쳐 바인드--------------------
		glBindTexture(GL_TEXTURE_2D,  m_GaugeUVBindID );
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glVertexPointer(3, GL_FLOAT, 0, m_GaugeVertex);
		///-------------중복 라인 그리기
		glTexCoordPointer(2,GL_FLOAT, 0,  m_pUVPacket->texture);	//�ؽ�ó��ǥ(UV) �迭 �Ѱ��ֱ�

		for(int i = 0; i < m_iLineCount ; i++) {
			BindLineColor(i);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
		BindLineColor(m_iLineCount);
		///------------게이지 텍스쳐 그리기--------------------
		glTexCoordPointer(2,GL_FLOAT, 0,  m_GaugeUV);	//�ؽ�ó��ǥ(UV) �迭 �Ѱ��ֱ�
		glPushMatrix();
		m_GaugeUV[4] = m_GaugeUV[0] + m_GaugeUV_WidthPercent*m_fDrawPercent;
		m_GaugeUV[6] = m_GaugeUV[2] + m_GaugeUV_WidthPercent*m_fDrawPercent;
		glTranslatef( m_fDrawPosX, 0 ,  0);
		glScalef(m_fDrawPercent, 1.0f, 1.0f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glPopMatrix();

	glPopMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

void pbHPMeter::Update(float fTime) {
	if( m_Phase == PHASE_NONE) {

	}
	else if( m_Phase == PHASE_OPEN_HORIZON) {
		m_fAniTime += fTime;
		m_fHorizonScale = m_fAniTime;
		if(m_fAniTime > 1.0f) {
			m_fAniTime = 0.3f;
			m_Phase = PHASE_OPEN_VERTICAL;
			m_fHorizonScale = 1.0f;
		}
	}
	else if( m_Phase == PHASE_OPEN_VERTICAL) {
		m_fAniTime += 2*fTime;
		m_fVerticalScale = m_fAniTime;
		if(m_fAniTime > 1.0f) {
			m_fAniTime = 0.0f;
			m_Phase = PHASE_GAUGING;
			m_fVerticalScale = 1.0f;

			pbBoss::GetInstance()->SetBattlePhase(true	);
		}
	}
	else if( m_Phase == PHASE_GAUGING) {
		AnimationGauge(fTime);
	}
	else if( m_Phase == PHASE_CLOSE_HORIZON) {
		m_fAniTime += fTime;
		m_fHorizonScale = (1.0f - m_fAniTime );
		if(m_fAniTime > 1.0f) {
			m_fAniTime = 0.0f;
			m_Phase = PHASE_NONE;
			m_fHorizonScale = 0.0f;
		}
	}

}

//---------------------------------------------- pbBoss------------------------------------------------------------------------------------//
pbBoss* pbBoss::SingleObject = NULL;

pbBoss::pbBoss() {
	m_bBattlePhase = false;
	m_bBossAlive = true;

	m_pMarionette = NULL;
	m_pBodyDrawUnit = NULL;
	m_pHPMeter = NULL;
}

pbBoss::~pbBoss() {

}

void pbBoss::Create() {
	if( SingleObject == NULL ) {
		SingleObject = new pbBoss();

		SingleObject->m_pMarionette = new pbMarionette();
		SingleObject->m_pBodyDrawUnit = new pbBasicDrawUnit();
		SingleObject->m_pHPMeter = new pbHPMeter();
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
}

void pbBoss::Update(float fTime) {
	int m_MarionetteState = m_pMarionette->GetState();

	if( m_MarionetteState == APPROACHING) {
		if( m_pMarionette->GetActionCondition() ) {
			m_pMarionette->SelectMoveState(WEAVING_UP);
			/*m_bBattlePhase = true;*/
			GetHPMeter()->OpenMeter();
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

			LOGE("CHANGE TO DIE");
		}
	}

	m_pMarionette->MoveUpdate(fTime);

	m_pHPMeter->Update(fTime);
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
}

void pbBoss::DrawThis() {
		m_pBodyDrawUnit->PreSettingDraw();
		glColor4f(1.0f , 0.0f, 0.0f, 1.0f);	//임시
		m_pBodyDrawUnit->DrawThis();
		glColor4f(1.0f , 1.0f, 1.0f, 1.0f);
	glPopMatrix();

	m_pHPMeter->Draw();
}

void pbBoss::Approaching() {
	GetMarionette()->SelectMoveState(APPROACHING);
	GetMarionette()->SetMovePause(false);

	LOGE("pbBoss::Approaching() CHANGE TO APPROACHING");
}

void pbBoss::PostDieProcess() {
	GetMarionette()->SetMovePause(true);
	pbSceneManager::getInstance().RemoveRenderToCurrentScene(GetInstance());
}

void pbBoss::DecreaseHP(float fDamage){
	if( GetInstance() != NULL) {
		GetHPMeter()->DecreaseHP(fDamage);

		if( GetHPMeter()->IsDied()) {
			GetMarionette()->SelectMoveState(WALKOUT);
//			LOGE("CHANGE TO WALKOUT");
			GetInstance()->SetBattlePhase(false);
		}

//		LOGE("DAMAGE :");
		LOGfloatString("CurrentHP :", GetHPMeter()->GetTotalHP());
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

		delete SingleObject->m_pHPMeter;

		delete SingleObject;
		SingleObject = NULL;
	}
}
