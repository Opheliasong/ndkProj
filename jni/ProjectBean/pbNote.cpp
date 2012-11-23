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
//pbTouchArea 및 하위 객체군
//=======================

//---------- pbToucRect------------
/*
 * TouchRect의 생성자
 */
pbTouchRect::pbTouchRect(){

}
/*
 * TouchRect의 인자를 가진 생성자
 */
pbTouchRect::pbTouchRect(int aTop, int aBottom, int aLeft,int aRight){
	top = aTop;
	bottom = aBottom;
	left = aLeft;
	right = aRight;
}
/*
 * TouchRect의 소멸자
 */
pbTouchRect::~pbTouchRect(){

}

/*
 * TouchRect Rect를 설정하는 부분
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

/* 좌표 x,y가 TouchRect영역 안에 있는지를 판별하는 함수
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
//pbNote군 영역
//=======================
/*
 * pbNote 생성자
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
	//[FIXLOG:8-30] 수정 : 타임델타 첫시작 값 높음 현상 때문에 갱신 될때까지 버티기 위해 older부터 시작하도록 수정
	m_NoteGrowState = NOTEGROWSTATE::NOTEOLDER;

	m_fAlertTime  = 0.0f;
	m_bEndPhase = false;

	m_Color.Init(1.0f,1.0f,1.0f,1.0f);			//추가 : 8-24
	m_EffectColor.Init(1.0f,1.0f,1.0f,1.0f);			//추가 : 8-24
	m_fAnimationTime = 0.0f;	//- + 0 으로 상태조절을 한다. 추가 : 8-29
	m_fAnimationPercent = 0.0f;	// 추가 : 8-29

	m_EffectColor.A = 0.0f; // 추가 : 8-29

	m_fEffectScale = 1.0f;
	m_fEffectAngle = 0.0f;

	m_TargetOnSwitch = false;

}


/*
 * pbNote소멸자
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

// 변경 : 8-24
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

	//사용 종료 코드
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
 * pbSwipeNote 생성자
 */
pbSwipeNote::pbSwipeNote(){
	m_Types = NOTETYPE::SWIPE;

}

/*
 * SwipeNote 소멸자
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
			//x,y에 관해서 영역판단을 한다.
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
					m_NoteJudgement = NOTEJUDGEMENT::NOTJUDGE;	//실패시 초기화
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
 * Tab노트의 타이머에 의한 상태 변화 함수
 * 해당 TabNote가 상태가 변할 타이밍에 호출이 된다.
 * TabNote의 경우 None, Up, Complete, Down 으로 상태가 변화 되며 이 상태 변화에 따라 판정이 틀려지게 된다.
 *
 * 현재의 노트의 상태에서 타이머에 의해 CallBack이 되었을 경우 다음 상태로 변화를 시킨다.
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
	/////바디
		glPushMatrix();

		BindColor();
		pbDataStorage::BindVertexAndTexture(m_BodyVertexIndex, m_BodyUVIndex);
		glTranslatef(m_PositionX, m_PositionY, 0.f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glPopMatrix();

	/////이펙트
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

	if( m_NoteJudgement == NOTEJUDGEMENT::MISS) {	//미스 체크
		//사용 종료 코드
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
		//------------------------애니메이션 루프-----------------------------//
		m_fAnimationTime += fTime;

		if( m_fAnimationTime > 0.04f)
		{
			m_fAnimationTime = 0.0f;
			m_BodyUVIndex++;
			if(m_BodyUVIndex == m_AniUVEndIndex)
			{
				m_BodyUVIndex = m_AniUVStartIndex;

				m_bEndPhase = false;

				//사용 종료 코드
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
 * pbDoubleTabNote 생성자
 */
pbDoubleTapNote::pbDoubleTapNote(){
	m_Types = NOTETYPE::DTAP;

}

/*
 * DoubleNote 소멸자
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
			//x,y에 관해서 영역판단을 한다.
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
						if( m_bNotifyPause) //업데이트의 체크와 터치 체크 동시에 되는 현상 막기
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
					m_NoteJudgement = NOTEJUDGEMENT::NOTJUDGE;	//실패시 초기화
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
 * Tab노트의 타이머에 의한 상태 변화 함수
 * 해당 TabNote가 상태가 변할 타이밍에 호출이 된다.
 * TabNote의 경우 None, Up, Complete, Down 으로 상태가 변화 되며 이 상태 변화에 따라 판정이 틀려지게 된다.
 *
 * 현재의 노트의 상태에서 타이머에 의해 CallBack이 되었을 경우 다음 상태로 변화를 시킨다.
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
	/////2번째 노트
		glPushMatrix();
		glColor4f(1.0f, 1.0f, 1.0f, m_fSecondNoteAlpha);
		pbDataStorage::BindVertexAndTexture(m_BodyVertexIndex, m_BodyUVIndex);
		glTranslatef(m_vSecondNoteCurrentPos[0], m_vSecondNoteCurrentPos[1], 0.f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glPopMatrix();

	/////바디
		if( !m_bEndPhase) {
			glPushMatrix();

			BindColor();
			pbDataStorage::BindVertexAndTexture(m_BodyVertexIndex, m_BodyUVIndex);
			glTranslatef(m_PositionX, m_PositionY, 0.f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			glPopMatrix();

			/////이펙트
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

	if( m_NoteJudgement == NOTEJUDGEMENT::MISS) {	//미스 체크
		//사용 종료 코드
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
			/////----------------------판정 애니메이션 -----------------------///
			if( m_fAnimationTime != 0.0f )
			{
				m_fAnimationPercent += fTime*(1/m_fAnimationTime);
				float m_fChangeValue = 0.0f;
				if( m_fAnimationTime > 0.0f ) { // 상승시
					m_fChangeValue = 0.5f + m_fAnimationPercent*0.5f;
					m_EffectColor.A  = m_fAnimationPercent;
				}
				else if( m_fAnimationTime < 0.0f ) 	{  //하강시 ColorChangeLerp 는 마이너스 값
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
			//---------------판정 시간 제한-----------------//
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

			//터치 영역 옮겨놓기
			getTouchArea()->movePositon(m_vSecondNoteCurrentPos[0], m_vSecondNoteCurrentPos[1]);
			return;	//update함수 마지막에 처리하는 setnotePosition(x,y)가 터치영역을 움직여 초기화 시키므로 통과해야한다
		}
	}
	else if( m_bEndPhase )
	{
		m_vSecondNoteCurrentPos[0] += pbGlobalInGameVariable::fWorldMoveX;

		//------------------------애니메이션 루프-----------------------------//
		m_fAnimationTime += fTime;

		if( m_fAnimationTime > 0.04f)
		{
			m_fAnimationTime = 0.0f;
			m_BodyUVIndex++;
			if(m_BodyUVIndex == m_AniUVEndIndex)
			{
				m_BodyUVIndex = m_AniUVStartIndex;

				m_bEndPhase = false;

				//사용 종료 코드
				pbRenderProcess::RemoveRenderNote(this);
				pbNoteProcessor::GetInstance()->RemoveNoteAndReturningMemory(this);
				return;
			}
		}
		//=======================================//

		return;	//두번째 노트의 이동 초기화 방지
	}// end if Check EndPhase


	setNotePosition(x,y);
}

//---------- pbLongPressNote------------
/*
 * LongPress Note 생성자
 */
pbLongPressNote::pbLongPressNote(){
	m_Types = NOTETYPE::LPRESS;
}

/*
 * pbLongPressNote 소멸자
 */
pbLongPressNote::~pbLongPressNote(){
}

void pbLongPressNote::ExtraInitialize()
{
	m_fActionPointDistance = 0.0f;			//앤드페이즈가 시작되는 지점
	m_fActionPointPosY = 0.0f;
	m_fPreVisionPosX = 0.0f;					//누르고있어야할 곳을 표시하는 위치
	m_fDestinationPosY = 0.0f;			//최종적으로 가게되는 곳

	m_fPressPercent = 0.0f;
	m_fPressTime = 0.0f;

	m_bNotePressed = false;

	m_iPhase = 0;

	m_fRotate = 0.0f;
}

void pbLongPressNote::SetDestination(GLfloat DistanceFromPosX, GLfloat DistanceFromPosY){
	m_fPressTime = (DistanceFromPosX - pbDataStorage::GetVertexWidth(m_BodyVertexIndex) )/pbGlobalInGameVariable::fWorldMoveSpeed;	// 누르는 시간은 좀더 작다
	m_fPreVisionPosX = m_PositionX + (DistanceFromPosX - pbDataStorage::GetVertexWidth(m_BodyVertexIndex) );
	m_fActionPointDistance =  DistanceFromPosX - ( pbDataStorage::GetVertexWidth(m_BodyVertexIndex)/2 );	//떨어지기 시작할 위치는 너비의 반
	m_fActionPointPosY = m_PositionY;
	m_fDestinationPosY = m_PositionY + DistanceFromPosY;
}

void pbLongPressNote::notify(int x, int y, TOUCHSTATUS::TYPE Touchstatus) {
	switch (Touchstatus) {
		case TOUCHSTATUS::TAPDOWN: {
			//x,y에 관해서 영역판단을 한다.
			if (projectBean::pbNote::CheckingTouchOn(x, y)) {
				if( m_TargetOnSwitch)
				{
					m_bNotePressed = true;
					npAudioSystem::playEffect(0);

					pbUserData::SingleObject->UsingAbilityPoint(30);
				}
				else {
//					pbEffectProcess::pEffectProcessor->AddEffect(EFFECTTYPE::JUDGE, m_PositionX, m_PositionY+ JUDGE_EFFECT_H, 1.0f, NOTEJUDGEMENT::FAIL);
					m_NoteJudgement = NOTEJUDGEMENT::NOTJUDGE;	//실패시 초기화
					pbUserData::SingleObject->UsingAbilityPoint(120);
					pbComboManager::GetInstance()->ResetCombo();

					pbTouchLayer::removeObserver(this);
					nitroFrame::npTimer::findRemove(this);
				}
			}
			break;
		case TOUCHSTATUS::TAPUP:
			{
				if (m_bNotePressed) {	//눌러진 상태여야 탭업 판정에 들어간다
					m_bNotePressed = false;

					if (m_fPressPercent < 1.0f) {
//						m_NoteJudgement = NOTEJUDGEMENT::FAIL;
//						pbEffectProcess::pEffectProcessor->AddEffect(	EFFECTTYPE::JUDGE, m_PositionX, m_PositionY+ JUDGE_EFFECT_H,	 1.0f, m_NoteJudgement);
						m_NoteJudgement = NOTEJUDGEMENT::NOTJUDGE; //실패시 초기화

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
	/////바디
		glPushMatrix();

		BindColor();
		pbDataStorage::BindVertexAndTexture(m_BodyVertexIndex, m_BodyUVIndex);
		glTranslatef(m_PositionX, m_PositionY, 0.f);
		glRotatef(m_fRotate*360.0f, 0.0f, 0.0f, 1.0f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glPopMatrix();
	/////목표
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

	if( m_NoteJudgement == NOTEJUDGEMENT::MISS) {	//미스 체크
		//사용 종료 코드
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
		BodyPosX -= pbGlobalInGameVariable::fWorldMoveX;//가속하기 위해 반대로 더 굴려준다
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
 * breif: TargetingNote가 TouchLayer에서 callback 되어지는 함수
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
					m_NoteJudgement = NOTEJUDGEMENT::NOTJUDGE;	//실패시 초기화
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
	/////바디
		glPushMatrix();

		BindColor();
		pbDataStorage::BindVertexAndTexture(m_BodyVertexIndex, m_BodyUVIndex);
		glTranslatef(m_PositionX, m_PositionY, 0.f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glPopMatrix();

	/////이펙트
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
		/////----------------------마무리 애니메이션 -----------------------///
		//------------------------애니메이션 루프-----------------------------//
		m_fAnimationTime += fTime;

		if(m_fAnimationTime > 0.04f)
		{
			m_fAnimationTime = 0.0f;
			m_BodyUVIndex++;
			if(m_BodyUVIndex ==m_AniUVEndIndex)
			{
				m_BodyUVIndex =m_AniUVStartIndex;

				m_bEndPhase = false;

				//사용 종료 코드
				pbRenderProcess::RemoveRenderNote(this);
				pbNoteProcessor::GetInstance()->RemoveNoteAndReturningMemory(this);
				return;
			}
		}
	}

	setNotePosition(x,y);
}


}
