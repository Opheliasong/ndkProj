/*
 * NotePatternGroup.cpp
 *
 *  Created on: 2013. 1. 2.
 *      Author: NitroSoft
 */

#include "pbNotePatternGroup.h"

pbNotePatternGroup::pbNotePatternGroup() {
	m_iTotalNote = 0;
	m_iNoteIndex = 0;

	m_NotePatternVector.reserve(4);
}

pbNotePatternGroup::~pbNotePatternGroup() {
	m_NotePatternVector.clear();
}

void pbNotePatternGroup::AddNotePattern(int NoteType, float GenDistance, float PosY) {
	m_NotePatternVector.push_back(NotePatternVector::value_type(NoteType, GenDistance, PosY));
	m_iTotalNote++;

	LOGE("Add Note Pattern");
}

pbNotePattern* pbNotePatternGroup::GetCurrentNotePattern(float fMoveDistance) {
	if( fMoveDistance >= m_NotePatternVector[m_iNoteIndex].m_fGenDistance ) {
		pbNotePattern* pNotePattern = &(m_NotePatternVector[m_iNoteIndex]);
		LOGE("PreIndex : %d", m_iNoteIndex);
		m_iNoteIndex++;
		LOGE("PostIndex : %d", m_iNoteIndex);
		return pNotePattern;
		//return &(m_NotePatternVector[m_iNoteIndex++]);
	}
	return NULL;
}

