/*
 * pbNoteProcesser.h
 *
 *  Created on: 2012. 7. 16.
 *      Author: NitroPigSoft02
 */

#ifndef PBNOTEPROCESSER_H_
#define PBNOTEPROCESSER_H_

#include "../stdafx.h"
#include "../pbGlobalVariable.h"
 #include "../pbMainFrame.h"
#include "../pbCharacter.h"

#include "pbNoteElement.h"
#include "pbTargetingNotes.h"
#include "pbTabNotes.h"
#include "pbDTabNotes.h"
#include "pbLongPressNotes.h"
#include "pbNinjaNotes.h"

class pbNoteElement;
//class pbTabNotes;
//class pbDTabNotes;
//class pbLongPressNotes;
//class pbNinjaNotes;

//--------------------------------Note Droper------------------------------------------------------------------------------------//
class pbNoteDropper : public npTimerObserver{
public:
	pbNoteDropper();
	~pbNoteDropper();

	void NoteDropCheck();
	void SetGenerateNote(bool On);

	void onTimeAlerts();		//Timer에 의해서 Event Signal이 발생하였을 경우 작동될 로직

	void RemoveNoteAndReturningMemory(pbNoteElement* pNote);
	void RemoveNoteAndReturningMemory(pbTabNotes* pNote);
	void RemoveNoteAndReturningMemory(pbDTabNotes* pNote);
	void RemoveNoteAndReturningMemory(pbLongPressNotes* pNote);
	void RemoveNoteAndReturningMemory(pbNinjaNotes* pNote);

private:
	float m_fGenerateTime;
	bool m_bGenerateState;

	pbMemoryRentalUnit<pbTabNotes>* m_pTargetNoteRentalUnit;
	pbMemoryRentalUnit<pbDTabNotes>* m_pDoubleTapNoteRentalUnit;
	pbMemoryRentalUnit<pbLongPressNotes>* m_pLongNoteRentalUnit;
	pbMemoryRentalUnit<pbNinjaNotes>* m_pNinjaNoteRentalunit;
};

//======================================================================================//



//--------------------------------Note Process------------------------------------------------------------------------------------//
class pbNoteProcessor {
private:
	pbNoteProcessor();
	~pbNoteProcessor();
//	void registControled(pbNote* pNote);
//	void removeControled(pbNote* pNote);

public:
	static void Create();

	//AddNotes 함수군.
	void AddNotes(pbTabNotes* TabNote)	;
	void AddNotes(pbDTabNotes* DTabNote);
	void AddNotes(pbLongPressNotes* LongPressNote);
	void AddNotes(pbNinjaNotes* NinjaNote);

	//ReleaseNote 함수군
	//Delete Stack에 올려두는 Interface
	void ReleaseNote(pbTabNotes* TabNote);
	void ReleaseNote(pbDTabNotes* DTabNote);
	void ReleaseNote(pbLongPressNotes* LongPressNote);
	void ReleaseNote(pbNinjaNotes* NinjaNote);

	void Update(float time);

	void ClearDataStore();
	void RenderingAll();
	static void Release();

	//void AddTargetingNote(float X, float Y);
	//void AddSwipeNote(float X, float Y, int numChain , float fChainVectorX, float fChainVectorY);
	//void AddDoubTapNote(float X, float Y, float fSecondDistanceX, float fSecondDistanceY);
	//void AddLongNote(float X, float Y, float fSecondDistanceX, float fSecondDistanceY);
	//void CreateDriedNoteByType(TiXmlNode* DriedDataNode);
	//void CreateNote(pbNoteElement* pDriedNote, float PosX, float PosY);

	static pbNoteProcessor* GetInstance() { return SingleObject; }
	static pbNoteDropper* GetNoteDropper() { return SingleObject->m_NoteDropper; }

	void RequestRemoveNote(pbNoteElement* pNote);
private:
	void CheckMissNote();
	void CheckTargetingAbleOrUnalbe();

	static pbNoteProcessor* SingleObject;
	int m_NoteCounts;
	pbNoteDropper* m_NoteDropper;
	npLinkNode<pbNoteElement*>* m_ControledNoteStore;
	npLinkNode<pbNoteElement*>* m_deleteStacks;
	//pbMemoryRentalUnit<pbNoteElement>* m_NoteElementsUnits;
};

//class pbDriedNote{
//public:
//	pbDriedNote():NoteType(NOTETYPE::NONETYPE), ID(-1), fPosX(0), fPosY(0){};
//	virtual ~pbDriedNote(){};
//
//	virtual void SetData(NOTETYPE::TYPE type, int id, float posX, float posY)
//	{
//		NoteType = type; ID = id; fPosX = posX; fPosY = posY;
//	}
//
//public:
//	NOTETYPE::TYPE NoteType;
//	int ID;
//	float fPosX;
//	float fPosY;
//};
//
//class pbDriedNote_Chain : public pbDriedNote{
//public:
//	pbDriedNote_Chain() : numChain(0), fChainVectorX(0), fChainVectorY(0){};
//	virtual ~pbDriedNote_Chain(){};
//
//	void SetTotalChainCount(int count) { numChain = count;}
//	void SetNextChainVector(float posX, float posY) { fChainVectorX = posX; fChainVectorY = posY;}
//public:
//	int numChain;
//	float fChainVectorX;
//	float fChainVectorY;
//
//};
//
//class pbDriedNote_DoublePos : public pbDriedNote{
//public:
//	pbDriedNote_DoublePos() : fSecondDistanceX(0), fSecondDistanceY(0){};
//	virtual ~pbDriedNote_DoublePos(){};
//
//	void SecondPos(float posX, float posY) { fSecondDistanceX = posX; fSecondDistanceY = posY;}
//public:
//	float fSecondDistanceX;	//Body��ġ�� ���������� ������� �Ÿ�
//	float fSecondDistanceY; //Body��ġ�� ���������� ������� �Ÿ�
//};
//
//class pbDriedNoteArray{
//public:
//	pbDriedNoteArray(){ m_ppDriedNote = NULL; m_ArraySize = 0; m_AddCount = 0; m_CallCount = 0;}
//	~pbDriedNoteArray(){};
//
//	void CreateArray(int size);
//	void AddDriedNote(pbDriedNote* pDriedNote);
//
//	pbDriedNote* GetDriedNote_ByCallCount(){ return m_ppDriedNote[m_CallCount]; }
//	void IncreaseCallCount() { m_CallCount++;}
//
//	bool IsExistUsableDriedNote() { return m_CallCount < m_AddCount;}
//
//	void ReleaseArray();
//
//private:
//	int m_ArraySize;
//	int m_AddCount;
//	int m_CallCount;
//	pbDriedNote** m_ppDriedNote;
//};

//}

#endif /* PBNOTEPROCESSER_H_ */
