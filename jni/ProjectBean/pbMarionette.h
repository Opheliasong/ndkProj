/*
 * pbMarionette.h
 *
 *  Created on: 2012. 11. 28.
 *      Author: NitroSoft
 */

#ifndef PBMARIONETTE_H_
#define PBMARIONETTE_H_

#include "stdafx.h"

//----------------------------------------------������Ʈ ���� ------------------------------------------------------------------------------------//
// ������Ʈ ���� �۵� ��
// ������ġ���� ������ ������ �̵� ������ ����ð����� ����Ͽ� ��ġ�� ��ȯ�Ѵ�.
class pbObjectMover {
protected:
	pbObjectMover():m_ID(-1) { m_vDir[0] = 0;  m_vDir[1] = 0; m_vPos[0] = 0;  m_vPos[1] = 0; LOGE("CREATE MOVER"); m_fpActionFunc = NULL;  }
public:
	pbObjectMover(int ID, float dirX, float dirY, bool(ActionFunc)(float*)){ m_ID = ID; m_vDir[0] = dirX; m_vDir[1] = dirY; m_vPos[0] = 0;  m_vPos[1] = 0; m_fpActionFunc = ActionFunc; }
	virtual ~pbObjectMover() {};

	virtual inline void SetID(int ID) { m_ID = ID;}
	virtual inline int GetID() { return m_ID;}

	virtual inline void SetDir(float dirX, float dirY) { m_vDir[0] = dirX; m_vDir[1] = dirY;}
	virtual inline void SetStartPos(float PosX, float PosY) { m_vStartPos[0] = PosX; m_vStartPos[1] = PosY;}

	virtual inline void SetActionFunc(bool(ActionFunc)(float*)) { m_fpActionFunc = ActionFunc;}
	virtual inline bool CallActionFunc(float* pV2Pos) {
		return (*m_fpActionFunc)(pV2Pos);
	}

	virtual float* GetV2PosByTime(float fAccumulateTime) = 0;

protected:
	int m_ID;
	npV2Vector m_vDir;
	npV2Vector m_vPos;
	npV2Vector m_vStartPos;

	bool (*m_fpActionFunc)(float* pV2Pos);
};



class pbLineMover : public pbObjectMover {
private:
	pbLineMover() { }
public:
	pbLineMover(int ID, float dirX, float dirY, bool(ActionFunc)(float*)){ SetID(ID); SetDir(dirX, dirY); SetActionFunc(ActionFunc);}
	virtual ~pbLineMover() {};

	virtual float* GetV2PosByTime(float fAccumulateTime);
private:
};

class pbZigZagMover : public pbObjectMover {
private:
	pbZigZagMover() { m_fCycle = 0.0f;	m_fAmplitudeX = 0.0f;	m_fAmplitudeY = 0.0f;}
public:
	pbZigZagMover(int ID, float dirX, float dirY, float fAmplitude, float fCycle, bool(ActionFunc)(float*) ){
		SetID(ID); SetDir(dirX, dirY); SetActionFunc(ActionFunc);

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

	void AddLineMoveState(int ID, float dirX, float dirY, bool(ActionFunc)(float*));
	void AddZigZagMoveState(int ID, float dirX, float dirY, float fAmplitude, float fCycle, bool(ActionFunc)(float*));

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

	inline bool GetActionCondition() { return m_bActionComplete; }
private:
	npV2Vector m_vPos;

	typedef  npLinkNode<pbObjectMover*> pbObjectMoverList;
	pbObjectMoverList*  m_pMoverHead;
	pbObjectMover* m_pCurrentMover;

	int m_CurrentState;
	float m_fAccumulateTime;

	bool m_bUpdatePause;
	bool m_bActionComplete;
};


#endif /* PBMARIONETTE_H_ */
