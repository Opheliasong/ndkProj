/*
 * pbNoteProcesser.h
 *
 *  Created on: 2012. 7. 16.
 *      Author: NitroPigSoft02
 */

#ifndef PBNOTEPROCESSER_H_
#define PBNOTEPROCESSER_H_

#include "stdafx.h"

namespace projectBean {
//--------------------------------�Ľ� �ؿ��� �淮ȭ�� ������ Ŭ����------------------------------------------------------------------------------------//
class pbDriedNote{
public:
	pbDriedNote():NoteType(NOTETYPE::NONETYPE), ID(-1), fPosX(0), fPosY(0){};
	virtual ~pbDriedNote(){};

	virtual void SetData(NOTETYPE::TYPE type, int id, float posX, float posY)
	{
		NoteType = type; ID = id; fPosX = posX; fPosY = posY;
	}

public:
	NOTETYPE::TYPE NoteType;
	int ID;
	float fPosX;
	float fPosY;
};

class pbDriedNote_Chain : public pbDriedNote{
public:
	pbDriedNote_Chain() : numChain(0), fChainVectorX(0), fChainVectorY(0){};
	virtual ~pbDriedNote_Chain(){};

	void SetTotalChainCount(int count) { numChain = count;}
	void SetNextChainVector(float posX, float posY) { fChainVectorX = posX; fChainVectorY = posY;}
public:
	int numChain;
	float fChainVectorX;
	float fChainVectorY;

};

class pbDriedNote_DoublePos : public pbDriedNote{
public:
	pbDriedNote_DoublePos() : fSecondDistanceX(0), fSecondDistanceY(0){};
	virtual ~pbDriedNote_DoublePos(){};

	void SecondPos(float posX, float posY) { fSecondDistanceX = posX; fSecondDistanceY = posY;}
public:
	float fSecondDistanceX;	//Body��ġ�� ���������� ������� �Ÿ�
	float fSecondDistanceY; //Body��ġ�� ���������� ������� �Ÿ�
};

class pbDriedNoteArray{
public:
	pbDriedNoteArray(){ m_ppDriedNote = NULL; m_ArraySize = 0; m_AddCount = 0; m_CallCount = 0;}
	~pbDriedNoteArray(){};

	void CreateArray(int size);
	void AddDriedNote(pbDriedNote* pDriedNote);

	pbDriedNote* GetDriedNote_ByCallCount(){ return m_ppDriedNote[m_CallCount]; }
	void IncreaseCallCount() { m_CallCount++;}

	bool IsExistUsableDriedNote() { return m_CallCount < m_AddCount;}

	void ReleaseArray();

private:
	int m_ArraySize;
	int m_AddCount;
	int m_CallCount;
	pbDriedNote** m_ppDriedNote;
};
//======================================================================================//

//--------------------------------Note Droper------------------------------------------------------------------------------------//
class pbNoteDropper : public npObserver{
public:
	pbNoteDropper();
	~pbNoteDropper();

	bool DriedDataParsing(const char* fileName);

	void CreateDriedNote_Targeting(float PosX, float PosY);
	void CreateDriedNote_Swipe(float PosX, float PosY, int numChain, float fChainVectorX, float fChainVectorY);
	void CreateDriedNote_Long(float PosX, float PosY, float fSecondDistanceX, float fSecondDistanceY);
	void CreateDriedNote_DoubleTap(float PosX, float PosY, float fSecondDistanceX, float fSecondDistanceY);

	void CreateDriedNoteByType(TiXmlNode* DriedDataNode);
	void NoteDropCheck();
	void SetGenerateNote(bool On);

	virtual void notify(){};
	virtual void notify(int x, int y, TOUCHSTATUS::TYPE Touchstatus){};
	virtual void notify(TOUCHSTATUS::TYPE Touchstatus){};
	virtual void wakeUpNotify();
	virtual float resetWakeTime();

private:
	pbDriedNoteArray* m_pDriedNoteArray;

	float m_fGenerateTime;
	bool m_bGenerateMode;
};

//======================================================================================//



//--------------------------------Note Process------------------------------------------------------------------------------------//
class pbNoteProcessor {
private:
	pbNoteProcessor();
	~pbNoteProcessor();

	void registControled(pbNote* pNote);
	void removeControled(pbNote* pNote);

public:
	static void Create();
	bool LoadData(const char* fileName);
//	bool DriedDataParsing(const char* fileName);

	void AddTargetingNote(float X, float Y);
	void AddSwipeNote(float X, float Y, int numChain , float fChainVectorX, float fChainVectorY);
	void AddDoubTapNote(float X, float Y, float fSecondDistanceX, float fSecondDistanceY);
	void AddLongNote(float X, float Y, float fSecondDistanceX, float fSecondDistanceY);

	void CheckMissNote();
	void CreateDriedNoteByType(TiXmlNode* DriedDataNode);
	void CreateNote(pbDriedNote* pDriedNote, float PosX, float PosY);
//	void NoteProductionCheck();

	void Update(float time);

	void ClearDataStore();
	static void Release();

	void RemoveNoteAndReturningMemory(pbTargetingNote* pNote);
	void RemoveNoteAndReturningMemory(pbSwipeNote* pNote);
	void RemoveNoteAndReturningMemory(pbDoubleTapNote* pNote);
	void RemoveNoteAndReturningMemory(pbLongPressNote* pNote);

	static pbNoteProcessor* GetInstance() { return SingleObject; }
	static pbNoteDropper* GetNoteDropper() { return SingleObject->m_NoteDropper; }
private:
	static pbNoteProcessor* SingleObject;
	npLinkNode<pbNote>* m_ControledNoteStore;

	int m_NoteCounts;

	pbNoteDropper* m_NoteDropper;

	pbMemoryRentalUnit<pbSwipeNote>* m_pSwipeNoteRentalUnit;
	pbMemoryRentalUnit<pbDoubleTapNote>* m_pDoubleTapNoteRentalUnit;
	pbMemoryRentalUnit<pbLongPressNote>* m_pLongNoteRentalUnit;
	pbMemoryRentalUnit<pbTargetingNote>* m_pTargetNoteRentalUnit;
};
}

#endif /* PBNOTEPROCESSER_H_ */
