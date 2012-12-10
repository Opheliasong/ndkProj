#include "pbEffect.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ ���� ����Ʈ------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*pbJudgementEffect::pbJudgementEffect()
{

}

pbJudgementEffect::~pbJudgementEffect()
{

}

void pbJudgementEffect::Initialize(GLuint VertexIndex, GLuint UVIndex, float LifeTime, bool Infinite)
{
	m_BodyVertexIndex = VertexIndex;
	m_BodyUVIndex = UVIndex;

	m_fLifeTime = LifeTime;
	m_bInfinite = Infinite;
	m_bAlive = true;
}


void pbJudgementEffect::Draw()
{
	///--------------------------------------
	pbDataStorage::BindVertexAndTexture(m_BodyVertexIndex, m_BodyUVIndex);

	//�ؽ�ó ���ε�
	glPushMatrix();
	glTranslatef(m_vPos[0], m_vPos[1],  0);
	glScalef(m_fScale, m_fScale, 1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glPopMatrix();

}

void pbJudgementEffect::Update(float fTime)
{
	if( m_bAlive)
	{
		if( !m_bInfinite)
			m_fLifeTime -= fTime;

		if(m_fLifeTime < 0.0f)
		{
			m_bAlive = false;
			pbEffectProcess::GetInstance()->removeControled(this);
			pbRenderProcess::RemoveRenderEffect(this);
			pbEffectProcess::GetInstance()->m_JudgeEffectRentalUnit->ReturningRentalMemory(this);
		}
		m_vPos[1] += 50*fTime;
	}
}*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ ���ܾ� ����Ʈ------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pbStepUpEffect::pbStepUpEffect(){
	m_fMaxLifeTime = 0.0f;
	m_iPanelPhase = 0;
	m_fPanelScale = 0.0f;
	m_fPanelLerp = 0.0f;
	m_fPanelPosX = 0.0f;

	m_iStepUpPhase = 0;
	m_fStepUpLerp = 0.0f;
	m_fStepUpScale = 0.0f;
	m_fStepUpPosX = 0.0f;
	m_fStepUpAniTime = 0.0f;

	m_bLabelDrawing = true;
	m_fLabelTextWidth = 0.0f;
	m_fLabelTextPosX = 0.0f;
	m_iLabelPhase = 0;
	m_fLabelLerp = 0.0f;

	m_pPanelDrawUnit = new pbBasicDrawUnit();
	m_pLabelDrawUnit = new pbBasicDrawUnit();
	m_pLabelTextDrawUnit = new pbBasicDrawUnit();
	m_pStepUpDrawUnit = new pbBasicDrawUnit();


}

pbStepUpEffect::~pbStepUpEffect(){
	delete m_pPanelDrawUnit;
	delete m_pLabelDrawUnit;
	delete m_pLabelTextDrawUnit;
	delete m_pStepUpDrawUnit;
}

void pbStepUpEffect::Initialize(TAGDATA& PanelTagData, TAGDATA& LabelTextTagData, TAGDATA& LabelTagData, TAGDATA& StepUpTagData)
{
	DataReset();

	m_bInfinite = false;

	m_pPanelDrawUnit->SetTextureTAG(PanelTagData.Tag);
	m_pPanelDrawUnit->SetSize(PanelTagData.fWidth, PanelTagData.fHeight);

	m_pLabelTextDrawUnit->SetTextureTAG(LabelTextTagData.Tag);
	m_pLabelTextDrawUnit->SetSize(LabelTextTagData.fWidth, LabelTextTagData.fHeight);
	m_fLabelTextWidth = LabelTextTagData.fWidth;

	m_pLabelDrawUnit->SetTextureTAG(LabelTagData.Tag);
	m_pLabelDrawUnit->SetSize(LabelTagData.fWidth, LabelTagData.fHeight);

	m_pStepUpDrawUnit->SetTextureTAG(StepUpTagData.Tag);
	m_pStepUpDrawUnit->SetSize(StepUpTagData.fWidth, StepUpTagData.fHeight);


}

void pbStepUpEffect::DataReset()
{
	m_fLifeTime = 0;

//	m_bPanelDrawing = false;
	m_iPanelPhase = 0;
	m_fPanelScale = 0.2f;
	m_fPanelPosX = -600.0f;

//	m_bStepUpDrawing = false;
	m_fStepUpScale = 1.0f;
	m_fStepUpPosX = -600.0f;
	m_iStepUpPhase = 0;

	m_bLabelDrawing = false;
	m_iLabelPhase = 0;
	m_fLabelLerp = 0.0f;
	m_fLabelTextWidth = 0;
	m_fLabelTextPosX = 0.0f;

	m_bAlive = true;
}

void pbStepUpEffect::SetMaxLifeTime(float fLifeTime) {
	m_fMaxLifeTime = fLifeTime;
	m_fLifeTime = 0.0f;
}

void pbStepUpEffect::PreSettingDraw() {
	glPushMatrix();
	glTranslatef(m_vPos[0], m_vPos[1],  0);
}

void pbStepUpEffect::DrawThis() {
	///---------------Label Draw-------------------//
		if( m_bLabelDrawing ) {
			glPushMatrix();
			glTranslatef(0, -20,  0);
			m_pLabelDrawUnit->PreSettingDraw();
			m_pLabelDrawUnit->DrawThis();
			glPopMatrix();
			///---------------TextDraw-------------------//
			glPushMatrix();
				m_pLabelTextDrawUnit->PreSettingDraw();
				glTranslatef(m_fLabelTextPosX - 500, 0,  0);
				for(int i = 0; i < 10; i++) {
					glPushMatrix();
					glTranslatef( m_fLabelTextWidth*i, 0,  0);
					m_pLabelTextDrawUnit->DrawThis();
					glPopMatrix();
				}

			glPopMatrix();
		}
		///---------------BackPanel-------------------//
	//	if( m_bPanelDrawing ) {
			glPushMatrix();
			glTranslatef(m_fPanelPosX, 40,  0);
			glScalef(1.0f, m_fPanelScale, 1.0f);
			m_pPanelDrawUnit->PreSettingDraw();
			m_pPanelDrawUnit->DrawThis();
			glPopMatrix();
	//	}
		///---------------StepUp Text------------------//
			glPushMatrix();
			glTranslatef(m_fStepUpPosX, 40,  0);
			glScalef(m_fStepUpScale, m_fStepUpScale, 1.0f);
			m_pStepUpDrawUnit->PreSettingDraw();
			m_pStepUpDrawUnit->DrawThis();
			glPopMatrix();


	glPopMatrix();
}

void pbStepUpEffect::Update(float fTime)
{
	if( m_bAlive)
	{
		//---------------------------�г� ������ ����---------------------------------------//
		if( m_iPanelPhase == 0) {
			m_fPanelLerp += (1/0.15f)*fTime;
			//-------------���� �̵�-----------------//
			m_fPanelPosX = npLerp(-600, 0.0f, m_fPanelLerp*m_fPanelLerp);
			if( m_fPanelPosX > 0.0f)
				m_fPanelPosX = 0.0f;

			if( m_fPanelLerp > 1.0f) {
				m_fPanelLerp = 0.0f;
				m_iPanelPhase++;
			}
		}
		else if( m_iPanelPhase == 1) {
			m_fPanelLerp += (1/0.15f)*fTime;
			//-------------������ Ű���-----------------//
			m_fPanelScale = npLerp(0.2f, 1.0f, m_fPanelLerp);

			if( m_fPanelLerp > 1.0f) {
				m_fPanelLerp = 0.0f;
				m_fPanelScale = 1.0f;
				m_iPanelPhase++;
			}
		}
		else if( m_iPanelPhase == 2) {
			m_fPanelLerp += (1/0.55f)*fTime;
			//-------------��� -----------------//
			if( m_fPanelLerp > 1.0f) {
				m_fPanelLerp = 0.0f;
				m_iPanelPhase++;
			}

		}
		else if( m_iPanelPhase == 3) {
			m_fPanelLerp += (1/0.15f)*fTime;

			//-------------���� ����-----------------//
			m_fPanelScale = npLerp(1.0f, 0.2f, m_fPanelLerp);
			m_fPanelPosX = npLerp(0, 600.0f, m_fPanelLerp);

			if( m_fPanelLerp > 1.0f) {
				m_fPanelLerp = 0.0f;
				m_iPanelPhase++;
			}

		}
		//================================================//
		//---------------------------���ܾ� ������ ����---------------------------------------//
		if( m_iStepUpPhase == 0) {
			m_fStepUpLerp += (1/0.15f)*fTime;

			//--------------------���� �̵�-------------------//
			m_fStepUpPosX = npLerp(-600, 0.0f, m_fStepUpLerp*m_fStepUpLerp);
			if( m_fStepUpPosX > 0.0f)
				m_fStepUpPosX = 0.0f;

			if( m_fStepUpLerp > 1.0f) {
				m_fStepUpLerp = 0.0f;
				m_iStepUpPhase++;
			}
		}
		else if( m_iStepUpPhase == 1) {
			//-------������------------//
			m_fStepUpScale = 2.0f;
			m_iStepUpPhase++;
		}
		else if( m_iStepUpPhase == 2) {
			//--------�ִϸ��̼�------------//
			m_fStepUpAniTime += fTime;

/*			if( m_fStepUpAniTime > 0.10f) {
				m_StepUpAniCount++;
				m_fStepUpAniTime = 0.0f;
				if( m_StepUpAniCount > 5  )
					m_StepUpAniCount = 5;
			}*/
			//=====================//

			m_fStepUpLerp += (1/0.6f)*fTime;
			//-------�̵�-----------//
			m_fStepUpPosX += 200*fTime;

			if( m_fStepUpLerp > 1.0f) {
				m_fStepUpLerp = 0.0f;
				m_iStepUpPhase++;
			}
		}
		else if( m_iStepUpPhase == 3) {
			m_fStepUpLerp += (1/0.15f)*fTime;

			//-------------���� �̵�-----------------//
			m_fStepUpPosX = npLerp(m_fStepUpPosX, 800, m_fStepUpLerp);

			if( m_fStepUpLerp > 1.0f) {
				m_fStepUpLerp = 0.0f;
				m_iStepUpPhase++;
			}

		}
		//================================================//
		//---------------------------�� ������ ����---------------------------------------//
		if (m_iLabelPhase == 0) {
			m_fLabelLerp += (1 / 0.25f) * fTime;

			if (m_fLabelLerp > 1.0f) {
				m_fLabelLerp = 0.0f;
				m_iLabelPhase++;
				m_bLabelDrawing = true;
			}

		} else if (m_iLabelPhase == 1) {
			m_fLabelLerp += (1 / 0.6f) * fTime;

			//-------------�� ����-----------------//
			m_fLabelTextPosX -= fTime*m_fLabelTextWidth*2.0f;

			if (m_fLabelLerp > 1.0f) {
				m_fLabelLerp = 0.0f;
				m_iLabelPhase++;
				m_bLabelDrawing = false;
			}

		}
		//================================================//
		if( m_fLifeTime >= m_fMaxLifeTime )
		{
			m_bAlive = false;
			pbEffectManager::GetInstance()->RemoveEffectAndReturningMemory(this);
			pbSceneManager::getInstance().RemoveRenderToScene(m_RegistedScene, this);
		}

		if( !m_bInfinite)
			m_fLifeTime += fTime;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ ��ƼĿ ����Ʈ------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pbStickerEffect::pbStickerEffect(){
	m_pDrawUnit = new pbBasicDrawUnit();
}

pbStickerEffect::~pbStickerEffect(){
	delete m_pDrawUnit;
}

void pbStickerEffect::Initialize(TAGDATA& TagData, float LifeTime, bool Infinite)
{
	m_pDrawUnit->SetTextureTAG(TagData.Tag);
	m_pDrawUnit->SetSize(TagData.fWidth, TagData.fHeight);

	m_fLifeTime = LifeTime;
	m_bInfinite = Infinite;
	m_bAlive = true;

	m_Color.Init(1.0f, 1.0f, 1.0f, 1.0f);

}
void pbStickerEffect::PreSettingDraw() {
	glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
}
void pbStickerEffect::DrawThis() {
	glPushMatrix();
	glTranslatef(m_vPos[0], m_vPos[1], 0.0f);
	m_pDrawUnit->PreSettingDraw();
	m_pDrawUnit->DrawThis();
	glPopMatrix();

	glColor4f(1.0f, 1.0f,1.0f, 1.0f);
}

void pbStickerEffect::Update(float fTime)
{
	if( m_bAlive)
	{
		if( !m_bInfinite)
			m_fLifeTime -= fTime;

		if(m_fLifeTime < 0.0f)
		{
			m_bAlive = false;
			pbSceneManager::getInstance().RemoveRenderToScene(m_RegistedScene, this);
			pbEffectManager::GetInstance()->RemoveEffectAndReturningMemory(this);
			LOGE("RETURN StickerEffect");
		}
//		m_vPos[1] += 50*fTime;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ �����̻��� ����Ʈ------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pbHomingMissileEffect::pbHomingMissileEffect(){
	m_vOrigin[0] = 0;
	m_vOrigin[1] = 0;
	m_vDestination[0] = 0;
	m_vDestination[1] = 0;

	m_fBezierLerp = 1.0f;

	m_fpDecreaseHP= NULL;
	m_pMissileDrawUnit = new pbBasicDrawUnit();

	m_pPosArray[0] = m_vPos;
	m_pPosArray[1] = m_vPos_1;
	m_pPosArray[2] = m_vPos_2;
	m_pPosArray[3] = m_vPos_3;

	 m_fDamage = 0.0f;
}

pbHomingMissileEffect::~pbHomingMissileEffect(){
	delete m_pMissileDrawUnit;

}

void pbHomingMissileEffect::Initialize(TAGDATA& TagData, float LifeTime, bool Infinite){
	m_pMissileDrawUnit->SetTextureTAG(TagData.Tag);
	m_pMissileDrawUnit->SetSize(TagData.fWidth, TagData.fHeight);

	m_fLifeTime = LifeTime;
	m_bInfinite = Infinite;
	m_bAlive = true;
	m_fBezierLerp = 0;

}

void pbHomingMissileEffect::SetMissileCurve(float* pV2Origin, float* pV2Destination) {
	m_Bezier_Start.x = pV2Origin[0];
	m_Bezier_Start.y = pV2Origin[1];

	//--------------------0�� ���� ------------------------------//
	srand(time(NULL));
	int randX = rand() % 400 - 400;
	int randY = rand() % 400 - 200;
	m_Bezier_0_1.x = pV2Origin[0] + randX;
	m_Bezier_0_1.y = pV2Origin[1] + randY;

/*	randY = rand() % 400 - 200;
	m_Bezier_0_2.x =  pV2Origin[0] + 200;
	m_Bezier_0_2.y = pV2Destination[1] + randY;*/

	//--------------------1�� ���� ------------------------------//
	randX = rand() % 400 - 400;
	randY = rand() % 400 - 200;
	m_Bezier_1_1.x = pV2Origin[0] + randX;
	m_Bezier_1_1.y = pV2Origin[1] + randY;

/*	randY = rand() % 400 - 200;
	m_Bezier_1_2.x =  pV2Origin[0] + 200;
	m_Bezier_1_2.y = pV2Destination[1] + randY;*/

	//--------------------2�� ���� ------------------------------//
	randX = rand() % 400 - 400;
	randY = rand() % 400 - 200;
	m_Bezier_2_1.x = pV2Origin[0] + randX;
	m_Bezier_2_1.y = pV2Origin[1] + randY;

/*	randY = rand() % 400 - 200;
	m_Bezier_2_2.x =  pV2Origin[0] + 200;
	m_Bezier_2_2.y = pV2Destination[1] + randY;*/

	//--------------------3�� ���� ------------------------------//
	randX = rand() % 400 - 400;
	randY = rand() % 400 - 200;
	m_Bezier_3_1.x = pV2Origin[0] + randX;
	m_Bezier_3_1.y = pV2Origin[1] + randY;

/*	randY = rand() % 400 - 200;
	m_Bezier_3_2.x =  pV2Origin[0] + 200;
	m_Bezier_3_2.y = pV2Destination[1] + randY;*/

	//--------------------4�� ���� ------------------------------//
/*	randX = rand() % 400 - 400;
	randY = rand() % 400 - 200;
	m_Bezier_4_1.x = pV2Origin[0] + randX;
	m_Bezier_4_1.y = pV2Origin[1] + randY;*/

/*	randY = rand() % 400 - 200;
	m_Bezier_4_2.x =  pV2Origin[0] + 200;
	m_Bezier_4_2.y = pV2Destination[1] + randY;*/

	m_Bezier_End.x = pV2Destination[0];
	m_Bezier_End.y = pV2Destination[1];
}

void pbHomingMissileEffect::PreSettingDraw() {
	m_pMissileDrawUnit->PreSettingDraw();
}

void pbHomingMissileEffect::DrawThis() {
	for(int i = 0; i < MAX_MISSILE; i++) {
		glPushMatrix();
		glTranslatef(m_pPosArray[i][0], m_pPosArray[i][1],  0);
		m_pMissileDrawUnit->DrawThis();
		glPopMatrix();
	}

}

void pbHomingMissileEffect::Update(float fTime){
	if( m_bAlive)
	{
//		if( !m_bInfinite)
//			m_fLifeTime -= fTime;

		m_fBezierLerp += (1/m_fLifeTime)*fTime;

		if(m_fBezierLerp > 1.0f)
		{
			m_bAlive = false;
			m_fBezierLerp = 1.0f;
			pbSceneManager::getInstance().RemoveRenderToScene(m_RegistedScene, this);
			pbEffectManager::GetInstance()->RemoveEffectAndReturningMemory(this);

			if( m_fpDecreaseHP != NULL)
				(*m_fpDecreaseHP)(m_fDamage);
			else
				LOGE("pbHomingMissileEffect:Update m_fpDecreaseHP is NULL");

			LOGE("RETURN pbHomingMissileEffect");
			return ;
		}

		BEZIER_2POINT CurrentBezierPoint = GetPointFromBezier3(&m_Bezier_Start, &m_Bezier_0_1, &m_Bezier_End, m_fBezierLerp);
		m_vPos[0] = CurrentBezierPoint.x;
		m_vPos[1] = CurrentBezierPoint.y;

		CurrentBezierPoint = GetPointFromBezier3(&m_Bezier_Start, &m_Bezier_1_1, &m_Bezier_End, m_fBezierLerp);
		m_vPos_1[0] = CurrentBezierPoint.x;
		m_vPos_1[1] = CurrentBezierPoint.y;

		CurrentBezierPoint = GetPointFromBezier3(&m_Bezier_Start, &m_Bezier_2_1, &m_Bezier_End, m_fBezierLerp);
		m_vPos_2[0] = CurrentBezierPoint.x;
		m_vPos_2[1] = CurrentBezierPoint.y;

		CurrentBezierPoint = GetPointFromBezier3(&m_Bezier_Start, &m_Bezier_3_1,  &m_Bezier_End, m_fBezierLerp);
		m_vPos_3[0] = CurrentBezierPoint.x;
		m_vPos_3[1] = CurrentBezierPoint.y;

/*		CurrentBezierPoint = GetPointFromBezier3(&m_Bezier_Start, &m_Bezier_4_1, &m_Bezier_End, m_fBezierLerp);
		m_vPos_4[0] = CurrentBezierPoint.x;
		m_vPos_4[1] = CurrentBezierPoint.y;*/

/*		BEZIER_2POINT CurrentBezierPoint = GetPointFromBezier4(&m_Bezier_Start, &m_Bezier_0_1, &m_Bezier_0_2, &m_Bezier_End, m_fBezierLerp);
		m_vPos[0] = CurrentBezierPoint.x;
		m_vPos[1] = CurrentBezierPoint.y;

		CurrentBezierPoint = GetPointFromBezier4(&m_Bezier_Start, &m_Bezier_1_1, &m_Bezier_1_2, &m_Bezier_End, m_fBezierLerp);
		m_vPos_1[0] = CurrentBezierPoint.x;
		m_vPos_1[1] = CurrentBezierPoint.y;

		CurrentBezierPoint = GetPointFromBezier4(&m_Bezier_Start, &m_Bezier_2_1, &m_Bezier_2_2, &m_Bezier_End, m_fBezierLerp);
		m_vPos_2[0] = CurrentBezierPoint.x;
		m_vPos_2[1] = CurrentBezierPoint.y;

		CurrentBezierPoint = GetPointFromBezier4(&m_Bezier_Start, &m_Bezier_3_1, &m_Bezier_3_2, &m_Bezier_End, m_fBezierLerp);
		m_vPos_3[0] = CurrentBezierPoint.x;
		m_vPos_3[1] = CurrentBezierPoint.y;

		CurrentBezierPoint = GetPointFromBezier4(&m_Bezier_Start, &m_Bezier_4_1, &m_Bezier_4_2, &m_Bezier_End, m_fBezierLerp);
		m_vPos_4[0] = CurrentBezierPoint.x;
		m_vPos_4[1] = CurrentBezierPoint.y;*/
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------����Ʈ �Ŵ���------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pbEffectManager* pbEffectManager::SingleObject = NULL;

pbEffectManager::pbEffectManager(){
	m_pEffectLinkHeader = NULL;

	m_pStickerEffectRentalUnit = NULL;
	m_pStepUpEffectRentalUnit = NULL;
	m_pHomingMissileEffectRentalUnit = NULL;

}
pbEffectManager::~pbEffectManager(){
	if(SingleObject != NULL) {
		ClearDataStore();
	}
}


void pbEffectManager::Create()
{
	if(SingleObject == NULL)
	{
		SingleObject = new pbEffectManager();

		SingleObject->m_pEffectLinkHeader = new EffectList;
		SingleObject->m_pEffectLinkHeader->setHeader();

		SingleObject->m_pStickerEffectRentalUnit = new pbMemoryRentalUnit<pbStickerEffect>;
		SingleObject->m_pStickerEffectRentalUnit->Initialize(5);
		SingleObject->m_pStepUpEffectRentalUnit =new pbMemoryRentalUnit<pbStepUpEffect>;
		SingleObject->m_pStepUpEffectRentalUnit->Initialize(2);

		SingleObject->m_pHomingMissileEffectRentalUnit =new pbMemoryRentalUnit<pbHomingMissileEffect>;
		SingleObject->m_pHomingMissileEffectRentalUnit->Initialize(4);

		LOGI("pbEffectProcess Create");

		return;
	}

	LOGE("pbEffectProcess Create Failed");
}

void pbEffectManager::AddStepUpEffect(float X, float Y, TAGDATA& PanelTagData, TAGDATA& LabelTextTagData, TAGDATA& LabelTagData, TAGDATA& StepUpTagData)
{
	pbStepUpEffect* pEffect = m_pStepUpEffectRentalUnit->RentalMemory();
	if( pEffect == NULL) return ;
	pEffect->SetRegistedSceneTag(m_SceneTag);
	pEffect->Initialize(PanelTagData, LabelTextTagData, LabelTagData, StepUpTagData);
	pEffect->SetPos(X, Y);
	//pEffect->SetScale(fScale);

	pEffect->SetMaxLifeTime(1.0f);

	registControled(pEffect);
	pbSceneManager::getInstance().RegistRenderToScene(m_SceneTag, pEffect);
}

void pbEffectManager::AddStickerEffect(float X, float Y, TAGDATA& TagData, float lifeTime) {
	pbStickerEffect* pEffect = m_pStickerEffectRentalUnit->RentalMemory();
	if( pEffect == NULL) return;
	pEffect->SetRegistedSceneTag(m_SceneTag);
	pEffect->Initialize(TagData, lifeTime, false);
	pEffect->SetPos(X, Y);

	registControled(pEffect);
	pbSceneManager::getInstance().RegistRenderToScene(m_SceneTag, pEffect);

	LOGE("ADD StickerEffect");
}

void pbEffectManager::AddHomingMissileEffect(float fStartX, float fStartY, float fDestX, float fDestY, TAGDATA& TagData,  float LifeTime, float fDamage, void (*DecreaseHP)(float fDamage) ) {
	pbHomingMissileEffect* pEffect = m_pHomingMissileEffectRentalUnit->RentalMemory();
	if( pEffect == NULL) return;
	pEffect->SetRegistedSceneTag(m_SceneTag);
	pEffect->Initialize(TagData, LifeTime, false);

	npV2Vector vOrigin, vDestination;
	vOrigin[0] = fStartX;
	vOrigin[1] = fStartY;
	vDestination[0] = fDestX;
	vDestination[1] = fDestY;

	pEffect->SetMissileCurve(vOrigin, vDestination);
	pEffect->SetDecreaseHPFunc(DecreaseHP);
	pEffect->SetDamage(fDamage);

//		pEffect->SetPos(X, Y);

	registControled(pEffect);
	pbSceneManager::getInstance().RegistRenderToScene(m_SceneTag, pEffect);

	LOGE("ADD HomingMissileEffect");
}

void pbEffectManager::AddMissEffect() {
	pbStickerEffect* pEffect = m_pStickerEffectRentalUnit->RentalMemory();
	if( pEffect == NULL) return;
	pEffect->SetRegistedSceneTag(m_SceneTag);
	TAGDATA TagData;
	TagData.SetData("ci", 200, 200);
	pEffect->Initialize(TagData, 0.1f, false);
	pEffect->SetPos(400, 240);

	pEffect->SetColor(1.0f,1.0f,1.0f,1.0f);

	registControled(pEffect);
	pbSceneManager::getInstance().RegistRenderToScene(m_SceneTag, pEffect);
}

void pbEffectManager::Update(float fTime)
{
	EffectList* iterator;
	EffectList* head = m_pEffectLinkHeader;

	iterator = head->getNext();
	while( iterator != head ) {
		pbEffect* pEffect = iterator->getKernel();
		iterator = iterator->getNext();

		pEffect->Update(fTime);
	}

}

void pbEffectManager::ClearDataStore() {
	LinkNodeDeleteAllKernel(pbEffect*, m_pEffectLinkHeader)
	EffectList::clearList(m_pEffectLinkHeader);

	LOGI("pbEffectProcess ClearDataStore");
}

void pbEffectManager::Release()
{
	if( SingleObject != NULL) {
		SingleObject->ClearDataStore();

		if( SingleObject->m_pStickerEffectRentalUnit != NULL) {
			SingleObject->m_pStickerEffectRentalUnit->Release();
			delete SingleObject->m_pStickerEffectRentalUnit;
			SingleObject->m_pStickerEffectRentalUnit = NULL;
		}

		if( SingleObject->m_pStepUpEffectRentalUnit != NULL) {
			SingleObject->m_pStepUpEffectRentalUnit->Release();
			delete SingleObject->m_pStepUpEffectRentalUnit;
			SingleObject->m_pStepUpEffectRentalUnit = NULL;
		}

		if( SingleObject->m_pHomingMissileEffectRentalUnit != NULL) {
			SingleObject->m_pHomingMissileEffectRentalUnit->Release();
			delete SingleObject->m_pHomingMissileEffectRentalUnit;
			SingleObject->m_pHomingMissileEffectRentalUnit = NULL;
		}


		if( SingleObject->m_pEffectLinkHeader != NULL) {
			delete SingleObject->m_pEffectLinkHeader;
			SingleObject->m_pEffectLinkHeader =NULL;
		}

		delete SingleObject;
		SingleObject = NULL;
	}
}

void pbEffectManager::registControled(pbEffect* pEffect){
	EffectList::addTail(pEffect,m_pEffectLinkHeader);
}

void pbEffectManager::removeControled(pbEffect* pEffect){
	EffectList::findDelete(pEffect,m_pEffectLinkHeader);
}


// ����Ʈ���� �����ϰ� ��Ż�޸𸮸� ��ȯ�Ѵ�
void pbEffectManager::RemoveEffectAndReturningMemory(pbStickerEffect* pEffect) {
	if( pEffect != NULL) {
		removeControled(pEffect);
		m_pStickerEffectRentalUnit->ReturningRentalMemory(pEffect);

		LOGI("pbEffectProcess::ReturningMemory: pbStickerEffect");
	}
}

void pbEffectManager::RemoveEffectAndReturningMemory(pbStepUpEffect* pEffect) {
	if( pEffect != NULL) {
		removeControled(pEffect);
		m_pStepUpEffectRentalUnit->ReturningRentalMemory(pEffect);
		LOGI("pbEffectProcess::ReturningMemory: pbStepUpEffect");
	}
}

void pbEffectManager::RemoveEffectAndReturningMemory(pbHomingMissileEffect* pEffect) {
	if( pEffect != NULL) {
		removeControled(pEffect);
		m_pHomingMissileEffectRentalUnit->ReturningRentalMemory(pEffect);
		LOGI("pbEffectProcess::ReturningMemory: pbHomingMissileEffect");
	}
}
