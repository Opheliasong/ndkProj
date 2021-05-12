
#include "pbBackground.h"

//GLfloat pbBackground::m_BackgroundVertex[12] = {0,};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------pbBasicBackground-------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void pbBasicBackground::PreSettingDraw() {
	glPushMatrix();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	m_BackgroundTextureRect->PreSettingDraw();
}
void pbBasicBackground::DrawThis() {
	glTranslatef(m_vPos[0], m_vPos[1],  0);
	m_BackgroundTextureRect->DrawThis();
	glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------pbStaticBackground-------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pbStaticBackground::pbStaticBackground() {

}
pbStaticBackground::~pbStaticBackground() {
	delete m_BackgroundTextureRect;
}

void pbStaticBackground::Initialize(TAGDATA& TagData){
	m_BackgroundTextureRect = new pbBasicDrawUnit(TagData.Tag);
	m_BackgroundTextureRect->SetSize(TagData.fWidth, TagData.fHeight);
}

void pbStaticBackground::Update(float fTime) {

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------���� ��׶���-------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pbMoveBackground::pbMoveBackground() {

}
pbMoveBackground::~pbMoveBackground() {
	delete m_BackgroundTextureRect;
}

void pbMoveBackground::Initialize(TAGDATA& TagData){
	m_BackgroundTextureRect = new pbBasicDrawUnit(TagData.Tag);
	m_BackgroundTextureRect->SetSize(TagData.fWidth, TagData.fHeight);

	m_fBGAreaWidth = TagData.fWidth * 2 + WIDTH;
	m_fVertexWidth = TagData.fWidth;

}

void pbMoveBackground::Update(float fTime) {
	m_fBackgroundFlowPercent += (1/ ( (m_fBGAreaWidth/pbStageValue::GetStageMoveSpeed() )/m_fScrollRatio ) )*fTime;

	if( m_fBackgroundFlowPercent > 1.0f)
		m_fBackgroundFlowPercent = 0.0f;

	m_vPos[0] = npLerp( m_fBGAreaWidth - m_fVertexWidth,  -m_fVertexWidth,m_fBackgroundFlowPercent);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------��ũ�� ��׶���-------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GLfloat pbScrollBackground::m_UVControllVertex[18] = {0, };

pbScrollBackground::pbScrollBackground() {


}
pbScrollBackground::~pbScrollBackground() {
	
}

void pbScrollBackground::Initialize(TAGDATA& TagData){
		//Vertex ����
		SetVertexByCenter(m_UVControllVertex, TagData.fWidth, TagData.fHeight);			// ���� ���ؽ� ũ�� ����
		SetVertexByCenter(&(m_UVControllVertex[6]), TagData.fWidth, TagData.fHeight);			// ���� ���ؽ� ũ�� ����

		m_fBGAreaWidth = WIDTH;
		m_fVertexWidth = TagData.fWidth;

		//UV ����
		sprite* pSprite = npContainerDAO::GetInstance().getSpriteByTAG(TagData.Tag);

		m_BackgroundUVIndex = pSprite->currentScreenplay->getKernel();
		TextureAtlasIter textureAtlasIterator =  npAtlasMap::getInstance().FrameContainer.find(m_BackgroundUVIndex);
		UVPacket* uvPacket = &textureAtlasIterator->second;

		m_UVBindID = uvPacket->bindTextureID;
		memcpy(m_BackgroundUV, uvPacket->texture, sizeof(GLfloat)*8);

		m_StandardUV_LeftSideUCoord = m_BackgroundUV[4] ;
		m_StandardUV_RightSideUCoord = m_BackgroundUV[0];

		m_fBackgroundUV_Width = m_BackgroundUV[4] - m_BackgroundUV[0];

}

void pbScrollBackground::PreSettingDraw() {
	glBindTexture(GL_TEXTURE_2D,  m_UVBindID );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
void pbScrollBackground::DrawThis() {
	///--------------------------------------
	glPushMatrix();

	//-------------------------------������ �ؽ���-----------------------------------------//
	m_BackgroundUV[0] = m_StandardUV_RightSideUCoord + m_fBackgroundUV_WidthPercent;
	m_BackgroundUV[2] = m_BackgroundUV[0];
	m_BackgroundUV[4] = m_StandardUV_LeftSideUCoord;
	m_BackgroundUV[6] = m_BackgroundUV[4];
	glPushMatrix();
	glTranslatef(m_vPos[0], m_vPos[1],  0);
	glVertexPointer(3, GL_FLOAT, 0, m_UVControllVertex);
	glTexCoordPointer(2,GL_FLOAT, 0,  m_BackgroundUV);	//�ؽ�ó��ǥ(UV) �迭 �Ѱ��ֱ�
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glPopMatrix();
	//------------------------------�������� �ؽ���-----------------------------------------//
	m_BackgroundUV[0] = m_StandardUV_RightSideUCoord;
	m_BackgroundUV[2] = m_BackgroundUV[0];
	m_BackgroundUV[4] = m_StandardUV_RightSideUCoord + m_fBackgroundUV_WidthPercent;
	m_BackgroundUV[6] = m_BackgroundUV[4];
	glPushMatrix();
	glTranslatef(m_vPos[0], m_vPos[1],  0);
	glVertexPointer(3, GL_FLOAT, 0, &(m_UVControllVertex[6]) );
	glTexCoordPointer(2,GL_FLOAT, 0,  m_BackgroundUV );	//�ؽ�ó��ǥ(UV) �迭 �Ѱ��ֱ�
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glPopMatrix();

	glPopMatrix();
}

void pbScrollBackground::Update(float fTime) {
	m_fBackgroundFlowPercent += (1/ ( (m_fBGAreaWidth/pbStageValue::GetStageMoveSpeed())/m_fScrollRatio ) )*fTime;

	if( m_fBackgroundFlowPercent > 1.0f)
		m_fBackgroundFlowPercent = 0.0f;

	//------------------�߰� ���ؽ� ����---------------------//
	m_UVControllVertex[6] = m_UVControllVertex[12] - m_fVertexWidth * m_fBackgroundFlowPercent;
	m_UVControllVertex[9] = m_UVControllVertex[6];
	//==============================//

	//------UV ���� ���----------//
	m_fBackgroundUV_WidthPercent = m_fBackgroundUV_Width*m_fBackgroundFlowPercent;


}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------pbTouchableBackground   -------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pbTouchableBackground::pbTouchableBackground() {
	m_bTouched = false;
}
pbTouchableBackground::~pbTouchableBackground() {
	delete m_BackgroundTextureRect;
}

void pbTouchableBackground::Initialize(TAGDATA& TagData){
	m_BackgroundTextureRect = new pbBasicDrawUnit(TagData.Tag);
	m_BackgroundTextureRect->SetSize(TagData.fWidth, TagData.fHeight);

	m_fBGAreaWidth = TagData.fWidth * 2 + WIDTH;
	m_fVertexWidth = TagData.fWidth;

}

void pbTouchableBackground::Update(float fTime) {
/*	m_fBackgroundFlowPercent += (1/ ( (m_fBGAreaWidth/pbGlobalInGameVariable::fWorldMoveSpeed)/m_fScrollRatio ) )*fTime;

	if( m_fBackgroundFlowPercent > 1.0f)
		m_fBackgroundFlowPercent = 0.0f;

	m_vPos[0] = npLerp( m_fBGAreaWidth - m_fVertexWidth,  -m_fVertexWidth,m_fBackgroundFlowPercent);*/

}

void pbTouchableBackground::notify() {
	if(TouchLayer::GetInstance().touchFlag == TOUCHFLAGS::TAPDOWN) {
	//	LOGE("[DEBUG]pbTouchableBackground:: TAPDOWN");

		int x = TouchLayer::GetInstance().pointX;
		int y = TouchLayer::GetInstance().pointY;
//		LOGfloatString("X", x);
//		LOGfloatString("Y", y);

		int HalfWidth = m_BackgroundTextureRect->getWidth()/2;
		int HalfHeight =m_BackgroundTextureRect->getHeight()/2;

		int left = m_vPos[0] - HalfWidth;
		int right = m_vPos[0] + HalfWidth;
		int top = m_vPos[1] + HalfHeight;
		int bottom = m_vPos[1] - HalfHeight;

		if (x >= left && x <= right) {
			if (y >= bottom && y <= top) {
				m_bTouched = true;
				LOGE("[DEBUG]pbTouchableBackground:: Touched");
			}
		}
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------pbBackgroundProcessor-------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pbBackgroundProcessor::pbBackgroundProcessor() {
	m_pBackgroundQueue = NULL;
	m_iBackgroundCount = 0;

	m_pBackgroundQueue = new BackgroundList();
	m_pBackgroundQueue->setHeader();

	LOGI("pbBackgroundProcessor Create Complete");
}
pbBackgroundProcessor::~pbBackgroundProcessor() {
	ClearDataStore();
	BackgroundList::destroyList(m_pBackgroundQueue);
	LOGI("pbBackgroundProcessor::Release() Complete");
}

pbBackgroundProcessor& pbBackgroundProcessor::GetInstance() {
	static pbBackgroundProcessor SingleTon;
	return SingleTon;
}

pbBackground* pbBackgroundProcessor::AddScrollBackGround(float X, float Y, TAGDATA& TagData, float SpeedRatio) {
	pbBackground* pCreateBG = new pbScrollBackground();
	pCreateBG->Initialize(TagData);
	pCreateBG->SetPos(X, Y);
	pCreateBG->SetFlowSpeedRatio(SpeedRatio);

	registControled(pCreateBG);

	return pCreateBG;
}

pbBackground* pbBackgroundProcessor::AddMoveBackGround(float X, float Y, TAGDATA& TagData, float SpeedRatio) {
	pbBackground* pCreateBG = new pbMoveBackground();
	pCreateBG->Initialize(TagData);
	pCreateBG->SetPos(X, Y);
	pCreateBG->SetFlowSpeedRatio(SpeedRatio);

	registControled(pCreateBG);

	return pCreateBG;
}
pbBackground* pbBackgroundProcessor::AddStaticBackGround(float X, float Y, TAGDATA& TagData) {
	pbBackground* pCreateBG = new pbStaticBackground();
	pCreateBG->Initialize(TagData);
	pCreateBG->SetPos(X, Y);

	registControled(pCreateBG);

	return pCreateBG;
}

pbTouchableBackground* pbBackgroundProcessor::AddTouchableBackGround(float X, float Y, TAGDATA& TagData) {
	pbTouchableBackground* pCreateBG = new pbTouchableBackground();
	pCreateBG->Initialize(TagData);
	pCreateBG->SetPos(X, Y);

	registControled(pCreateBG);
	TouchLayer::GetInstance().RegistedObserver(pCreateBG);

	return pCreateBG;
}


void pbBackgroundProcessor::Update(float fTime) {
	BackgroundList* iterator;
	BackgroundList* head = m_pBackgroundQueue;
	iterator = head->getNext();
	while( iterator != head ) {
		pbBackground* pkernel = iterator->getKernel();
		iterator = iterator->getNext();
		pkernel->Update(fTime);
	}
}

void pbBackgroundProcessor::ClearDataStore() {
	LinkNodeDeleteAllKernel(pbBackground*, m_pBackgroundQueue)
	BackgroundList::clearList(m_pBackgroundQueue);

	LOGfloatString("ClearBackground :", m_iBackgroundCount);

	m_iBackgroundCount = 0;

	LOGI("pbBackgroundProcessor::ClearDataStore() Complete");
}

void pbBackgroundProcessor::registControled(pbBackground* pBG){
	BackgroundList::addTail(pBG,m_pBackgroundQueue);

	m_iBackgroundCount++;
}

void pbBackgroundProcessor::DeleteBackground(pbBackground* pBG) {
	if( pBG != NULL ) {
		BackgroundList::findDelete(pBG,m_pBackgroundQueue);
		delete pBG;

		m_iBackgroundCount--;
	}
}

/*
pbShootingStar* pbShootingStar::SingleObject = NULL;

pbShootingStar::pbShootingStar() {
	m_iMaxStar = 0;

	m_VertexIndex = 0;
	m_UVIndex = 0;

	m_pfScale = NULL;
	m_pvPos = NULL;
}
pbShootingStar::~pbShootingStar() {

}

void pbShootingStar::Initialize(int size) {
	if( SingleObject == NULL) {
		SingleObject = new pbShootingStar();

		SingleObject->m_iMaxStar = size;
		SingleObject->m_pfScale = new float[size];
		SingleObject->m_pfAlpha = new float[size];

		SingleObject->m_pvPos = new float*[size];

		for(int i = 0; i < size; ++i) {
			SingleObject->m_pvPos[i] = new float[2];
		}

		SingleObject->DataReset();

		SingleObject->m_VertexIndex = 6;
		SingleObject->m_UVIndex = 22;

		pbRenderProcess::RegistRenderShootingStar(SingleObject);
	}
}

void pbShootingStar::DataReset() {
	for(int i = 0; i < m_iMaxStar; ++i) {
		m_pvPos[i][0] = i*800;
		m_pvPos[i][1] = (double)(rand()%410) + 20.0f;
		m_pfScale[i] =  ( (double)(rand()%25) )/10 + 0.5f;
		m_pfAlpha[i] =  25/(m_pfScale[i] * 100);
	}
}

void pbShootingStar::Draw() {
	///--------------------------------------
	pbDataStorage::BindVertexAndTexture(m_VertexIndex, m_UVIndex);

	//�ؽ�ó ���ε�
	for(int i = 0; i < m_iMaxStar; ++i) {
		glPushMatrix();

		glColor4f(1.0, 1.0, 1.0, m_pfAlpha[i]);
		glTranslatef(m_pvPos[i][0], m_pvPos[i][1],  0);
		glScalef(m_pfScale[i], m_pfScale[i], 1.0f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glPopMatrix();
	}


}

void pbShootingStar::Update(float fTime) {
	for(int i = 0; i < m_iMaxStar; ++i) {
		if(m_pvPos[i][0] < 0.0f ) {
			m_pvPos[i][0] = (double)(rand()%100 + 900);
			m_pvPos[i][1] = (double)(rand()%410) + 20.0f;
			m_pfScale[i] =  ( (double)(rand()%25) )/10 + 0.5f;
			m_pfAlpha[i] =  50/(m_pfScale[i] * 100);
		}

		m_pvPos[i][0] += pbGlobalInGameVariable::fWorldMoveX*(m_pfAlpha[i]*3.0f);
	}
}

void pbShootingStar::Release() {
	delete[] SingleObject->m_pfScale;
	delete[] SingleObject->m_pfAlpha;

	for(int i = 0; i < SingleObject->m_iMaxStar; ++i) {
		delete [] SingleObject->m_pvPos[i];
	}

	delete[] SingleObject->m_pvPos;

	delete SingleObject;
	LOGI("pbShootingStar::Release() Complete");
}*/
