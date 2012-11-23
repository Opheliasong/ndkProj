#include "pbEffect.h"
#include "pbDataStorage.h"
#include "pbRenderProcess.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ 판정 이펙트------------------------------------------------------------------------------///////
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

	//텍스처 바인딩
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
////------------------------------------------------------ 스텝업 이펙트------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pbStepUpEffect::pbStepUpEffect()
{

}

pbStepUpEffect::~pbStepUpEffect()
{

}

void pbStepUpEffect::Initialize(GLuint VertexIndex, GLuint UVIndex, float LifeTime, bool Infinite)
{
	m_BodyVertexIndex = 24;
	m_BodyUVIndex = 74;

	m_StepUpVertexIndex = 22;
	m_StepUpUVIndex = 67;

	m_LabelVertexIndex = 25;
	m_LabelUVIndex = 75;

	m_LabelTextVertexIndex =23;
	m_LabelTextUVIndex = 73;

	m_fLifeTime = LifeTime;
	m_bInfinite = Infinite;
	m_bAlive = true;

	DataReset();

}

void pbStepUpEffect::DataReset()
{
//	m_bPanelDrawing = false;
	m_iPanelPhase = 0;
	m_fPanelScale = 0.2f;
	m_PanelPosX = -600.0f;

//	m_bStepUpDrawing = false;
	m_fStepUpScale = 1.0f;
	m_fStepUpPosX = -600.0f;
	m_iStepUpPhase = 0;
	m_StepUpAniCount = 0;


	m_bLabelDrawing = false;
	m_iLabelPhase = 0;
	m_fLabelLerp = 0.0f;
	m_fLabelWidth = pbDataStorage::GetVertexWidth(m_LabelTextVertexIndex);
	m_fLabelPosX = 0.0f;
}

void pbStepUpEffect::SetMaxLifeTime(float fLifeTime) {
	m_fMaxLifeTime = fLifeTime;
	m_fLifeTime = 0.0f;
}

void pbStepUpEffect::Draw()
{

	glPushMatrix();
	glTranslatef(m_vPos[0], m_vPos[1],  0);
	///---------------라벨패널-------------------//
	if( m_bLabelDrawing ) {
		glPushMatrix();
		glTranslatef(0, -20,  0);

			glPushMatrix();
			pbDataStorage::BindVertexAndTexture(m_LabelVertexIndex, m_LabelUVIndex);

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glPopMatrix();

		///---------------라벨텍스트-------------------//
			glPushMatrix();
			pbDataStorage::BindVertexAndTexture(m_LabelTextVertexIndex, m_LabelTextUVIndex);

			glTranslatef(m_fLabelPosX - 500, 0,  0);
			for(int i = 0; i < 10; i++) {
				glTranslatef( m_fLabelWidth, 0,  0);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			}
			glPopMatrix();

		glPopMatrix();
	}
	///---------------백패널-------------------//
//	if( m_bPanelDrawing ) {
		glPushMatrix();
		pbDataStorage::BindVertexAndTexture(m_BodyVertexIndex, m_BodyUVIndex);

		glTranslatef(m_PanelPosX, 40,  0);
		glScalef(1.0f, m_fPanelScale, 1.0f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glPopMatrix();
//	}
	///---------------텍스트-------------------//
		glPushMatrix();
		pbDataStorage::BindVertexAndTexture(m_StepUpVertexIndex, m_StepUpUVIndex + m_StepUpAniCount);

		glTranslatef(m_fStepUpPosX, 40,  0);
		glScalef(m_fStepUpScale, m_fStepUpScale, 1.0f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glPopMatrix();

	glPopMatrix();

}

void pbStepUpEffect::Update(float fTime)
{
	if( m_bAlive)
	{
		//---------------------------패널 페이즈 조절---------------------------------------//
		if( m_iPanelPhase == 0) {
			m_fPanelLerp += (1/0.15f)*fTime;
			//-------------등장 이동-----------------//
			m_PanelPosX = Lerp(-600, 0.0f, m_fPanelLerp*m_fPanelLerp);
			if( m_PanelPosX > 0.0f)
				m_PanelPosX = 0.0f;

			if( m_fPanelLerp > 1.0f) {
				m_fPanelLerp = 0.0f;
				m_iPanelPhase++;
			}
		}
		else if( m_iPanelPhase == 1) {
			m_fPanelLerp += (1/0.15f)*fTime;
			//-------------스케일 키우기-----------------//
			m_fPanelScale = Lerp(0.2f, 1.0f, m_fPanelLerp);

			if( m_fPanelLerp > 1.0f) {
				m_fPanelLerp = 0.0f;
				m_fPanelScale = 1.0f;
				m_iPanelPhase++;
			}
		}
		else if( m_iPanelPhase == 2) {
			m_fPanelLerp += (1/0.55f)*fTime;
			//-------------대기 -----------------//
			if( m_fPanelLerp > 1.0f) {
				m_fPanelLerp = 0.0f;
				m_iPanelPhase++;
			}

		}
		else if( m_iPanelPhase == 3) {
			m_fPanelLerp += (1/0.15f)*fTime;

			//-------------퇴장 연출-----------------//
			m_fPanelScale = Lerp(1.0f, 0.2f, m_fPanelLerp);
			m_PanelPosX = Lerp(0, 600.0f, m_fPanelLerp);

			if( m_fPanelLerp > 1.0f) {
				m_fPanelLerp = 0.0f;
				m_iPanelPhase++;
			}

		}
		//================================================//
		//---------------------------스텝업 페이즈 조절---------------------------------------//
		if( m_iStepUpPhase == 0) {
			m_fStepUpLerp += (1/0.15f)*fTime;

			//--------------------등장 이동-------------------//
			m_fStepUpPosX = Lerp(-600, 0.0f, m_fStepUpLerp*m_fStepUpLerp);
			if( m_fStepUpPosX > 0.0f)
				m_fStepUpPosX = 0.0f;

			if( m_fStepUpLerp > 1.0f) {
				m_fStepUpLerp = 0.0f;
				m_iStepUpPhase++;
			}
		}
		else if( m_iStepUpPhase == 1) {
			//-------스케일------------//
			m_fStepUpScale = 2.0f;
			m_iStepUpPhase++;
		}
		else if( m_iStepUpPhase == 2) {
			//--------애니메이션------------//
			m_fStepUpAniTime += fTime;

			if( m_fStepUpAniTime > 0.10f) {
				m_StepUpAniCount++;
				m_fStepUpAniTime = 0.0f;
				if( m_StepUpAniCount > 5  )
					m_StepUpAniCount = 5;
			}
			//=====================//

			m_fStepUpLerp += (1/0.6f)*fTime;
			//-------이동-----------//
			m_fStepUpPosX += 200*fTime;

			if( m_fStepUpLerp > 1.0f) {
				m_fStepUpLerp = 0.0f;
				m_iStepUpPhase++;
			}
		}
		else if( m_iStepUpPhase == 3) {
			m_fStepUpLerp += (1/0.15f)*fTime;

			//-------------퇴장 이동-----------------//
			m_fStepUpPosX = Lerp(m_fStepUpPosX, 800, m_fStepUpLerp);

			if( m_fStepUpLerp > 1.0f) {
				m_fStepUpLerp = 0.0f;
				m_iStepUpPhase++;
			}

		}
		//================================================//
		//---------------------------라벨 페이즈 조절---------------------------------------//
		if (m_iLabelPhase == 0) {
			m_fLabelLerp += (1 / 0.25f) * fTime;

			if (m_fLabelLerp > 1.0f) {
				m_fLabelLerp = 0.0f;
				m_iLabelPhase++;
				m_bLabelDrawing = true;
			}

		} else if (m_iLabelPhase == 1) {
			m_fLabelLerp += (1 / 0.6f) * fTime;

			//-------------라벨 무빙-----------------//
			m_fLabelPosX -= fTime*m_fLabelWidth*2.0f;

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
			pbEffectProcess::GetInstance()->RemoveEffectAndReturningMemory(this);
			pbRenderProcess::RemoveRenderEffect(this);
		}

		if( !m_bInfinite)
			m_fLifeTime += fTime;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ 피버가능 이펙트------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pbFeverAvailableEffect::pbFeverAvailableEffect()
{

}

pbFeverAvailableEffect::~pbFeverAvailableEffect()
{

}

void pbFeverAvailableEffect::Initialize(GLuint VertexIndex, GLuint UVIndex, float LifeTime, bool Infinite)
{
	m_BodyVertexIndex = 24;
	m_BodyUVIndex = 74;

	m_StepUpVertexIndex = 23;
	m_StepUpUVIndex = 73;

/*	m_LabelVertexIndex = 25;
	m_LabelUVIndex = 75;

	m_LabelTextVertexIndex =23;
	m_LabelTextUVIndex = 73;*/

	m_fLifeTime = LifeTime;
	m_bInfinite = Infinite;
	m_bAlive = true;

	DataReset();

}

void pbFeverAvailableEffect::DataReset()
{
//	m_bPanelDrawing = false;
	m_iPanelPhase = 0;
	m_fPanelScale = 0.2f;
	m_PanelPosX = -600.0f;

//	m_bStepUpDrawing = false;
	m_fStepUpScale = 1.0f;
	m_fStepUpPosX = -600.0f;
	m_iStepUpPhase = 0;
	m_StepUpAniCount = 0;

	m_Color.Init(1.0f, 1.0f, 1.0f, 1.0f);

/*	m_bLabelDrawing = false;
	m_iLabelPhase = 0;
	m_fLabelLerp = 0.0f;
	m_fLabelWidth = pbDataStorage::GetVertexWidth(m_LabelTextVertexIndex);
	m_fLabelPosX = 0.0f;*/
}

void pbFeverAvailableEffect::SetMaxLifeTime(float fLifeTime) {
	m_fMaxLifeTime =fLifeTime;
	m_fLifeTime = 0.0f;
}

void pbFeverAvailableEffect::Draw()
{

	glPushMatrix();
//	glScalef(m_fScale, m_fScale, 1.0f);
	glTranslatef(m_vPos[0], m_vPos[1],  0);
	///---------------라벨패널-------------------//
/*	if( m_bLabelDrawing ) {
		glPushMatrix();
		glTranslatef(0, -20,  0);

			glPushMatrix();
			pbDataStorage::BindVertexAndTexture(m_LabelVertexIndex, m_LabelUVIndex);

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glPopMatrix();

		///---------------라벨텍스트-------------------//
			glPushMatrix();
			pbDataStorage::BindVertexAndTexture(m_LabelTextVertexIndex, m_LabelTextUVIndex);

			glTranslatef(m_fLabelPosX - 500, 0,  0);
			for(int i = 0; i < 10; i++) {
				glTranslatef( m_fLabelWidth, 0,  0);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			}
			glPopMatrix();

		glPopMatrix();
	}*/
	///---------------백패널-------------------//
//	if( m_bPanelDrawing ) {
		glPushMatrix();
		pbDataStorage::BindVertexAndTexture(m_BodyVertexIndex, m_BodyUVIndex);

		glTranslatef(m_PanelPosX, 40,  0);
		glScalef(m_fScale, m_fScale*m_fPanelScale, 1.0f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glPopMatrix();
//	}
	///---------------텍스트-------------------//
		glPushMatrix();
		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		pbDataStorage::BindVertexAndTexture(m_StepUpVertexIndex, m_StepUpUVIndex );

		glTranslatef(m_fStepUpPosX, 40,  0);
		glScalef(m_fScale*m_fStepUpScale, m_fScale*m_fStepUpScale, 1.0f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glPopMatrix();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glPopMatrix();

}

void pbFeverAvailableEffect::Update(float fTime)
{
	if( m_bAlive)
	{
		//---------------------------패널 페이즈 조절---------------------------------------//
		if( m_iPanelPhase == 0) {
			m_fPanelLerp += (1/0.15f)*fTime;
			//-------------등장 이동-----------------//
			m_PanelPosX = Lerp(-600, 0.0f, m_fPanelLerp*m_fPanelLerp);
			if( m_PanelPosX > 0.0f)
				m_PanelPosX = 0.0f;

			if( m_fPanelLerp > 1.0f) {
				m_fPanelLerp = 0.0f;
				m_iPanelPhase++;
			}
		}
		else if( m_iPanelPhase == 1) {
			m_fPanelLerp += (1/0.15f)*fTime;
			//-------------스케일 키우기-----------------//
			m_fPanelScale = Lerp(0.2f, 1.0f, m_fPanelLerp);

			if( m_fPanelLerp > 1.0f) {
				m_fPanelLerp = 0.0f;
				m_fPanelScale = 1.0f;
				m_iPanelPhase++;
			}
		}
		else if( m_iPanelPhase == 2) {
			m_fPanelLerp += (1/0.55f)*fTime;
			//-------------대기 -----------------//
			if( m_fPanelLerp > 1.0f) {
				m_fPanelLerp = 0.0f;
				m_iPanelPhase++;
			}

		}
		else if( m_iPanelPhase == 3) {
			m_fPanelLerp += (1/0.15f)*fTime;

			//-------------퇴장 연출-----------------//
			m_fPanelScale = Lerp(1.0f, 0.2f, m_fPanelLerp);
			m_PanelPosX = Lerp(0, 600.0f, m_fPanelLerp);

			if( m_fPanelLerp > 1.0f) {
				m_fPanelLerp = 0.0f;
				m_iPanelPhase++;
			}

		}
		//================================================//
		//---------------------------스텝업 페이즈 조절---------------------------------------//
		if( m_iStepUpPhase == 0) {
			m_fStepUpLerp += (1/0.15f)*fTime;

			//--------------------등장 이동-------------------//
			m_fStepUpPosX = Lerp(-600, 0.0f, m_fStepUpLerp*m_fStepUpLerp);
			if( m_fStepUpPosX > 0.0f)
				m_fStepUpPosX = 0.0f;

			if( m_fStepUpLerp > 1.0f) {
				m_fStepUpLerp = 0.0f;
				m_iStepUpPhase++;
			}
		}
		else if( m_iStepUpPhase == 1) {
			//-------스케일------------//
			m_fStepUpScale = 2.0f;
			m_iStepUpPhase++;
		}
		else if( m_iStepUpPhase == 2) {
			//--------애니메이션------------//
			m_fStepUpAniTime += fTime;

			if( m_fStepUpAniTime > 0.10f) {
				m_fStepUpAniTime = 0.0f;
				m_StepUpAniCount++;

				//반짝반짝
				if( (m_StepUpAniCount % 2) == 0 ) {
					m_Color.R = 1.0f;
					m_Color.G = 0.2f;
					m_Color.B = m_Color.G;
				}
				else {
					m_Color.R = 1.0f;
					m_Color.G = 1.0f;
					m_Color.B = 0.6f;
				}
			}
			//=====================//

			m_fStepUpLerp += (1/0.6f)*fTime;
			//-------이동-----------//
			m_fStepUpPosX += 200*fTime;

			if( m_fStepUpLerp > 1.0f) {
				m_fStepUpLerp = 0.0f;
				m_iStepUpPhase++;
			}
		}
		else if( m_iStepUpPhase == 3) {
			m_fStepUpLerp += (1/0.15f)*fTime;

			//-------------퇴장 이동-----------------//
			m_fStepUpPosX = Lerp(m_fStepUpPosX, 800, m_fStepUpLerp);

			if( m_fStepUpLerp > 1.0f) {
				m_fStepUpLerp = 0.0f;
				m_iStepUpPhase++;
			}

		}
		//================================================//
		//---------------------------라벨 페이즈 조절---------------------------------------//
/*		if (m_iLabelPhase == 0) {
			m_fLabelLerp += (1 / 0.25f) * fTime;

			if (m_fLabelLerp > 1.0f) {
				m_fLabelLerp = 0.0f;
				m_iLabelPhase++;
				m_bLabelDrawing = true;
			}

		} else if (m_iLabelPhase == 1) {
			m_fLabelLerp += (1 / 0.6f) * fTime;

			//-------------라벨 무빙-----------------//
			m_fLabelPosX -= fTime*m_fLabelWidth*2.0f;

			if (m_fLabelLerp > 1.0f) {
				m_fLabelLerp = 0.0f;
				m_iLabelPhase++;
				m_bLabelDrawing = false;
			}

		}*/
		//================================================//
		if( m_fLifeTime >= m_fMaxLifeTime )
		{
			m_bAlive = false;
			pbEffectProcess::GetInstance()->RemoveEffectAndReturningMemory(this);
			pbRenderProcess::RemoveRenderEffect(this);
		}

		if( !m_bInfinite)
			m_fLifeTime += fTime;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ 콤보 이펙트------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
pbComboEffect::pbComboEffect()
{

}

pbComboEffect::~pbComboEffect()
{

}

void pbComboEffect::Initialize(GLuint VertexIndex, GLuint UVIndex, float LifeTime, bool Infinite)
{
	m_BodyVertexIndex = VertexIndex;
	m_BodyUVIndex = UVIndex;

	m_fLifeTime = LifeTime;
	m_bInfinite = Infinite;
	m_bAlive = true;

	//버텍스 세팅
	m_BodyVertexIndex =VertexIndex;
	float width = pbDataStorage::GetVertexWidth(VertexIndex);
	m_PlacementWidth = width;

	//넘버링 UV 세팅
	for(int i = 0; i < NUMBERING; i++)
	{
		m_NumberUVIndex[i] = UVIndex + i;
	}

	DataReset();

}

void pbComboEffect::DataReset()
{
	for(int i = 0; i < MAX_DIGITS; i++)
		m_DigitsNumber[i] = 0;

	m_CurrentDigits = 1;
}

void pbComboEffect::SetCombo(int Combo) {
	int count = 0;
	int DigitsNumber = Combo;
	while(1)
	{
		m_DigitsNumber[count++] = DigitsNumber%10;
		DigitsNumber /= 10;

		if( DigitsNumber == 0 )
			break;

	}

	m_CurrentDigits = count;
}


void pbComboEffect::Draw()
{
	glPushMatrix();
	glTranslatef(m_vPos[0], m_vPos[1], 0.0f);
		//테이블에서 아이디로 찾는다
		for(int i = 0 ; i < m_CurrentDigits; ++i)		{
			glPushMatrix();
			pbDataStorage::BindVertexAndTexture(m_BodyVertexIndex, m_NumberUVIndex[m_DigitsNumber[i]]);
			//텍스처 바인딩
			glTranslatef(m_PlacementWidth -((float)i)*m_PlacementWidth, 0,  0);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glPopMatrix();
		}
	glPopMatrix();

}

void pbComboEffect::Update(float fTime)
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
			pbEffectProcess::GetInstance()->m_ComboEffectRentalUnit.ReturningRentalMemory(this);
		}
		m_vPos[1] += 50*fTime;
	}
}
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ 스티커 이펙트------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pbStickerEffect::pbStickerEffect()
{

}

pbStickerEffect::~pbStickerEffect()
{

}

void pbStickerEffect::Initialize(GLuint VertexIndex, GLuint UVIndex, float LifeTime, bool Infinite)
{
	m_BodyVertexIndex = VertexIndex;
	m_BodyUVIndex = UVIndex;

	m_fLifeTime = LifeTime;
	m_bInfinite = Infinite;
	m_bAlive = true;

	//버텍스 세팅
	m_fScaleX = pbDataStorage::GetUVWidth(UVIndex);
	m_fScaleY = pbDataStorage::GetUVHeight(UVIndex);

	m_Color.Init(1.0f, 1.0f, 1.0f, 1.0f);

}


void pbStickerEffect::Draw()
{
	///--------------------------------------
	pbDataStorage::BindVertexAndTexture(m_BodyVertexIndex, m_BodyUVIndex);

	glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);

	//텍스처 바인딩
	glPushMatrix();
	glTranslatef(m_vPos[0], m_vPos[1],  0);
	glScalef(m_fScale*m_fScaleX, m_fScale*m_fScaleY, 1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glPopMatrix();

	glColor4f(1.0f, 1.0f,1.0f,1.0f);
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
			pbRenderProcess::RemoveRenderEffect(this);
			pbEffectProcess::GetInstance()->RemoveEffectAndReturningMemory(this);
			LOGE("RETURN StickerEffect");
		}
//		m_vPos[1] += 50*fTime;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////------------------------------------------------------ 유도미사일 이펙트------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pbHomingMissileEffect::pbHomingMissileEffect(){
	m_vOrigin[0] = 0;
	m_vOrigin[1] = 0;
	m_vDestination[0] = 0;
	m_vDestination[1] = 0;


	m_fScaleX = 1.0f;
	m_fScaleY = 1.0f;

	m_fBezierLerp = 1.0f;

	m_fpDecreaseHP= NULL;
}

pbHomingMissileEffect::~pbHomingMissileEffect()
{

}

void pbHomingMissileEffect::Initialize(GLuint VertexIndex, GLuint UVIndex, float LifeTime, bool Infinite)
{
	m_BodyVertexIndex = VertexIndex;
	m_BodyUVIndex = UVIndex;

	m_fLifeTime = LifeTime;
	m_bInfinite = Infinite;
	m_bAlive = true;
	m_fBezierLerp = 0;

	//버텍스 세팅
	m_fScaleX = pbDataStorage::GetUVWidth(UVIndex);
	m_fScaleY = pbDataStorage::GetUVHeight(UVIndex);

	m_Color.Init(1.0f, 1.0f, 1.0f, 1.0f);


}

void pbHomingMissileEffect::SetMissileCurve(float* pV2Origin, float* pV2Destination) {
	m_Bezier_Start.x = pV2Origin[0];
	m_Bezier_Start.y = pV2Origin[1];

	//--------------------0번 사출 ------------------------------//
	srand(time(NULL));
	int randX = rand() % 400 - 400;
	int randY = rand() % 400 - 200;
	m_Bezier_0_1.x = pV2Origin[0] + randX;
	m_Bezier_0_1.y = pV2Origin[1] + randY;

/*	randY = rand() % 400 - 200;
	m_Bezier_0_2.x =  pV2Origin[0] + 200;
	m_Bezier_0_2.y = pV2Destination[1] + randY;*/

	//--------------------1번 사출 ------------------------------//
	randX = rand() % 400 - 400;
	randY = rand() % 400 - 200;
	m_Bezier_1_1.x = pV2Origin[0] + randX;
	m_Bezier_1_1.y = pV2Origin[1] + randY;

/*	randY = rand() % 400 - 200;
	m_Bezier_1_2.x =  pV2Origin[0] + 200;
	m_Bezier_1_2.y = pV2Destination[1] + randY;*/

	//--------------------2번 사출 ------------------------------//
	randX = rand() % 400 - 400;
	randY = rand() % 400 - 200;
	m_Bezier_2_1.x = pV2Origin[0] + randX;
	m_Bezier_2_1.y = pV2Origin[1] + randY;

/*	randY = rand() % 400 - 200;
	m_Bezier_2_2.x =  pV2Origin[0] + 200;
	m_Bezier_2_2.y = pV2Destination[1] + randY;*/

	//--------------------3번 사출 ------------------------------//
	randX = rand() % 400 - 400;
	randY = rand() % 400 - 200;
	m_Bezier_3_1.x = pV2Origin[0] + randX;
	m_Bezier_3_1.y = pV2Origin[1] + randY;

/*	randY = rand() % 400 - 200;
	m_Bezier_3_2.x =  pV2Origin[0] + 200;
	m_Bezier_3_2.y = pV2Destination[1] + randY;*/

	//--------------------4번 사출 ------------------------------//
	randX = rand() % 400 - 400;
	randY = rand() % 400 - 200;
	m_Bezier_4_1.x = pV2Origin[0] + randX;
	m_Bezier_4_1.y = pV2Origin[1] + randY;

/*	randY = rand() % 400 - 200;
	m_Bezier_4_2.x =  pV2Origin[0] + 200;
	m_Bezier_4_2.y = pV2Destination[1] + randY;*/

	m_Bezier_End.x = pV2Destination[0];
	m_Bezier_End.y = pV2Destination[1];
}


void pbHomingMissileEffect::Draw()
{
	///--------------------------------------
	pbDataStorage::BindVertexAndTexture(m_BodyVertexIndex, m_BodyUVIndex);

	glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);

	//텍스처 바인딩
	glPushMatrix();
	glTranslatef(m_vPos[0], m_vPos[1],  0);
	glScalef(m_fScale*m_fScaleX, m_fScale*m_fScaleY, 1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glPopMatrix();

	//텍스처 바인딩
	glPushMatrix();
	glTranslatef(m_vPos_1[0], m_vPos_1[1],  0);
	glScalef(m_fScale*m_fScaleX, m_fScale*m_fScaleY, 1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glPopMatrix();

	//텍스처 바인딩
	glPushMatrix();
	glTranslatef(m_vPos_2[0], m_vPos_2[1],  0);
	glScalef(m_fScale*m_fScaleX, m_fScale*m_fScaleY, 1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glPopMatrix();

	//텍스처 바인딩
	glPushMatrix();
	glTranslatef(m_vPos_3[0], m_vPos_3[1],  0);
	glScalef(m_fScale*m_fScaleX, m_fScale*m_fScaleY, 1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glPopMatrix();

	//텍스처 바인딩
	glPushMatrix();
	glTranslatef(m_vPos_4[0], m_vPos_4[1],  0);
	glScalef(m_fScale*m_fScaleX, m_fScale*m_fScaleY, 1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glPopMatrix();


	glColor4f(1.0f, 1.0f,1.0f,1.0f);
}

void pbHomingMissileEffect::Update(float fTime)
{
	if( m_bAlive)
	{
		if( !m_bInfinite)
			m_fLifeTime -= fTime;

		m_fBezierLerp += (1/m_fLifeTime)*fTime;

		if(m_fLifeTime < 0.0f)
		{
			m_bAlive = false;
			m_fBezierLerp = 0.0f;
			pbRenderProcess::RemoveRenderEffect(this);
			pbEffectProcess::GetInstance()->RemoveEffectAndReturningMemory(this);

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

		CurrentBezierPoint = GetPointFromBezier3(&m_Bezier_Start, &m_Bezier_4_1, &m_Bezier_End, m_fBezierLerp);
		m_vPos_4[0] = CurrentBezierPoint.x;
		m_vPos_4[1] = CurrentBezierPoint.y;

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
////-----------------------------------------------------이펙트 매니저------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pbEffectProcess* pbEffectProcess::SingleObject = NULL;

pbEffectProcess::pbEffectProcess(){
	m_pEffectLinkHeader = NULL;

	m_pStickerEffectRentalUnit = NULL;
	m_pStepUpEffectRentalUnit = NULL;
	m_pFeverAvailableEffectRentalUnit = NULL;
	m_pHomingMissileEffectRentalUnit = NULL;

}
pbEffectProcess::~pbEffectProcess(){
	if(SingleObject != NULL) {
		ClearDataStore();
	}
}


void pbEffectProcess::Create()
{
	if(SingleObject == NULL)
	{
		SingleObject = new pbEffectProcess();

		SingleObject->m_pEffectLinkHeader = new pbLinkNode<pbEffect>;
		SingleObject->m_pEffectLinkHeader->setHeader();

		SingleObject->m_pStickerEffectRentalUnit = new pbMemoryRentalUnit<pbStickerEffect>;
		SingleObject->m_pStickerEffectRentalUnit->Initialize(5);
		SingleObject->m_pStepUpEffectRentalUnit =new pbMemoryRentalUnit<pbStepUpEffect>;
		SingleObject->m_pStepUpEffectRentalUnit->Initialize(2);
		SingleObject->m_pFeverAvailableEffectRentalUnit =new pbMemoryRentalUnit<pbFeverAvailableEffect>;
		SingleObject->m_pFeverAvailableEffectRentalUnit->Initialize(2);

		SingleObject->m_pHomingMissileEffectRentalUnit =new pbMemoryRentalUnit<pbHomingMissileEffect>;
		SingleObject->m_pHomingMissileEffectRentalUnit->Initialize(4);

		LOGI("pbEffectProcess Create");

		return;
	}

	LOGE("pbEffectProcess Create Failed");
}

void pbEffectProcess::AddStepUpEffect(float X, float Y, GLuint StartVertexIndex, GLuint StartUVIndex, float fScale) {
	//현재 스텝업 이펙트는 하나만 쓰므로 렌탈유닛을 쓰지않는다
	pbStepUpEffect* pEffect = m_pStepUpEffectRentalUnit->RentalMemory();
	if( pEffect == NULL) return ;
	// 17 33
	pEffect->Initialize(StartVertexIndex, StartUVIndex, 0.0f, false);
	pEffect->SetPos(X, Y);
	pEffect->SetScale(fScale);

	pEffect->SetMaxLifeTime(1.0f);

	registControled(pEffect);
	pbRenderProcess::RegistRenderEffect(pEffect);
}

void pbEffectProcess::AddFeverAvailableEffect(float X, float Y, GLuint StartVertexIndex, GLuint StartUVIndex, float fScale) {
	//현재 이 이펙트는 하나만 쓰므로 렌탈유닛을 쓰지않는다
	pbFeverAvailableEffect* pEffect = m_pFeverAvailableEffectRentalUnit->RentalMemory();
	if( pEffect == NULL) return;

	// 23 24
	pEffect->Initialize(StartVertexIndex, StartUVIndex, 0.0f, false);
	pEffect->SetPos(X, Y);
	pEffect->SetScale(fScale);

	pEffect->SetMaxLifeTime(1.0f);

	registControled(pEffect);
	pbRenderProcess::RegistRenderEffect(pEffect);
}

void pbEffectProcess::AddStickerEffect(float X, float Y, GLuint StartUVIndex, float fScale) {
	//스티커 이펙트는 불러온 텍스쳐의 크기와 같은 버텍스를 자동으로 사용한다
	if( m_pStickerEffectRentalUnit != NULL) {
		pbStickerEffect* pEffect = m_pStickerEffectRentalUnit->RentalMemory();
		if( pEffect == NULL) return;

		pEffect->Initialize(17, StartUVIndex, 0.3f, false);
		pEffect->SetPos(X, Y);
		pEffect->SetScale(fScale);

		registControled(pEffect);
		pbRenderProcess::RegistRenderEffect(pEffect);

		LOGE("ADD StickerEffect");
	}
}

void pbEffectProcess::AddHomingMissileEffect(float fStartX, float fStartY, float fDestX, float fDestY, GLuint StartUVIndex, float LifeTime, float fDamage, void (*DecreaseHP)(float fDamage) ) {
	//스티커 이펙트는 불러온 텍스쳐의 크기와 같은 버텍스를 자동으로 사용한다
	if( m_pStickerEffectRentalUnit != NULL) {
		pbHomingMissileEffect* pEffect = m_pHomingMissileEffectRentalUnit->RentalMemory();
		if( pEffect == NULL) return;

		pEffect->Initialize(17, StartUVIndex, LifeTime, false);

		npV2Vector vOrigin, vDestination;
		vOrigin[0] = fStartX;
		vOrigin[1] = fStartY;
		vDestination[0] = fDestX;
		vDestination[1] = fDestY;

		pEffect->SetMissileCurve(vOrigin, vDestination);
		pEffect->SetDecreaseHPFunc(DecreaseHP);
		pEffect->SetDamage(fDamage);

//		pEffect->SetPos(X, Y);
		pEffect->SetScale(1.0f);

		registControled(pEffect);
		pbRenderProcess::RegistRenderEffect(pEffect);

		LOGE("ADD HomingMissileEffect");
	}
}

void pbEffectProcess::AddMissEffect() {
	//미스이펙트는 스티커 이펙트를 설정해서 사용한다
	if( m_pStickerEffectRentalUnit != NULL) {
		pbStickerEffect* pEffect = m_pStickerEffectRentalUnit->RentalMemory();
		if( pEffect == NULL) return;

		pEffect->Initialize(8, 29, 0.1f, false);
		pEffect->SetPos(400, 240);
		pEffect->SetScale(1.0f);

		pEffect->SetWidth(800);
		pEffect->SetHeight(480);

		pEffect->m_Color.R = 1.0f;
		pEffect->m_Color.G = 0.0f;
		pEffect->m_Color.B = 0.0f;
		pEffect->m_Color.A = 1.0f;

		registControled(pEffect);
		pbRenderProcess::RegistRenderEffect(pEffect);
	}
}

void pbEffectProcess::Update(float fTime)
{
	pbLinkNode<pbEffect>* iterator;
	pbLinkNode<pbEffect>* head = m_pEffectLinkHeader;

	iterator = head->getNext();
	while( iterator != head ) {
		pbEffect* pEffect = iterator->getKernel();
		iterator = iterator->getNext();

		pEffect->Update(fTime);
	}

}

void pbEffectProcess::ClearDataStore() {
	if( m_pEffectLinkHeader != NULL)
		pbLinkNode<pbEffect>::ClearList(m_pEffectLinkHeader);

	LOGI("pbEffectProcess ClearDataStore");
}

void pbEffectProcess::Release()
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

		if( SingleObject->m_pFeverAvailableEffectRentalUnit != NULL) {
			SingleObject->m_pFeverAvailableEffectRentalUnit->Release();
			delete SingleObject->m_pFeverAvailableEffectRentalUnit;
			SingleObject->m_pFeverAvailableEffectRentalUnit = NULL;
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

void pbEffectProcess::registControled(pbEffect* pEffect){
	pbLinkNode<pbEffect>* pTargetNode = pbLinkNode<pbEffect>::makeLinkNode(pEffect);
	pbLinkNode<pbEffect>::addTail(pTargetNode,m_pEffectLinkHeader);
}

void pbEffectProcess::removeControled(pbEffect* pEffect){
	pbLinkNode<pbEffect>* pTargetNode = pbLinkNode<pbEffect>::makeLinkNode(pEffect);
	pbLinkNode<pbEffect>::findDelete(pTargetNode,m_pEffectLinkHeader);
}


// 리스트에서 제거하고 렌탈메모리를 반환한다
void pbEffectProcess::RemoveEffectAndReturningMemory(pbStickerEffect* pEffect) {
	if( pEffect != NULL) {
		removeControled(pEffect);
		m_pStickerEffectRentalUnit->ReturningRentalMemory(pEffect);
	}
}

void pbEffectProcess::RemoveEffectAndReturningMemory(pbStepUpEffect* pEffect) {
	if( pEffect != NULL) {
		removeControled(pEffect);
		m_pStepUpEffectRentalUnit->ReturningRentalMemory(pEffect);
	}
}

void pbEffectProcess::RemoveEffectAndReturningMemory(pbFeverAvailableEffect* pEffect) {
	if( pEffect != NULL) {
		removeControled(pEffect);
		m_pFeverAvailableEffectRentalUnit->ReturningRentalMemory(pEffect);
	}
}

void pbEffectProcess::RemoveEffectAndReturningMemory(pbHomingMissileEffect* pEffect) {
	if( pEffect != NULL) {
		removeControled(pEffect);
		m_pHomingMissileEffectRentalUnit->ReturningRentalMemory(pEffect);
	}
}
