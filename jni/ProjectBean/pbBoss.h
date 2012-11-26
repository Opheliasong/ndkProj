#ifndef PBBOSS_H_
#define PBBOSS_H_

#include "stdafx.h"

//----------------------------------------------������Ʈ ���� ------------------------------------------------------------------------------------//
// ������Ʈ ���� �۵� ��
// ������ġ���� ������ ������ �̵� ������ ����ð����� ����Ͽ� ��ġ�� ��ȯ�Ѵ�.
class pbObjectMover {
protected:
	pbObjectMover():m_ID(-1) { m_vDir[0] = 0;  m_vDir[1] = 0; m_vPos[0] = 0;  m_vPos[1] = 0; LOGE("CREATE MOVER");}
public:
	pbObjectMover(int ID, float dirX, float dirY){ m_ID = ID; m_vDir[0] = dirX; m_vDir[1] = dirY; m_vPos[0] = 0;  m_vPos[1] = 0;}
	virtual ~pbObjectMover() {};

	virtual inline void SetID(int ID) { m_ID = ID;}
	virtual inline int GetID() { return m_ID;}

	virtual inline void SetDir(float dirX, float dirY) { m_vDir[0] = dirX; m_vDir[1] = dirY;}
	virtual inline void SetStartPos(float PosX, float PosY) { m_vStartPos[0] = PosX; m_vStartPos[1] = PosY;}
/*	virtual inline float GetDirX() { return m_vDir[0];}
	virtual inline float GetDirY() { return m_vDir[1];}*/

	virtual float* GetV2PosByTime(float fAccumulateTime) = 0;

protected:
	int m_ID;
	npV2Vector m_vDir;
	npV2Vector m_vPos;
	npV2Vector m_vStartPos;
};



class pbLineMover : public pbObjectMover {
private:
	pbLineMover() { }
public:
	pbLineMover(int ID, float dirX, float dirY){ SetID(ID); SetDir(dirX, dirY); }
	virtual ~pbLineMover() {};

	virtual float* GetV2PosByTime(float fAccumulateTime);
private:
};

class pbZigZagMover : public pbObjectMover {
private:
	pbZigZagMover() { m_fCycle = 0.0f;	m_fAmplitudeX = 0.0f;	m_fAmplitudeY = 0.0f;}
public:
	pbZigZagMover(int ID, float dirX, float dirY, float fAmplitude, float fCycle){
		SetID(ID); SetDir(dirX, dirY);

		// �븻 ���͸� ������ �־��ش�. ������⿡ ���� ���� ����
		float vectorsize = sqrt(dirX*dirX + dirY*dirY);

		m_fAmplitudeX = dirY / vectorsize;
		m_fAmplitudeX *= fAmplitude;

		m_fAmplitudeY = -dirX / vectorsize;
		m_fAmplitudeY *= fAmplitude;


		m_fCycle = fCycle;
	}
	virtual ~pbZigZagMover() {};

	virtual float* GetV2PosByTime(float fAccumulateTime);
private:
	float m_fCycle;

	float m_fAmplitudeX;
	float m_fAmplitudeY;
};




//----------------------------------------------��������Ʈ ------------------------------------------------------------------------------------//
class pbMarionette{
public:
	pbMarionette();
	~pbMarionette();

	void AddLineMoveState(int ID, float dirX, float dirY);
	void AddZigZagMoveState(int ID, float dirX, float dirY, float fAmplitude, float fCycle);

	void DeleteMoveState(int ID);

	void SelectMoveState(int ID);

	void MoveUpdate(float fTime);

	inline int GetState(){ return m_CurrentState;}

	void Translate();

	inline void SetMovePause(bool bPause) { m_bUpdatePause = bPause;}

	void ClearDataStore();

	inline float* GetV2Pos() { return m_vPos;}
	inline void SetPosX(float PosX) {m_vPos[0] =PosX;}
	inline void SetPosY(float PosY) {m_vPos[1] =PosY;}

private:
	npV2Vector m_vPos;

	typedef  npLinkNode<pbObjectMover*> pbObjectMoverList;
	pbObjectMoverList*  m_pMoverHead;
	pbObjectMover* m_pCurrentMover;

	int m_CurrentState;
	float m_fAccumulateTime;

	bool m_bUpdatePause;
};

//----------------------------------------------pbBoss------------------------------------------------------------------------------------//

class pbBoss : public npDrawable{
private:
	pbBoss();
	~pbBoss();

public:
	static void Create();
	void LoadData();

	void Update(float fTime);

	virtual void PreSettingDraw();
	virtual void DrawThis();

	static void DecreaseHP(float fDamage);

	void ClearDataStore();
	static void Release();

	inline void SetState(int State) { m_pMarionette->SelectMoveState(State); }
	inline bool IsBattlePhase() { return m_bBattlePhase;}

	inline static pbBoss* GetInstance() { return SingleObject; }
	inline static pbMarionette* GetMarionette() { return GetInstance()->m_pMarionette;}

	enum {NONE = 0, APPROACHING, WEAVING_UP, WEAVING_DOWN, WALKOUT, DIE};
private:
	static pbBoss* SingleObject;
	pbMarionette* m_pMarionette;
	float m_fHP;

	pbBasicDrawUnit* m_pBodyDrawUnit;

	bool m_bBattlePhase;
	bool m_bBossAlive;

};


#endif
