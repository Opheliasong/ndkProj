#ifndef _GUIDELINE_H
#define _GUIDELINE_H
#include "stdafx.h"

class pbDriedGuideLineData{
public:
	pbDriedGuideLineData(){ PosX = 0.0f; PosY = 0.0f; }
	~pbDriedGuideLineData(){}
public:
	GLfloat PosX;
	GLfloat PosY;
};

typedef npLinkNode<pbDriedGuideLineData> pbDriedGuideLineList;
typedef npLinkNode<pbDriedGuideLineData>* pbDriedGuideLineList_Iterator;

//------------------------------------------------------���̵���� ��ο�------------------------------------------------------------//
class pbGuideLineDrawer {
public:
	pbGuideLineDrawer();
	~pbGuideLineDrawer();

	void SetFirstVertexBuffer(float* pV2Pos);
	void SetVertexBuffer(pbDriedGuideLineList_Iterator pStarIterator);

	inline void SetPosX(float PosX) { m_vPos[0] = PosX; }
	void SetPos(float PosX, float PosY) { m_vPos[0] = PosX; m_vPos[1] = PosY; }

	void Draw();

	enum { MAX_VERTEX = 24, MAX_VERTEXBUFFER = MAX_VERTEX*2,};
private:
	float m_VertexBuffer[MAX_VERTEXBUFFER];
	float* m_pV2FirstPos;

	pbDriedGuideLineList* m_pDriedLineDataListHeader;
	pbDriedGuideLineList_Iterator m_DriedLineDataList_Iterator;
	npV2Vector m_vPos;

	int m_AddIndex;
};

//------------------------------------------------------���̵���� �δ�------------------------------------------------------------//
class pbGuideLineLoader {
public:
	pbGuideLineLoader();
	~pbGuideLineLoader();

	void registLinkNode(pbDriedGuideLineData* pLinedata);
	void removeLinkNode(pbDriedGuideLineData* pLinedata);

	bool GuideLineDataParsing(const char* fileName);

	void SetFirstLineForLerpPoint();
	void FirstNodeDelete();

	void SettingPosYFromFirstLineLerp(float* pV2Pos);
	void SetMovedPosX(float fPosX) { m_fMovedPosX = fPosX;}

	pbDriedGuideLineList_Iterator GetSecondLineIterator() { return m_pSecondLineIter; }

	void SetUpdateList(bool Update) { m_bUpdateList = Update; }
	bool GetUpdateList() { return m_bUpdateList;}

	void Clear();

private:
	//--------�Ľ� ������--------------//
	pbDriedGuideLineList* m_pDriedLineDataListHeader;
	pbDriedGuideLineList_Iterator m_pSecondLineIter;
	GLuint m_TotalGuideLineData;

	npV2Vector m_vPos_Point1;
	npV2Vector m_vPos_Point2;

	float m_fMovedPosX;
	bool m_bUpdateList;
};


//------------------------------------------------------���̵���� ���ʷ�����------------------------------------------------------------//

class pbGuideLineGenerator{
private:
	pbGuideLineGenerator();
	~pbGuideLineGenerator();

public:
	static void Create();

	void LoadGuideLine(const char* fileName);

	void Draw();

	void Update(float fTime);

	void ClearDataStore();
	static void Release();


	static pbGuideLineGenerator* GetInstance() { return m_pSingleObject;}
private:
	static pbGuideLineGenerator* m_pSingleObject;

	pbGuideLineLoader* m_pGuideLineLoader;
	pbGuideLineDrawer* m_pGuideLineDrawer;

};

/*class pbGuideLineVertexData{
public:
	pbGuideLineVertexData(){ LinkedPosX = NULL; LinkedPosY = NULL; m_RentalIndex = -1;}
	~pbGuideLineVertexData(){}
public:
	GLfloat* LinkedPosX;
	GLfloat* LinkedPosY;

	int m_RentalIndex;
};*/


/*class pbGuideLineGenerator{
private:
	pbGuideLineGenerator();
	~pbGuideLineGenerator();

	inline bool IsInScreen(float x, float y);
	void FirstDataOnLoad();
	void AddNextVertexData();
	void DeleteFirstGuideLineVertex();
	void InScreenCheckAndAddData();
	void FollowLineControl();

	bool GuideLineDataParsing(const char* fileName);
public:
	static void Create();

	void LoadGuideLine(const char* fileName);

	void Draw();

	void MoveGuideLine(float fMoveX,  float fMoveY);

	void ClearDataStore();
	static void Release();

	GLfloat** GetFollowLineVertexBuffer() { return m_pSingleObject->m_FollowLineVertexPointer;}

	enum { MAX_VERTEX = 12, MAX_VERTEXBUFFER = MAX_VERTEX*3,};

	static pbGuideLineGenerator* GetInstance() { return m_pSingleObject;}
private:
	static pbGuideLineGenerator* m_pSingleObject;

	pbMemoryRentalUnit<pbGuideLineVertexData>* m_pVertexDataRentalUnit;

	//-------���ؽ� ������---------//
	pbGuideLineVertexData* m_pVertexDataStore[MAX_VERTEX];
	GLfloat m_GuideLineVertexBuffer[MAX_VERTEXBUFFER];
	GLuint m_CurrentStoreIndex;			//�ѷ����� ���ؽ��� �ε��� ī��Ʈ

	int m_iFollowLineCheckCount;
	GLfloat* m_FollowLineVertexPointer[2];
	//--------�Ľ� ������--------------//
	pbDriedGuideLineData* m_pDriedLineData;
	GLuint m_TotalGuideLineData;
	GLuint m_CurrentDriedLineDataIndex;		//�����;��ϴ� �Ľ̵������� �ε��� ī��Ʈ

	bool m_bEndGenerating;

};*/


#endif
