/*
 * pbNotePattern.h
 *
 *  Created on: 2013. 1. 2.
 *      Author: NitroSoft
 */

#ifndef PBNOTEPATTERN_H_
#define PBNOTEPATTERN_H_

class pbNotePattern {
public:
	pbNotePattern() : m_NoteType(-1), m_fGenDistance(0.0f), m_fPosY(0.0f) {};
	pbNotePattern(int Type, float GenDistance, float PosY) {
		Init(Type, GenDistance, PosY);
	}

	void Init(int Type, float GenDistance, float PosY) {
		m_NoteType  = Type; m_fGenDistance = GenDistance;  m_fPosY = PosY;
	}

	int m_NoteType;
	float m_fGenDistance;
	float m_fPosY;
};


#endif /* PBNOTEPATTERN_H_ */
