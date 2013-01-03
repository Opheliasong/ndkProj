/*
 * NotePatternGroup.h
 *
 *  Created on: 2013. 1. 2.
 *      Author: NitroSoft
 */

#ifndef NOTEPATTERNGROUP_H_
#define NOTEPATTERNGROUP_H_

#include "../stdafx.h"
#include "pbNotePattern.h"

class pbNotePatternGroup {
public:
	pbNotePatternGroup();
	~pbNotePatternGroup();

	void AddNotePattern(int NoteType, float GenDistance, float PosY);

	inline bool CheckConsumeAllPattern() { return m_iTotalNote == m_iNoteIndex; }

	pbNotePattern* GetCurrentNotePattern(float fMoveDistance);

	inline void ResetIndex() { m_iNoteIndex = 0; }

private:
	typedef std::vector<pbNotePattern> NotePatternVector;
	NotePatternVector m_NotePatternVector;

	int m_iTotalNote;
	int m_iNoteIndex;
};




#endif /* NOTEPATTERNGROUP_H_ */
