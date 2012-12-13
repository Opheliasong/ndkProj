/*
 * pbNoteElement.h
 *
 *  Created on: 2012. 11. 26.
 *      Author: Ophelia
 */

#ifndef PBNOTEELEMENT_H_
#define PBNOTEELEMENT_H_

#include "../stdafx.h"
#include "../../nitroFrame/Render/npRenderTypedef.h"
#include "../../nitroFrame/Render/Sprite.h"
#include "../../nitroFrame/Drawable/npDrawable.h"
#include "../../nitroFrame/TouchLayer/TouchLayer.h"
#include "../../nitroFrame/TouchLayer/TouchObserver.h"
#include "../../nitroFrame/Timer/npTimerObserver.h"
#include "../pbDrawUnit.h"
#include "../pbTypes.h"

class pbNoteElement: public npDrawable, public iTouchObserver, public npTimerObserver {
public:
	//노트의 상태를 나타내는 enum
	//NOTECREATE: 노트가 새로 생성된 상태, Targeting이 될것인지 말 것인지 판단해야 한다.
	//JUDGECOMPLETE: 노트의 Targeting Judge가 끝난 시점.
	//NORMAL: 노트의 기본 상태
	//DEAD: 노트가 유저의 공격(상호작용)에 의해 이벤트가 발생하여 죽은 시점
	typedef enum {NOTECREATE, NORMAL, DEAD, NONE}NOTESTATE;
	typedef enum {TABNOTE,DTABNOTE,LONGPRESS,NINJA}NOTETYPES;
	pbNoteElement();
	virtual ~pbNoteElement();

	virtual void PreSettingDraw()=0;
	virtual void DrawThis()=0;
	virtual void notify()=0;
	virtual void onTimeAlerts()=0;
	virtual void Update(float fTime)=0;
	virtual void ResetNoteState();

	bool setBaseTextureTAG(screenplayTag TAG);
	bool setEffectTextureTAG(screenplayTag TAG);
	bool setDeadEffectSprite(screenplayTag TAG);

	virtual void setNotePosition(float x, float y);
	void setNoteSize(float width, float height);

protected:
	virtual void ReleaseNote();

//	pbBasicDrawUnit* BodyActor;
//	pbBasicDrawUnit* Effector;
//	pbBasicDrawUnit* DeadActor;

	npTextureRect* BodyActor;
	npTextureRect* Effector;
	npTextureRect* DeadActor;

//	sprite* effectSprite;
//	sprite* baseSprite;
//	sprite* deadEffectSprite;

	static GLfloat vertex[12];
	NOTETYPE::TYPE m_Types;

	int m_ID;
	NP_DEFINE_PROPERTY(float, noteWidth, NoteWidth);
	NP_DEFINE_PROPERTY(float, noteHeight, NoteHeight);
	NP_DEFINE_PROPERTY(float, effectWidth, EffectSpriteWidth);
	NP_DEFINE_PROPERTY(float, effectHeight, EffectSpriteheight);
	NP_DEFINE_PROPERTY(float, positionX, PositionX);
	NP_DEFINE_PROPERTY(float, positionY, PositionY);
	NP_DEFINE_PROPERTY(bool, onEffectMode, EffectMode);
	NP_DEFINE_PROPERTY(NOTESTATE, noteState, NoteState);
	NP_DEFINE_PROPERTY(NOTETYPES, noteType, NoteType);
	bool m_bEndPhase;

	void setGlTranslateByPosition();
	void prepareDrawUnitRendering();
public:
	int m_RentalIndex;
};

#endif /* PBNOTEELEMENT_H_ */
