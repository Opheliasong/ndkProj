/*
 * pbNote.h
 *
 *  Created on: 2012. 7. 10.
 *      Author: NitroPigSoft02
 */

#ifndef PBNOTE_H_
#define PBNOTE_H_

#include "stdafx.h"

#define __DEBUG__
namespace projectBean {

class pbTouchRect {

public:
	float top;
	float left;
	float right;
	float bottom;

	pbTouchRect();
	~pbTouchRect();
	pbTouchRect(int aTop, int aBottom, int aLeft, int aRight);
	inline void setRects(int aTop, int aBottom, int aLeft, int aRight);
	inline bool IsInRect(float x, float y);
};

//===============Touch Area ����========================

class pbTouchArea {
protected:
	pbTouchRect* m_Body;
	float m_BodyWidth;
	float m_BodyHeight;
	float m_BodyHalfWidth;
	float m_BodyhalfHeight;
public:
	pbTouchArea();
	virtual ~pbTouchArea();

	bool InRect(float x, float y);
	void setTouchArea(float bodyWidth, float bodyHeight);		// ���� : ��ġ�� leftTop �����̴���� Center �������� ����
	void movePositon(float x, float y);

	void LogPrintRect();

	//Body Width/Height Getter
	virtual inline int getBodyWidth() {
		return m_BodyWidth;
	}
	virtual inline int getBodyHeight() {
		return m_BodyHeight;
	}
	virtual inline int getBodyHalfWidth() {
		return m_BodyHalfWidth;
	}
	virtual inline int getBodyHalfHeight() {
		return m_BodyhalfHeight;
	}
	virtual inline pbTouchRect* getTouchRect() { return m_Body;}
};


//===============Note �� ����========================
class pbNote: public npObserver {
protected:
	pbTouchArea* m_TouchArea; //Note �� ��ġ �� ������ ���ϴ� TouchArea

	float m_fAlertTime;
	NOTEGROWSTATE::TYPE m_NoteGrowState;		//��Ʈ�� �����ϴ� �Ϳ� �� ����

	NOTETYPE::TYPE m_Types;
	NOTEJUDGEMENT::TYPE m_NoteJudgement;

	NoteState::STATE m_State;				// ���� : 8-24

	COLOR_RGBA m_Color;			//�߰� : 8-24
	GLuint m_BodyVertexIndex; //�ٵ��� ���ؽ����� index
	GLuint m_BodyUVIndex; //�ٵ��� UV ���� index

	COLOR_RGBA m_EffectColor; // �߰� : 8-29
	GLuint m_EffectVertexIndex;
	GLuint m_EffectUVIndex; //����Ʈ���� UV ���� index
	float m_fEffectScale;
	float m_fEffectAngle;

	float m_fAnimationTime;	//- + 0 ���� ���������� �Ѵ�. �߰� : 8-29
	float m_fAnimationPercent;	// �߰� : 8-29

	int m_ID;
	float m_PositionX; //Note�� ��ġ�� x
	float m_PositionY; //Note�� ��ġ�� y

	bool m_bEndPhase;				//�߰� : 8-24

	bool m_TargetOnSwitch;

public:
	int m_RentalIndex;

protected:
	//Touch ����Ȯ�� �Լ�
	virtual bool CheckingTouchOn(int x, int y);
	virtual inline void BindColor() { glColor4f(m_Color.R,m_Color.G, m_Color.B, m_Color.A); }
	virtual inline void BindEffectColor() { glColor4f(m_EffectColor.R,m_EffectColor.G, m_EffectColor.B, m_EffectColor.A); }

public:
	//--------�⺻ �Լ� ------------------------//
	pbNote();
	pbNote(int ID, unsigned int VertexIndex, int width, int height,
			unsigned int UvIndex, int x, int y);
	virtual ~pbNote();

	virtual void ValueInitializer();
	virtual void randOnSwitch();

	virtual void JudgeMissNote();

	virtual void Draw() {};
	virtual void Update(float fTime) {};

	//---------observer ��� �Լ� ------------------------//
	virtual void notify();
	virtual void notify(int x, int y, TOUCHSTATUS::TYPE Touchstatus);
	virtual void notify(TOUCHSTATUS::TYPE touchStatus);
	virtual void wakeUpNotify();
	virtual float resetWakeTime();

	//-----------���� �Լ� ------------------------//
	virtual void setNotePosition(float x, float y);
	inline float GetPosX() {return m_PositionX;}
	inline float GetPosY() {return m_PositionY;}

	virtual NOTETYPE::TYPE getTypes() { return m_Types; }
	virtual void setTypes(NOTETYPE::TYPE Type){ m_Types = Type;}

	virtual inline NOTEGROWSTATE::TYPE getGrowState(){return m_NoteGrowState;}
	virtual inline void setGrowState(NOTEGROWSTATE::TYPE State){m_NoteGrowState = State;}

	virtual inline bool getTargetOnSwitch(){return m_TargetOnSwitch;}
	virtual inline void SetTargetOnSwitch(bool Switch) { m_TargetOnSwitch = Switch;}

	virtual inline bool GetEndPhase() { return m_bEndPhase; }
	virtual inline void SetEndPhase(bool isEndPhase) { m_bEndPhase = isEndPhase; }

	virtual inline NoteState::STATE GetNoteState() { return m_State; }
	virtual inline void SetNoteState(NoteState::STATE State) { m_State = State; }

	virtual inline NOTEJUDGEMENT::TYPE GetNoteJudge() { return m_NoteJudgement; }
	virtual inline void SetNoteJudge(NOTEJUDGEMENT::TYPE NoteJudge) { m_NoteJudgement = NoteJudge; }
	//---------TouchArea ��� �Լ� ------------------------//
	virtual void setTouchWidthAndHeight(int width, int height);	//���� : ���� ���ؽ��� ����ϱ⿡ ��ġ �������� ��谡�־� Touch�� �����ϴ� �Լ������� ����8-27
	virtual inline int getBodyHeight() {
		return m_TouchArea->getBodyHeight();
	}
	virtual inline int getBodyWidth() {
		return m_TouchArea->getBodyWidth();
	}

	virtual inline pbTouchArea* getTouchArea() { return m_TouchArea;}

};


/*=====================
 * pbSwipeNote
 */
class pbSwipeNote: public pbNote {
public:
	pbSwipeNote();
	~pbSwipeNote();

	void ExtraInitialize();

	void notify(int x, int y, TOUCHSTATUS::TYPE Touchstatus);

	void wakeUpNotify();
	float resetWakeTime();

	void setVertexIndices(GLuint  BodyIndex,GLuint EffectIndex);
	void setUVIndices(GLuint  BodyIndex,GLuint EffectIndex);
	void setEffectUVIndex(GLuint UVIndex){m_EffectUVIndex = UVIndex;}

	void Draw();

	void Update(float fTime);

	enum { ANICOUNT = 9};
private:
	GLuint m_AniUVStartIndex;
	GLuint m_AniUVEndIndex;
};

/*=====================
 * Double Tab Note.
 *=====================
 */
class pbDoubleTapNote: public pbNote {
public:

	pbDoubleTapNote();
	~pbDoubleTapNote();

	void ExtraInitialize();

	void notify(int x, int y, TOUCHSTATUS::TYPE Touchstatus);

	void wakeUpNotify();
	float resetWakeTime();

	virtual void setNotePosition(float x, float y);
	void SetSecondNotePosDelta(GLfloat DistanceFromPosX, GLfloat DistanceFromPosY);
	void setVertexIndices(GLuint  BodyIndex,GLuint EffectIndex);
	void setUVIndices(GLuint  BodyIndex, GLuint EffectIndex);

	void Draw();

	void Update(float fTime);

	enum { ANICOUNT = 8};
private:
	npV2Vector m_vSecondNoteCurrentPos;			//�ι�° ��Ʈ ������ġ
	npV2Vector m_vSecondNoteDestDistance;			//�ι�° ��Ʈ �̵���ġ
	int m_iNoteTouchCount;						//�ι��� ��Ʈ ��� ���� ��ġ ī��Ʈ
	float m_fSecondNoteAlpha;
	float m_fSecondNoteTime;

	GLuint m_AniUVStartIndex;
	GLuint m_AniUVEndIndex;
};

/*=====================
 * LongPress Note.
 *=====================
 */
class pbLongPressNote: public pbNote {
public:

	pbLongPressNote();
	~pbLongPressNote();

	void ExtraInitialize();

	void notify(int x, int y, TOUCHSTATUS::TYPE Touchstatus);

	void wakeUpNotify();
	float resetWakeTime();

	void SetDestination(GLfloat DistanceFromPosX, GLfloat DistanceFromPosY);
	void setVertexIndices(GLuint  BodyIndex);
	void setUVIndices(GLuint  BodyIndex, GLuint  EffectIndex, GLuint  DestinationIndex);

	void Draw();

	void Update(float fTime);

private:
	float m_fActionPointDistance;			//�ص������ ���۵Ǵ� ����
	float m_fActionPointPosY;
	float m_fPreVisionPosX;					//�������־���� ���� ǥ���ϴ� ��ġ
	float m_fDestinationPosY;			//���������� ���ԵǴ� ��

	float m_fPressPercent;
	float m_fPressTime;

	bool m_bNotePressed;

	int m_iPhase;

	float m_fRotate;

	GLuint m_DestinationVertIndex; //�������� ���ؽ����� index
	GLuint m_DestinationUVIndex; //�������� UV ���� index
};


class pbTargetingNote: public pbNote{
public:
	pbTargetingNote();
	~pbTargetingNote();

	void notify(int x, int y, TOUCHSTATUS::TYPE Touchstatus);

	void setVertexIndices(GLuint  BodyIndex,GLuint EffectIndex);
	void setUVIndices(GLuint  BodyIndex, GLuint EffectIndex);

	void setEffectUVIndex(GLuint UVIndex){m_EffectUVIndex = UVIndex;}

	void wakeUpNotify();
	float resetWakeTime();

	void Draw();

	void Update(float fTime);

	enum { ANICOUNT = 8};
private:
	GLuint m_AniUVStartIndex;
	GLuint m_AniUVEndIndex;


};

};
#endif /* PBNOTE_H_ */
