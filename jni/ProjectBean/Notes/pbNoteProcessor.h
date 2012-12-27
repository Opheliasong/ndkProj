/*
 * pbNoteProcesser.h
 *
 *  Created on: 2012. 7. 16.
 *      Author: NitroPigSoft02
 */

#ifndef PBNOTEPROCESSER_H_
#define PBNOTEPROCESSER_H_

#include "../stdafx.h"

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
class pbNoteProcessor : public npDrawable{
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

	void DeleteStackArrangement();
	void ClearDataStore();
	//void RenderingAll();
	virtual void PreSettingDraw();
	virtual void DrawThis();
	static void Release();

	static pbNoteProcessor* GetInstance() { return SingleObject; }
	static pbNoteDropper* GetNoteDropper() { return SingleObject->m_NoteDropper; }

	void RequestRemoveNote(pbNoteElement* pNote);
private:
	void CheckMissNote();
	void CheckTargetingAbleOrUnalbe();

	static pbNoteProcessor* SingleObject;
	int m_NoteCounts;
	pbNoteDropper* m_NoteDropper;
	npLinkNode<pbNoteElement*>* m_ControlledNoteStore;
	npLinkNode<pbNoteElement*>* m_deleteStacks;
};

#endif /* PBNOTEPROCESSER_H_ */
