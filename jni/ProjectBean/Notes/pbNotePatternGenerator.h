/*
 * pbPatternGenerator.h
 *
 *  Created on: 2013. 1. 2.
 *      Author: NitroSoft
 */

#ifndef PBPATTERNGENERATOR_H_
#define PBPATTERNGENERATOR_H_

#include "pbNotePatternGroup.h"

class pbNotePatternGenerator {
public:
	pbNotePatternGenerator();
	~pbNotePatternGenerator();

	void AddPatternGroup(pbNotePatternGroup* pPatternGroup);

	inline pbNotePattern* GetCurrentPattern() { return m_pCurrentPattern; }

	inline void ResetCurrentPattern(){ m_pCurrentPattern = NULL; }
	inline void ResetMoveDistance() { m_fMoveDistance = 0.0f; }

	void Update(float fTime);

private:
	typedef std::vector<pbNotePatternGroup*> NotePatternGroupVector;
	NotePatternGroupVector m_NotePatternGroupVector;

	int m_iTotalPatternGroup;
	int m_PatternGroupIndex;

	float m_fMoveDistance;

	pbNotePattern* m_pCurrentPattern;
};



#endif /* PBPATTERNGENERATOR_H_ */
