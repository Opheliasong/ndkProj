#include "pbGlobalVariable.h"
#include "pbMainFrame.h"


//-----------------�ΰ��ӿ��� ���� �� ����------------------------//
void pbGlobalInGameVariable::ResetGlobalVariable() {
	fWorldMoveDir = Direction::BACKWARD;
	fWorldMoveX = 0.0f;//����*���ǵ�*Ÿ��
	fWorldX = 0.0f;
	fWorldMoveSpeed = WORLD_MOVESPEED;
	bGamePause = false;
	//ColorKey[3] = { 0, 50, 50};
	NumLife = 3;
	PlusScore = 0;

	LOGI("pbGlobalInGameVariable::ResetGlobalVariable");
}
Direction::HEADING pbGlobalInGameVariable::fWorldMoveDir = Direction::BACKWARD;
GLfloat pbGlobalInGameVariable::fWorldMoveX = 0.0f;//����*���ǵ�*Ÿ��
GLfloat pbGlobalInGameVariable::fWorldX = 0.0f;
GLfloat pbGlobalInGameVariable::fWorldMoveSpeed = WORLD_MOVESPEED;
bool pbGlobalInGameVariable::bGamePause = false;
//float pbGlobalInGameVariable::fColorKey[3] = { 0, 50, 50};
GLuint pbGlobalInGameVariable::NumLife = 0;
int pbGlobalInGameVariable::PlusScore = 0;
/*bool pbGlobalInGameVariable::bCameraShake = false;
float pbGlobalInGameVariable::fCameraShakeTime = 0.0f;
float pbGlobalInGameVariable::fCameraShakeX = 0.0f;
float pbGlobalInGameVariable::fCameraShakeY = 0.0f;*/

//-----------------���� ������------------------------//
pbUserData::pbUserData():m_MaxAbilityPoint(0), m_CurrentAbilityPoint(0), m_UsedAbilityPoint(0) {
	AbilityPointInit();
}
pbUserData::~pbUserData() {

}

pbUserData& pbUserData::GetInstance(){
	static pbUserData singleton;

	return singleton;
}

void pbUserData::AbilityPointInit()
{
	m_MaxAbilityPoint = MAX_AP;
	m_CurrentAbilityPoint = MAX_AP;
	m_UsedAbilityPoint = 0;
}

void pbUserData::UsingAbilityPoint(int Point)
{
	m_UsedAbilityPoint += Point;
	m_CurrentAbilityPoint -= Point;
}

//===============================//
