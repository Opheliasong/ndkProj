#ifndef _GLOBALVARIABLE_H
#define _GLOBALVARIABLE_H

#include "stdafx.h"


class pbGlobalInGameVariable{
public:
	pbGlobalInGameVariable();
	~pbGlobalInGameVariable();

	static void ResetGlobalVariable();
public:
	static GLfloat fWorldMoveX;
	static GLfloat fWorldX;
	static GLfloat fWorldMoveSpeed;
	static Direction::HEADING fWorldMoveDir;
	static bool bGamePause;
	static float fColorKey[3];
	static GLuint NumLife;
	static int PlusScore;
/*	static bool bCameraShake;
	static float fCameraShakeX;
	static float fCameraShakeY;
	static float fCameraShakeTime;*/
};

class pbUserData{
private:
	pbUserData();
	~pbUserData();

public:
	static pbUserData& GetInstance();

	void AbilityPointInit();

	void UsingAbilityPoint(int Point);
	void ResetUsedAbilityPoint(){ m_UsedAbilityPoint = 0;}

	int GetUsedAbilityPoint() { return m_UsedAbilityPoint;}
	int GetCurrentAbilityPoint() { return m_CurrentAbilityPoint;}
	int GetMaxAbilityPoint() { return m_MaxAbilityPoint;}

private:
	int m_MaxAbilityPoint;
	int m_CurrentAbilityPoint;
	int m_UsedAbilityPoint;

};

#endif
