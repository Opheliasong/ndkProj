/*
 * pbPatternGenerator.cpp
 *
 *  Created on: 2013. 1. 2.
 *      Author: NitroSoft
 */

#include "pbNotePatternGenerator.h"

pbNotePatternGenerator::pbNotePatternGenerator() {
	m_NotePatternGroupVector.reserve(5);

	m_fMoveDistance = 0.0f;
	m_pCurrentPattern = NULL;

	m_iTotalPatternGroup = 0;
	m_PatternGroupIndex = 0;
}
pbNotePatternGenerator::~pbNotePatternGenerator() {
	NotePatternGroupVector::iterator iter;
	for(iter = m_NotePatternGroupVector.begin(); iter != m_NotePatternGroupVector.end(); iter++) {
		delete (*iter);
	}

	m_NotePatternGroupVector.clear();
}

void pbNotePatternGenerator::AddPatternGroup(pbNotePatternGroup* pPatternGroup) {
	m_NotePatternGroupVector.push_back(pPatternGroup);
	m_iTotalPatternGroup++;

	LOGE("pbNotePatternGenerator AddPatternGroup");
}

void pbNotePatternGenerator::Update(float fTime) {
	if(m_iTotalPatternGroup > 0) {
		m_fMoveDistance += pbStageValue::GetStageMoveSpeed() * fTime;

		pbNotePattern* pNotePattern = m_NotePatternGroupVector[m_PatternGroupIndex]->GetCurrentNotePattern(m_fMoveDistance);
		if( pNotePattern != NULL  ) { //생성할 노트 패턴이 있을때
			//노트 패턴 등록
			m_pCurrentPattern = pNotePattern;
		}

		if(m_NotePatternGroupVector[m_PatternGroupIndex]->CheckConsumeAllPattern()) {	//모두 다 소모 했는지 체크
			//패턴 그룹 초기화
			m_NotePatternGroupVector[m_PatternGroupIndex]->ResetIndex();
			//변수 초기화
			ResetMoveDistance();

			 //다음 패턴 그룹 선택
			srand(time(NULL));
			int index = rand() % m_iTotalPatternGroup;
			m_PatternGroupIndex = index;
		}

	}//end if TotalPatternGroup
}

