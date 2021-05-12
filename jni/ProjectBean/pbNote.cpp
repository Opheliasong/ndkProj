/*
 * pbNote.cpp
 *
 *  Created on: 2012. 7. 10.
 *      Author: NitroPigSoft02
 */

#include "pbTouchLayer.h"
#include "pbNote.h"
#include "DebugUtill.h"
#include "npTimer.h"
//#include "npSoundLib.h"
#include "npAudioSystem.h"
#include "pbDataStorage.h"
#include "pbGlobalVariable.h"
#include "pbEffect.h"
#include "pbCharacter.h"
#include "pbNoteProcessor.h"
#include "pbRenderProcess.h"


namespace projectBean{

//=======================
//pbTouchArea �� ���� ��ü��
//=======================

//---------- pbToucRect------------
/*
 * TouchRect�� ������
 */
pbTouchRect::pbTouchRect(){

}
/*
 * TouchRect�� ���ڸ� ���� ������
 */
pbTouchRect::pbTouchRect(int aTop, int aBottom, int aLeft,int aRight){
	top = aTop;
	bottom = aBottom;
	left = aLeft;
	right = aRight;
}
/*
 * TouchRect�� �Ҹ���
 */
pbTouchRect::~pbTouchRect(){

}

/*
 * TouchRect Rect�� �����ϴ� �κ�
 */
void pbTouchRect::setRects(int aTop, int aBottom, int aLeft,int aRight){
	top = aTop;
	bottom = aBottom;
	left = aLeft;
	right = aRight;
/*
	LOGE("======pbTouchRect::setRects========");
	LOGfloatString("TOP:",top);
	LOGfloatString("bottom:",bottom);
	LOGfloatString("left:",left);
	LOGfloatString("right:",right);
*/
}

/* ��ǥ x,y�� TouchRect���� �ȿ� �ִ����� �Ǻ��ϴ� �Լ�
 *
 */
bool pbTouchRect::IsInRect(float x, float y){
	if(y <= this->top && y >= this->bottom){
		if(x >= this->left && x <= this->right){
			return true;
		}
	}
	return false;
}

//---------- pbToucArea------------
pbTouchArea::pbTouchArea(){
	m_Body = new pbTouchRect;
}

pbTouchArea::~pbTouchArea(){
	LOGE("Default Touch Area=======");
	LOGE("Touch Area Destruction call");

	if(m_Body != NULL){
		delete m_Body;
	}
	LOGE("Default Touch Area call has Done");

}

bool pbTouchArea::InRect(float x, float y){
	/*
	LOGfloatString("=====================",58);
	LOGfloatString("Input X: ",x);
	LOGfloatString("Input y: ",y);
	LOGI("=====================");
	LOGfloatString("Inrect()/Top:",m_Body->top);
	LOGfloatString("Inrect()/bottom:",m_Body->bottom);
	LOGfloatString("Inrect()/left:",m_Body->left);
	LOGfloatString("Inrect()/right:",m_Body->right);
	 */
	/*
	if(y <= m_Body->top && y >= m_Body->bottom){
		if(x >= m_Body->left && x <= m_Body->right){
			return true;
		}
	}
	return false;
	*/
	return m_Body->IsInRect(x,y);
}

void pbTouchArea::movePositon(float x, float y){
	m_Body->setRects(y + m_BodyhalfHeight,y -m_BodyhalfHeight,
							x - m_BodyHalfWidth,x+m_BodyHalfWidth);

	/*m_Body->setRects(y,y+(-m_BodyHalHeight),x,x+m_BodyWidth);*/


/*
	LOGfloatString("movePosition()/Top:",m_Body->top);
	LOGfloatString("movePosition()/bottom:",m_Body->bottom);
	LOGfloatString("movePosition()/left:",m_Body->left);
	LOGfloatString("movePosition()/right:",m_Body->right);
*/
}

void pbTouchArea::setTouchArea(float bodyWidth, float bodyHeight){
	m_BodyWidth= bodyWidth;
	m_BodyHeight = bodyHeight;
	m_BodyHalfWidth = bodyWidth/2;
	m_BodyhalfHeight = bodyHeight/2;
	m_Body->setRects(m_BodyhalfHeight,-m_BodyhalfHeight,-m_BodyHalfWidth,m_BodyHalfWidth);
/*
	LOGfloatString("BodyHeight:",m_BodyHeight);
	LOGfloatString("BodyWidh:",m_BodyWidth);
*/
}

void pbTouchArea::LogPrintRect(){
	LOGfloatString("Top:",m_Body->top);
	LOGfloatString("bottom:",m_Body->bottom);
	LOGfloatString("left:",m_Body->left);
	LOGfloatString("right:",m_Body->right);
}


//=======================
//pbNote�� ����
//=======================
/*
 * pbNote ������
 */
pbNote::pbNote():m_TouchArea(NULL),m_PositionX(0),m_PositionY(0),m_State(NoteState::DEFAULT),m_BodyVertexIndex(0),m_BodyUVIndex(0),m_NoteGrowState(projectBean::NOTEGROWSTATE::NOTEBABY)
{
	if( m_TouchArea == NULL)
		m_TouchArea = new pbTouchArea;

	ValueInitializer();
}

pbNote::pbNote(int ID,unsigned int VertexIndex,int width, int height, unsigned int UvIndex, int x, int y):m_TouchArea(NULL),m_ID(ID),m_BodyVertexIndex(VertexIndex),m_BodyUVIndex(UvIndex),m_PositionX(x),m_PositionY(y)
{
	if( m_TouchArea == NULL)
		m_TouchArea = new pbTouchArea;

	m_TouchArea->movePositon(x,y);
	m_TouchArea->setTouchArea(width,height);

	ValueInitializer();
}

void pbNote::ValueInitializer(){
	m_Types = NOTETYPE::NONETYPE;
	m_NoteJudgement = NOTEJUDGEMENT::NOTJUDGE;
	m_bNotifyPause = false;
	//[FIXLOG:8-30] ���� : Ÿ�ӵ�Ÿ ù���� �� ���� ���� ������ ���� �ɶ����� ��Ƽ�� ���� older���� �����ϵ��� ����
	m_NoteGrowState = NOTEGROWSTATE::NOTEOLDER;

	m_fAlertTime  = 0.0f;
	m_bEndPhase = false;

	m_Color.Init(1.0f,1.0f,1.0f,1.0f);			//�߰� : 8-24
	m_EffectColor.Init(1.0f,1.0f,1.0f,1.0f);			//�߰� : 8-24
	m_fAnimationTime = 0.0f;	//- + 0 ���� ���������� �Ѵ�. �߰� : 8-29
	m_fAnimationPercent = 0.0f;	// �߰� : 8-29

	m_EffectColor.A = 0.0f; // �߰� : 8-29

	m_fEffectScale = 1.0f;
	m_fEffectAngle = 0.0f;

	m_TargetOnSwitch = false;

}


/*
 * pbNote�Ҹ���
 */
pbNote::~pbNote(){
	/*
	LOGE("pbNote Destruction=============");
	LOGE("Default Note Destruction call");*/
	delete m_TouchArea;
}

void pbNote::notify(){

}

void pbNote::notify(int x, int y, TOUCHSTATUS::TYPE Touchstatus){
	switch(Touchstatus){
		case TOUCHSTATUS::TAPDOWN:{
			CheckingTouchOn(x,y);
			break;
		}
		case TOUCHSTATUS::TAPUP:{
//			m_States = 0;
			break;
		}
		case TOUCHSTATUS::TAPMOVE:{
			CheckingTouchOn(x,y);
			break;
		}
	}
}

void pbNote::notify(TOUCHSTATUS::TYPE touchStatus){

/*	if(touchStatus == TABUP){
		m_States = 0;
	}*/
}

void pbNote::wakeUpNotify(){

}
float pbNote::resetWakeTime(){
	return m_fAlertTime;
}

// ���� : 8-24
bool pbNote::CheckingTouchOn(int x, int y){
	if(m_TouchArea->InRect(x,y)){
		this->m_State = NoteState::TOUCHED;

		return true;
	}

	return false;
/*		if(m_TouchArea->InRect(x,y)){
			this->m_States = 1;
		}
		else{
			m_States = 0;
		}

		return m_States;*/
}

void pbNote::setNotePosition(float x, float y){
	m_PositionX = x;
	m_PositionY = y;

	m_TouchArea->movePositon(x,y);
}

void pbNote::setTouchWidthAndHeight(int width, int height){
	m_TouchArea->setTouchArea(width,height);
}

void pbNote::JudgeMissNote(){
	pbTouchLayer::removeObserver(this);
	nitroFrame::npTimer::findRemove(this);

	//��� ���� �ڵ�
	m_bEndPhase = true;
	m_State = NoteState::END;
	m_NoteJudgement = NOTEJUDGEMENT::MISS;

}

void pbNote::randOnSwitch(){
	double randNum = (double)rand()/RAND_MAX * 2;
//	LOGfloatString("[[TargetNote::randOnSwitch]]RandNum:",randNum);
	if(randNum >= 0.2){
		m_TargetOnSwitch = true;
//		LOGI("[[TargetNote::randOnSwitch]]On!");
	}else{
		m_TargetOnSwitch = false;
//		LOGI("[[TargetNote::randOnSwitch]]Off!");
	}
}

//---------- pbSwipeNote------------
/*
 * pbSwipeNote ������
 */
pbSwipeNote::pbSwipeNote(){
	m_Types = NOTETYPE::SWIPE;

}

/*
 * SwipeNote �Ҹ���
 */
pbSwipeNote::~pbSwipeNote(){
}

void pbSwipeNote::ExtraInitialize()
{
}

void pbSwipeNote::notify(int x, int y, TOUCHSTATUS::TYPE Touchstatus) {
	switch (Touchstatus) {
		case TOUCHSTATUS::TAPDOWN:
		case TOUCHSTATUS::TAPMOVE: {
			//x,y�� ���ؼ� �����Ǵ��� �Ѵ�.
			if (projectBean::pbNote::CheckingTouchOn(x, y)) {
				if(m_TargetOnSwitch) {
					m_BodyUVIndex = m_AniUVStartIndex;
					m_bEndPhase = true;
					m_State = NoteState::END;
					m_fAnimationPercent = 0.0f;
					m_fAnimationTime = 0.0f;
					pbTouchLayer::removeObserver(this);
					nitroFrame::npTimer::findRemove(this);

					npAudioSystem::playEffect(0);
					pbUserData::SingleObject->UsingAbilityPoint(10);
					pbComboManager::GetInstance()->IncreaseCombo(789);
				}
				else	{
//					pbEffectProcess::pEffectProcessor->AddEffect(EFFECTTYPE::JUDGE, m_PositionX, m_PositionY+ JUDGE_EFFECT_H, 1.0f, NOTEJUDGEMENT::FAIL);
					m_NoteJudgement = NOTEJUDGEMENT::NOTJUDGE;	//���н� �ʱ�ȭ
					pbUserData::SingleObject->UsingAbilityPoint(30);
					pbComboManager::GetInstance()->ResetCombo();

					pbTouchLayer::removeObserver(this);
					nitroFrame::npTimer::findRemove(this);
				}//end if target
			}//end if check touch
		}
		break;
	}//end switch
}

/*
 * Tab��Ʈ�� Ÿ�̸ӿ� ���� ���� ��ȭ �Լ�
 * �ش� TabNote�� ���°� ���� Ÿ�ֿ̹� ȣ���� �ȴ�.
 * TabNote�� ��� None, Up, Complete, Down ���� ���°� ��ȭ �Ǹ� �� ���� ��ȭ�� ���� ������ Ʋ������ �ȴ�.
 *
 * ������ ��Ʈ�� ���¿��� Ÿ�̸ӿ� ���� CallBack�� �Ǿ��� ��� ���� ���·� ��ȭ�� ��Ų��.
 */
void pbSwipeNote::wakeUpNotify(){
	if(m_NoteGrowState == NOTEGROWSTATE::NOTEBABY){
		m_NoteGrowState = NOTEGROWSTATE::NOTEADULT;
		m_EffectColor.A = 1.0f;
		m_fEffectScale  = 3.0f;
	}else if(m_NoteGrowState == NOTEGROWSTATE::NOTEADULT){
	}else if(m_NoteGrowState == NOTEGROWSTATE::NOTEGROWUP){
	}else if(m_NoteGrowState == NOTEGROWSTATE::NOTEOLDER){
		m_NoteGrowState = NOTEGROWSTATE::NOTEBABY;
	}
}

float pbSwipeNote::resetWakeTime(){
	switch (m_NoteGrowState) {
		case NOTEGROWSTATE::NOTEBABY: {
			return 500.f;
			break;
		}
		case NOTEGROWSTATE::NOTEADULT: {
			return 35000.f;
			break;
		}
		case NOTEGROWSTATE::NOTEGROWUP:{
			return 0;
		}
		case NOTEGROWSTATE::NOTEOLDER:{
			return 0;
		}
	}
}

void pbSwipeNote::setVertexIndices(GLuint  BodyIndex, GLuint EffectIndex)
{
	m_BodyVertexIndex = BodyIndex;
	m_EffectVertexIndex = EffectIndex;
}

void pbSwipeNote::setUVIndices(GLuint  BodyIndex, GLuint EffectIndex)
{
	m_BodyUVIndex = BodyIndex;
	m_EffectUVIndex = EffectIndex;

	m_AniUVStartIndex = BodyIndex;
	m_AniUVEndIndex = BodyIndex + ANICOUNT;
}

void pbSwipeNote::Draw() {
	/////�ٵ�
		glPushMatrix();

		BindColor();
		pbDataStorage::BindVertexAndTexture(m_BodyVertexIndex, m_BodyUVIndex);
		glTranslatef(m_PositionX, m_PositionY, 0.f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glPopMatrix();

	/////����Ʈ
		if(!m_bEndPhase)
		{
			BindEffectColor();
			glPushMatrix();

			pbDataStorage::BindVertexAndTexture(m_EffectVertexIndex, m_EffectUVIndex);
			glTranslatef(m_PositionX, m_PositionY, 0.f);
			glScalef(m_fEffectScale, m_fEffectScale, 1.0f);
			glRotatef(m_fEffectAngle, 0.0f, 0.0f, 1.0f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			glPopMatrix();
		}
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void pbSwipeNote::Update(float fTime) {
	float x = m_PositionX + pbGlobalInGameVariable::fWorldMoveX;
	float y = m_PositionY;

	if( m_NoteJudgement == NOTEJUDGEMENT::MISS) {	//�̽� üũ
		//��� ���� �ڵ�
		pbRenderProcess::RemoveRenderNote(this);
		pbNoteProcessor::GetInstance()->RemoveNoteAndReturningMemory(this);
		return;
	}

	if( !m_bEndPhase )
	{
		if(m_NoteGrowState == NOTEGROWSTATE::NOTEADULT){

			m_fEffectAngle += 360*fTime;

			m_fAnimationPercent += (1/0.5)*fTime;
			if( m_fAnimationPercent > 1.0f ) {
				m_fAnimationPercent = 1.0f;
			}
			m_EffectColor.G = 1.0f - m_fAnimationPercent;
			m_EffectColor.B = m_EffectColor.G;
			m_fEffectScale = Lerp(3.0f, 1.0f, m_fAnimationPercent );

		}

	}
	else if( m_bEndPhase )
	{
		//------------------------�ִϸ��̼� ����-----------------------------//
		m_fAnimationTime += fTime;

		if( m_fAnimationTime > 0.04f)
		{
			m_fAnimationTime = 0.0f;
			m_BodyUVIndex++;
			if(m_BodyUVIndex == m_AniUVEndIndex)
			{
				m_BodyUVIndex = m_AniUVStartIndex;

				m_bEndPhase = false;

				//��� ���� �ڵ�
				pbRenderProcess::RemoveRenderNote(this);
				pbNoteProcessor::GetInstance()->RemoveNoteAndReturningMemory(this);
				return;
			}
		}
		//=======================================//
	}// end if Check EndPhase


	setNotePosition(x,y);
}

//---------- pbDoubleTabNote------------
/*
 * pbDoubleTabNote ������
 */
pbDoubleTapNote::pbDoubleTapNote(){
	m_Types = NOTETYPE::DTAP;

}

/*
 * DoubleNote �Ҹ���
 */
pbDoubleTapNote::~pbDoubleTapNote(){
}

void pbDoubleTapNote::ExtraInitialize()
{
	m_iNoteTouchCount = 0;
	m_fSecondNoteAlpha = 0.0f;
	m_fSecondNoteTime = 0.0f;
}

void pbDoubleTapNote::notify(int x, int y, TOUCHSTATUS::TYPE Touchstatus) {
	switch (Touchstatus) {
		case TOUCHSTATUS::TAPDOWN: {
			//x,y�� ���ؼ� �����Ǵ��� �Ѵ�.
			if (projectBean::pbNote::CheckingTouchOn(x, y)) {
				if( m_TargetOnSwitch )
				{
					if(m_iNoteTouchCount == 0 )
					{
						if( m_NoteJudgement != NOTEJUDGEMENT::FAIL)
						{
							npAudioSystem::playEffect(0);
							m_fSecondNoteTime  = 0.0f;
							m_iNoteTouchCount = 1;

							m_bNotifyPause = true;
						}
					}
					else if(m_iNoteTouchCount == 1)
					{
						if( m_bNotifyPause) //������Ʈ�� üũ�� ��ġ üũ ���ÿ� �Ǵ� ���� ����
						{
							m_fSecondNoteTime = 0.0f;
							m_bNotifyPause = false;
							m_bEndPhase = true;
							m_State = NoteState::END;
							m_fAnimationPercent = 0.0f;
							m_fAnimationTime = 0.0f;
							pbTouchLayer::removeObserver(this);
							nitroFrame::npTimer::findRemove(this);

							m_iNoteTouchCount = 0;
							pbComboManager::GetInstance()->IncreaseCombo(1125);

							pbUserData::SingleObject->UsingAbilityPoint(30);
						}
					}//end if count
				}
				else {
//					pbEffectProcess::pEffectProcessor->AddEffect(EFFECTTYPE::JUDGE, m_vSecondNoteCurrentPos[0], m_vSecondNoteCurrentPos[1] + JUDGE_EFFECT_H, 1.0f, NOTEJUDGEMENT::FAIL);
					m_NoteJudgement = NOTEJUDGEMENT::NOTJUDGE;	//���н� �ʱ�ȭ
					m_iNoteTouchCount = 0;

					pbUserData::SingleObject->UsingAbilityPoint(120);
					pbComboManager::GetInstance()->ResetCombo();

					pbTouchLayer::removeObserver(this);
					nitroFrame::npTimer::findRemove(this);
				}//end if target
			}//end if check touch
		}
		break;
	}
}

/*
 * Tab��Ʈ�� Ÿ�̸ӿ� ���� ���� ��ȭ �Լ�
 * �ش� TabNote�� ���°� ���� Ÿ�ֿ̹� ȣ���� �ȴ�.
 * TabNote�� ��� None, Up, Complete, Down ���� ���°� ��ȭ �Ǹ� �� ���� ��ȭ�� ���� ������ Ʋ������ �ȴ�.
 *
 * ������ ��Ʈ�� ���¿��� Ÿ�̸ӿ� ���� CallBack�� �Ǿ��� ��� ���� ���·� ��ȭ�� ��Ų��.
 */
void pbDoubleTapNote::wakeUpNotify(){
	if(m_NoteGrowState == NOTEGROWSTATE::NOTEBABY){
		m_NoteGrowState = NOTEGROWSTATE::NOTEADULT;
		m_EffectColor.A = 1.0f;
		m_fEffectScale  = 3.0f;
	}else if(m_NoteGrowState == NOTEGROWSTATE::NOTEADULT){
	}else if(m_NoteGrowState == NOTEGROWSTATE::NOTEGROWUP){
	}else if(m_NoteGrowState == NOTEGROWSTATE::NOTEOLDER){
		m_NoteGrowState = NOTEGROWSTATE::NOTEBABY;
	}
}

float pbDoubleTapNote::resetWakeTime(){
	switch (m_NoteGrowState) {
		case NOTEGROWSTATE::NOTEBABY: {
			return 500.f;
			break;
		}
		case NOTEGROWSTATE::NOTEADULT: {
			return 35000.f;
			break;
		}
		case NOTEGROWSTATE::NOTEGROWUP:{
			return 0;
		}
		case NOTEGROWSTATE::NOTEOLDER:{
			return 0;
		}
	}
}

void pbDoubleTapNote::setNotePosition(float x, float y){
	m_PositionX = x;
	m_PositionY = y;
	m_vSecondNoteCurrentPos[0] = m_PositionX;
	m_vSecondNoteCurrentPos[1] = m_PositionY;
	m_TouchArea->movePositon(x,y);
}

void pbDoubleTapNote::SetSecondNotePosDelta(GLfloat DistanceFromPosX, GLfloat DistanceFromPosY)
{
	m_vSecondNoteDestDistance[0] = DistanceFromPosX;
	m_vSecondNoteDestDistance[1] =DistanceFromPosY;
}

void pbDoubleTapNote::setVertexIndices(GLuint  BodyIndex,GLuint EffectIndex)
{
	m_BodyVertexIndex = BodyIndex;
	m_EffectVertexIndex = EffectIndex;
}

void pbDoubleTapNote::setUVIndices(GLuint  BodyIndex, GLuint  EffectIndex)
{
	m_BodyUVIndex = BodyIndex;  m_EffectUVIndex = EffectIndex;

	m_AniUVStartIndex = BodyIndex;
	m_AniUVEndIndex = BodyIndex + ANICOUNT;
}


void pbDoubleTapNote::Draw() {
	/////2��° ��Ʈ
		glPushMatrix();
		glColor4f(1.0f, 1.0f, 1.0f, m_fSecondNoteAlpha);
		pbDataStorage::BindVertexAndTexture(m_BodyVertexIndex, m_BodyUVIndex);
		glTranslatef(m_vSecondNoteCurrentPos[0], m_vSecondNoteCurrentPos[1], 0.f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glPopMatrix();

	/////�ٵ�
		if( !m_bEndPhase) {
			glPushMatrix();

			BindColor();
			pbDataStorage::BindVertexAndTexture(m_BodyVertexIndex, m_BodyUVIndex);
			glTranslatef(m_PositionX, m_PositionY, 0.f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			glPopMatrix();

			/////����Ʈ
			BindEffectColor();
			glPushMatrix();

			pbDataStorage::BindVertexAndTexture(m_EffectVertexIndex, m_EffectUVIndex);
			glTranslatef(m_PositionX, m_PositionY, 0.f);
			glScalef(m_fEffectScale, m_fEffectScale, 1.0f);
			glRotatef(m_fEffectAngle, 0.0f, 0.0f, 1.0f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			glPopMatrix();

		}
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void pbDoubleTapNote::Update(float fTime) {
	float x = m_PositionX + pbGlobalInGameVariable::fWorldMoveX;
	float y = m_PositionY;

	if( m_NoteJudgement == NOTEJUDGEMENT::MISS) {	//�̽� üũ
		//��� ���� �ڵ�
		pbRenderProcess::RemoveRenderNote(this);
		pbNoteProcessor::GetInstance()->RemoveNoteAndReturningMemory(this);
		return;
	}

	if( !m_bEndPhase )
	{
		if(m_NoteGrowState== NOTEGROWSTATE::NOTEADULT){

			m_fEffectAngle += 360*fTime;

			m_fAnimationPercent += (1/0.5)*fTime;
			if( m_fAnimationPercent > 1.0f ) {
				m_fAnimationPercent = 1.0f;
			}
			m_EffectColor.G = 1.0f - m_fAnimationPercent;
			m_EffectColor.B = m_EffectColor.G;
			m_fEffectScale = Lerp(3.0f, 1.0f, m_fAnimationPercent );

		}

		if( m_iNoteTouchCount == 0)
		{
			/////----------------------���� �ִϸ��̼� -----------------------///
			if( m_fAnimationTime != 0.0f )
			{
				m_fAnimationPercent += fTime*(1/m_fAnimationTime);
				float m_fChangeValue = 0.0f;
				if( m_fAnimationTime > 0.0f ) { // ��½�
					m_fChangeValue = 0.5f + m_fAnimationPercent*0.5f;
					m_EffectColor.A  = m_fAnimationPercent;
				}
				else if( m_fAnimationTime < 0.0f ) 	{  //�ϰ��� ColorChangeLerp �� ���̳ʽ� ��
					m_fChangeValue = 1.0f + m_fAnimationPercent*0.5f;
					m_EffectColor.A  = 1 + m_fAnimationPercent;
				}

				m_Color.R = m_fChangeValue;
				m_Color.G = m_fChangeValue;
				m_Color.B = m_fChangeValue;

			}
			//===============================================================================//
		}
		else if( m_iNoteTouchCount == 1)
		{
			//---------------���� �ð� ����-----------------//
			m_fSecondNoteTime += fTime*(1/2.0f);
			if( m_fSecondNoteTime > 1.0f) {

				LOGE("COUNT CANCEL");
				m_fSecondNoteTime = 0.0f;
				m_iNoteTouchCount = 0;

				m_fSecondNoteAlpha = 0.0f;
				m_Color.A = 1.0f;

				m_vSecondNoteCurrentPos[0] = m_PositionX;
				m_vSecondNoteCurrentPos[1] = m_PositionY;
				getTouchArea()->movePositon(m_vSecondNoteCurrentPos[0], m_vSecondNoteCurrentPos[1]);

				m_bNotifyPause = false;
				return;
			}
			//===============================================================================//

			if( m_fSecondNoteAlpha < 1.0f) 	{
				m_fSecondNoteAlpha += 0.2f;
				m_Color.A -= 0.1f;
			}

			m_vSecondNoteCurrentPos[0] += pbGlobalInGameVariable::fWorldMoveX;
			m_vSecondNoteCurrentPos[0] = Lerp(m_vSecondNoteCurrentPos[0], m_PositionX + m_vSecondNoteDestDistance[0], 0.2f);
			m_vSecondNoteCurrentPos[1] = Lerp(m_vSecondNoteCurrentPos[1], m_PositionY + m_vSecondNoteDestDistance[1], 0.2f);

			m_PositionX = x;
			m_PositionY = y;

			//��ġ ���� �Űܳ���
			getTouchArea()->movePositon(m_vSecondNoteCurrentPos[0], m_vSecondNoteCurrentPos[1]);
			return;	//update�Լ� �������� ó���ϴ� setnotePosition(x,y)�� ��ġ������ ������ �ʱ�ȭ ��Ű�Ƿ� ����ؾ��Ѵ�
		}
	}
	else if( m_bEndPhase )
	{
		m_vSecondNoteCurrentPos[0] += pbGlobalInGameVariable::fWorldMoveX;

		//------------------------�ִϸ��̼� ����-----------------------------//
		m_fAnimationTime += fTime;

		if( m_fAnimationTime > 0.04f)
		{
			m_fAnimationTime = 0.0f;
			m_BodyUVIndex++;
			if(m_BodyUVIndex == m_AniUVEndIndex)
			{
				m_BodyUVIndex = m_AniUVStartIndex;

				m_bEndPhase = false;

				//��� ���� �ڵ�
				pbRenderProcess::RemoveRenderNote(this);
				pbNoteProcessor::GetInstance()->RemoveNoteAndReturningMemory(this);
				return;
			}
		}
		//=======================================//

		return;	//�ι�° ��Ʈ�� �̵� �ʱ�ȭ ����
	}// end if Check EndPhase


	setNotePosition(x,y);
}

//---------- pbLongPressNote------------
/*
 * LongPress Note ������
 */
pbLongPressNote::pbLongPressNote(){
	m_Types = NOTETYPE::LPRESS;
}

/*
 * pbLongPressNote �Ҹ���
 */
pbLongPressNote::~pbLongPressNote(){
}

void pbLongPressNote::ExtraInitialize()
{
	m_fActionPointDistance = 0.0f;			//�ص������ ���۵Ǵ� ����
	m_fActionPointPosY = 0.0f;
	m_fPreVisionPosX = 0.0f;					//�������־���� ���� ǥ���ϴ� ��ġ
	m_fDestinationPosY = 0.0f;			//���������� ���ԵǴ� ��

	m_fPressPercent = 0.0f;
	m_fPressTime = 0.0f;

	m_bNotePressed = false;

	m_iPhase = 0;

	m_fRotate = 0.0f;
}

void pbLongPressNote::SetDestination(GLfloat DistanceFromPosX, GLfloat DistanceFromPosY){
	m_fPressTime = (DistanceFromPosX - pbDataStorage::GetVertexWidth(m_BodyVertexIndex) )/pbGlobalInGameVariable::fWorldMoveSpeed;	// ������ �ð��� ���� �۴�
	m_fPreVisionPosX = m_PositionX + (DistanceFromPosX - pbDataStorage::GetVertexWidth(m_BodyVertexIndex) );
	m_fActionPointDistance =  DistanceFromPosX - ( pbDataStorage::GetVertexWidth(m_BodyVertexIndex)/2 );	//�������� ������ ��ġ�� �ʺ��� ��
	m_fActionPointPosY = m_PositionY;
	m_fDestinationPosY = m_PositionY + DistanceFromPosY;
}

void pbLongPressNote::notify(int x, int y, TOUCHSTATUS::TYPE Touchstatus) {
	switch (Touchstatus) {
		case TOUCHSTATUS::TAPDOWN: {
			//x,y�� ���ؼ� �����Ǵ��� �Ѵ�.
			if (projectBean::pbNote::CheckingTouchOn(x, y)) {
				if( m_TargetOnSwitch)
				{
					m_bNotePressed = true;
					npAudioSystem::playEffect(0);

					pbUserData::SingleObject->UsingAbilityPoint(30);
				}
				else {
//					pbEffectProcess::pEffectProcessor->AddEffect(EFFECTTYPE::JUDGE, m_PositionX, m_PositionY+ JUDGE_EFFECT_H, 1.0f, NOTEJUDGEMENT::FAIL);
					m_NoteJudgement = NOTEJUDGEMENT::NOTJUDGE;	//���н� �ʱ�ȭ
					pbUserData::SingleObject->UsingAbilityPoint(120);
					pbComboManager::GetInstance()->ResetCombo();

					pbTouchLayer::removeObserver(this);
					nitroFrame::npTimer::findRemove(this);
				}
			}
			break;
		case TOUCHSTATUS::TAPUP:
			{
				if (m_bNotePressed) {	//������ ���¿��� �Ǿ� ������ ����
					m_bNotePressed = false;

					if (m_fPressPercent < 1.0f) {
//						m_NoteJudgement = NOTEJUDGEMENT::FAIL;
//						pbEffectProcess::pEffectProcessor->AddEffect(	EFFECTTYPE::JUDGE, m_PositionX, m_PositionY+ JUDGE_EFFECT_H,	 1.0f, m_NoteJudgement);
						m_NoteJudgement = NOTEJUDGEMENT::NOTJUDGE; //���н� �ʱ�ȭ

					}
				}
			}
			break;
		}
	}
}

void pbLongPressNote::setVertexIndices(GLuint  BodyIndex)
{
	m_BodyVertexIndex = BodyIndex; m_DestinationVertIndex = BodyIndex;
	m_EffectVertexIndex = BodyIndex;
}

void pbLongPressNote::setUVIndices(GLuint  BodyIndex, GLuint  EffectIndex, GLuint  DestinationIndex)
{
	m_BodyUVIndex = BodyIndex; m_DestinationUVIndex = DestinationIndex;
	m_EffectUVIndex = EffectIndex;

}

void pbLongPressNote::wakeUpNotify(){
	if(m_NoteGrowState == NOTEGROWSTATE::NOTEBABY){
		m_NoteGrowState = NOTEGROWSTATE::NOTEADULT;
		m_EffectColor.A = 1.0f;
		m_fEffectScale  = 3.0f;
		m_iPhase = 0;
		m_fPressPercent = 0.0f;
	}else if(m_NoteGrowState == NOTEGROWSTATE::NOTEADULT){
	}else if(m_NoteGrowState == NOTEGROWSTATE::NOTEGROWUP){
	}else if(m_NoteGrowState == NOTEGROWSTATE::NOTEOLDER){
		m_NoteGrowState = NOTEGROWSTATE::NOTEBABY;
	}
}

float pbLongPressNote::resetWakeTime(){
	switch (m_NoteGrowState) {
		case NOTEGROWSTATE::NOTEBABY: {
			return 500.f;
			break;
		}
		case NOTEGROWSTATE::NOTEADULT: {
			return 35000.f;
			break;
		}
		case NOTEGROWSTATE::NOTEGROWUP:{
			return 0;
		}
		case NOTEGROWSTATE::NOTEOLDER:{
			return 0;
		}
	}
}

void pbLongPressNote::Draw() {
	/////�ٵ�
		glPushMatrix();

		BindColor();
		pbDataStorage::BindVertexAndTexture(m_BodyVertexIndex, m_BodyUVIndex);
		glTranslatef(m_PositionX, m_PositionY, 0.f);
		glRotatef(m_fRotate*360.0f, 0.0f, 0.0f, 1.0f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glPopMatrix();
	/////��ǥ
		if(!m_bEndPhase)
		{
			glPushMatrix();

			BindColor();
			pbDataStorage::BindVertexAndTexture(m_DestinationVertIndex, m_DestinationUVIndex);
			glTranslatef(m_fPreVisionPosX, m_fActionPointPosY, 0.f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			glPopMatrix();

			BindEffectColor();
			glPushMatrix();

			pbDataStorage::BindVertexAndTexture(m_EffectVertexIndex, m_EffectUVIndex);
			glTranslatef(m_PositionX, m_PositionY, 0.f);
			glScalef(m_fEffectScale, m_fEffectScale, 1.0f);
			glRotatef(m_fEffectAngle, 0.0f, 0.0f, 1.0f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			glPopMatrix();
		}

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	//	LOGE("RENDER LNOTE");
}

void pbLongPressNote::Update(float fTime) {
	float BodyPosX = m_PositionX;
	float BodyPosY = m_PositionY;

	if( m_NoteJudgement == NOTEJUDGEMENT::MISS) {	//�̽� üũ
		//��� ���� �ڵ�
		pbRenderProcess::RemoveRenderNote(this);
		pbNoteProcessor::GetInstance()->RemoveNoteAndReturningMemory(this);
		return;
	}

	if( !m_bEndPhase )
	{
		if(m_NoteGrowState == NOTEGROWSTATE::NOTEADULT){

			m_fEffectAngle += 360*fTime;

			m_fAnimationPercent += (1/0.5)*fTime;
			if( m_fAnimationPercent > 1.0f ) {
				m_fAnimationPercent = 1.0f;
			}
			m_EffectColor.G = 1.0f - m_fAnimationPercent;
			m_EffectColor.B = m_EffectColor.G;
			m_fEffectScale = Lerp(3.0f, 1.0f, m_fAnimationPercent );

		}

		BodyPosX += pbGlobalInGameVariable::fWorldMoveX;
		m_fPreVisionPosX += pbGlobalInGameVariable::fWorldMoveX;
		if( m_bNotePressed  )
		{
			BodyPosX = m_PositionX;
			m_fActionPointDistance += pbGlobalInGameVariable::fWorldMoveX;

			m_fRotate -= (1/0.6)*fTime;
			m_fPressPercent +=  fTime*(1/m_fPressTime);

			if(m_fPressPercent >= 1.0f )
			{
				m_fPressPercent = 0.0f;
				m_NoteJudgement = NOTEJUDGEMENT::GREAT;

				m_State = NoteState::END;
				m_bEndPhase = true;
				m_fAnimationPercent = 0.0f;
				pbTouchLayer::removeObserver(this);
				nitroFrame::npTimer::findRemove(this);

				pbComboManager::GetInstance()->IncreaseCombo(2368);
			}//endif Presstime Check

		}//endif notePressed

	}
	else if( m_bEndPhase && m_iPhase != 1)
	{
		BodyPosX -= pbGlobalInGameVariable::fWorldMoveX;//�����ϱ� ���� �ݴ�� �� �����ش�
		m_fActionPointDistance += pbGlobalInGameVariable::fWorldMoveX;

		m_fRotate -= (1/0.55)*fTime;

		if (m_iPhase == 0)
		{
			m_fPressPercent +=  fTime*(1/0.7f);

			 if( m_fPressPercent > 1.0f )
			 {
				 m_iPhase++;

				pbRenderProcess::RemoveRenderNote(this);
				pbNoteProcessor::GetInstance()->RemoveNoteAndReturningMemory(this);
			 }
			 else
			 {
				 m_Color.A = 1.0f - m_fPressPercent;
			 }
		}
	}

	setNotePosition(BodyPosX,BodyPosY);
}


pbTargetingNote::pbTargetingNote(){
	m_Types = NOTETYPE::TARGETING;
}
pbTargetingNote::~pbTargetingNote(){

}
/*
 * breif: TargetingNote�� TouchLayer���� callback �Ǿ����� �Լ�
 *
 */
void pbTargetingNote::notify(int x, int y, TOUCHSTATUS::TYPE Touchstatus){
	switch (Touchstatus) {
		case projectBean::TOUCHSTATUS::TAPDOWN: {
			if (projectBean::pbNote::CheckingTouchOn(x, y)) {

				if( m_TargetOnSwitch)
				{
					m_bEndPhase = true;
					m_State = NoteState::END;
					m_fAnimationPercent = 0.0f;
	//				m_fAnimationTime = 0.1f;
					pbTouchLayer::removeObserver(this);
					nitroFrame::npTimer::findRemove(this);

					npAudioSystem::playEffect(0);
					pbUserData::SingleObject->UsingAbilityPoint(30);
					pbComboManager::GetInstance()->IncreaseCombo(1234);
				}
				else
				{
	//				pbEffectProcess::pEffectProcessor->AddEffect(EFFECTTYPE::JUDGE, m_PositionX, m_PositionY+ JUDGE_EFFECT_H, 1.0f, NOTEJUDGEMENT::FAIL);
					m_NoteJudgement = NOTEJUDGEMENT::NOTJUDGE;	//���н� �ʱ�ȭ
					pbUserData::SingleObject->UsingAbilityPoint(120);
					pbComboManager::GetInstance()->ResetCombo();

					pbTouchLayer::removeObserver(this);
					nitroFrame::npTimer::findRemove(this);
				}//endif Target

			}//endif Check touch

		}
		break;
	}//end switch

}

void pbTargetingNote::setVertexIndices(GLuint  BodyIndex,GLuint EffectIndex){
	m_BodyVertexIndex = BodyIndex;
	m_EffectVertexIndex = EffectIndex;
}
void pbTargetingNote::setUVIndices(GLuint  BodyIndex, GLuint EffectIndex){
	m_BodyUVIndex = BodyIndex;
	m_EffectUVIndex = EffectIndex;

	m_AniUVStartIndex = BodyIndex;
	m_AniUVEndIndex = BodyIndex + ANICOUNT;
}

void pbTargetingNote::wakeUpNotify(){
	if(m_NoteGrowState == NOTEGROWSTATE::NOTEBABY){
		m_NoteGrowState = NOTEGROWSTATE::NOTEADULT;
		m_EffectColor.A = 1.0f;
		m_fEffectScale  = 3.0f;
	}else if(m_NoteGrowState == NOTEGROWSTATE::NOTEADULT){
	}else if(m_NoteGrowState == NOTEGROWSTATE::NOTEGROWUP){
	}else if(m_NoteGrowState == NOTEGROWSTATE::NOTEOLDER){
		m_NoteGrowState = NOTEGROWSTATE::NOTEBABY;
	}
}

float pbTargetingNote::resetWakeTime(){
	switch (m_NoteGrowState) {
		case NOTEGROWSTATE::NOTEBABY: {
			return 500.f;
			break;
		}
		case NOTEGROWSTATE::NOTEADULT: {
			return 35000.f;
			break;
		}
		case NOTEGROWSTATE::NOTEGROWUP:{
			return 0;
		}
		case NOTEGROWSTATE::NOTEOLDER:{
			return 0;
		}
	}
}

void pbTargetingNote::Draw() {
	/////�ٵ�
		glPushMatrix();

		BindColor();
		pbDataStorage::BindVertexAndTexture(m_BodyVertexIndex, m_BodyUVIndex);
		glTranslatef(m_PositionX, m_PositionY, 0.f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glPopMatrix();

	/////����Ʈ
		if(!m_bEndPhase)
		{
			BindEffectColor();
			glPushMatrix();

			pbDataStorage::BindVertexAndTexture(m_EffectVertexIndex, m_EffectUVIndex);
			glTranslatef(m_PositionX, m_PositionY, 0.f);
			glScalef(m_fEffectScale, m_fEffectScale, 1.0f);
			glRotatef(m_fEffectAngle, 0.0f, 0.0f, 1.0f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			glPopMatrix();
		}
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void pbTargetingNote::Update(float fTime) {
	float x = m_PositionX + pbGlobalInGameVariable::fWorldMoveX;
	float y =m_PositionY;

	if(m_NoteJudgement == NOTEJUDGEMENT::MISS){
		pbRenderProcess::RemoveRenderNote(this);
		pbNoteProcessor::GetInstance()->RemoveNoteAndReturningMemory(this);
		return;
	}

	if( !m_bEndPhase )
	{
		if(m_NoteGrowState == NOTEGROWSTATE::NOTEADULT){
			if(m_TargetOnSwitch) {
				m_fEffectAngle += 360*fTime;

				m_fAnimationPercent += (1/0.5)*fTime;
				if(m_fAnimationPercent > 1.0f ) {
					m_fAnimationPercent = 1.0f;
				}
				m_EffectColor.G = 1.0f -m_fAnimationPercent;
				m_EffectColor.B =m_EffectColor.G;
				m_fEffectScale = Lerp(3.0f, 1.0f,m_fAnimationPercent );
			}
		}
	}
	else if(m_bEndPhase){
		/////----------------------������ �ִϸ��̼� -----------------------///
		//------------------------�ִϸ��̼� ����-----------------------------//
		m_fAnimationTime += fTime;

		if(m_fAnimationTime > 0.04f)
		{
			m_fAnimationTime = 0.0f;
			m_BodyUVIndex++;
			if(m_BodyUVIndex ==m_AniUVEndIndex)
			{
				m_BodyUVIndex =m_AniUVStartIndex;

				m_bEndPhase = false;

				//��� ���� �ڵ�
				pbRenderProcess::RemoveRenderNote(this);
				pbNoteProcessor::GetInstance()->RemoveNoteAndReturningMemory(this);
				return;
			}
		}
	}

	setNotePosition(x,y);
}


}
