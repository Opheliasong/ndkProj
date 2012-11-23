
#include "pbBackground.h"

GLfloat pbBackground::m_BackgrounVertex[12] = {0,};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------pbStaticBackground-------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pbStaticBackground::pbStaticBackground() {

}
pbStaticBackground::~pbStaticBackground() {

}

void pbStaticBackground::Initialize(float Width, float Height, GLuint UVIndex){
		//Vertex ����
		SetVertexByCenter(m_BackgrounVertex, Width, Height);			// ���� ���ؽ� ũ�� ����

		//UV ����
		m_BackgroundUVIndex = UVIndex;

}

void pbStaticBackground::PreSettingDraw() {
	TextureAtlasIter textureAtlasIterator =  npAtlasMap::getInstance().FrameContainer.find(m_BackgroundUVIndex);
	UVPacket* uvPacket = &textureAtlasIterator->second;

	glBindTexture(GL_TEXTURE_2D,  uvPacket->bindTextureID );
	glTexCoordPointer(2,GL_FLOAT, 0,  uvPacket->texture);	//텍스처좌표(UV) 배열 넘겨주기
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
void pbStaticBackground::DrawThis() {
	glPushMatrix();

	//-------------------------------������ �ؽ���-----------------------------------------//
	glTranslatef(m_vPos[0], m_vPos[1],  0);
	glVertexPointer(3, GL_FLOAT, 0, m_BackgrounVertex);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glPopMatrix();
}

void pbStaticBackground::Update(float fTime) {

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------���� ��׶���-------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pbMoveBackground::pbMoveBackground() {

}
pbMoveBackground::~pbMoveBackground() {

}

void pbMoveBackground::Initialize(float Width, float Height, GLuint UVIndex){
		//Vertex ����
		SetVertexByCenter(m_BackgrounVertex, Width, Height);			// ���� ���ؽ� ũ�� ����

		m_fBGAreaWidth = Width*2 + WIDTH;	//ȭ�� �ٱ� �ݴ��� �ٱ����
		m_fVertexWidth = Width;
		//UV ����
		m_BackgroundUVIndex = UVIndex;

}

void pbMoveBackground::PreSettingDraw() {
	TextureAtlasIter textureAtlasIterator =  npAtlasMap::getInstance().FrameContainer.find(m_BackgroundUVIndex);
	UVPacket* uvPacket = &textureAtlasIterator->second;

	glBindTexture(GL_TEXTURE_2D,  uvPacket->bindTextureID );
	glTexCoordPointer(2,GL_FLOAT, 0,  uvPacket->texture);	//텍스처좌표(UV) 배열 넘겨주기
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
void pbMoveBackground::DrawThis() {
	///--------------------------------------
	glPushMatrix();
	glTranslatef(m_vPos[0], m_vPos[1],  0);
	glVertexPointer(3, GL_FLOAT, 0, m_BackgrounVertex);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glPopMatrix();
}

void pbMoveBackground::Update(float fTime) {
	m_fBackgroundFlowPercent += (1/ ( (m_fBGAreaWidth/pbGlobalInGameVariable::fWorldMoveSpeed)/m_fScrollRatio ) )*fTime;

	if( m_fBackgroundFlowPercent > 1.0f)
		m_fBackgroundFlowPercent = 0.0f;

	m_vPos[0] = npLerp( m_fBGAreaWidth - m_fVertexWidth,  -m_fVertexWidth,m_fBackgroundFlowPercent);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------��ũ�� ��׶���-------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GLfloat pbScrollBackground::m_BackgrounVertex[18] = {0, };

pbScrollBackground::pbScrollBackground() {
	m_vPos[0] = 0.0f;
	m_vPos[1] = 0.0f;

	m_fBackgroundUV_WidthPercent = 1.0f;
	m_fBackgroundFlowPercent = 1.0f;
	m_fBGAreaWidth = 0.0f;;
	m_fBackGroundUV_Width = 1.0f;
	m_fScrollRatio = 1.0f;

	m_UVBindID = 0;

	m_BackgroundUVIndex  = 0 ;

	m_StandardUV_LeftSideUCoord = 0;
	m_StandardUV_RightSideUCoord = 0;

}
pbScrollBackground::~pbScrollBackground() {
	
}

void pbScrollBackground::Initialize(float Width, float Height, GLuint UVIndex){


		//Vertex ����
		SetVertexByCenter(m_BackgrounVertex, Width, Height);			// ���� ���ؽ� ũ�� ����
		SetVertexByCenter(&(m_BackgrounVertex[6]), Width, Height);			// ���� ���ؽ� ũ�� ����

		m_fBGAreaWidth = WIDTH;
		m_fVertexWidth = Width;

		//UV ����
		m_BackgroundUVIndex = UVIndex;
		TextureAtlasIter textureAtlasIterator =  npAtlasMap::getInstance().FrameContainer.find(m_BackgroundUVIndex);
		UVPacket* uvPacket = &textureAtlasIterator->second;

		m_UVBindID = uvPacket->bindTextureID;
		memcpy(m_BackgroundUV, uvPacket->texture, sizeof(GLfloat)*8);

		m_StandardUV_LeftSideUCoord = m_BackgroundUV[4] ;
		m_StandardUV_RightSideUCoord = m_BackgroundUV[0];

		m_fBackGroundUV_Width = m_BackgroundUV[4] - m_BackgroundUV[0];

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
	glVertexPointer(3, GL_FLOAT, 0, m_BackgrounVertex);
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
	glVertexPointer(3, GL_FLOAT, 0, &(m_BackgrounVertex[6]) );
	glTexCoordPointer(2,GL_FLOAT, 0,  m_BackgroundUV );	//�ؽ�ó��ǥ(UV) �迭 �Ѱ��ֱ�
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glPopMatrix();

	glPopMatrix();
}

void pbScrollBackground::Update(float fTime) {

	m_fBackgroundFlowPercent += (1/ ( (m_fBGAreaWidth/pbGlobalInGameVariable::fWorldMoveSpeed)/m_fScrollRatio ) )*fTime;

	if( m_fBackgroundFlowPercent > 1.0f)
		m_fBackgroundFlowPercent = 0.0f;

	//------------------�߰� ���ؽ� ����---------------------//
	m_BackgrounVertex[6] = m_BackgrounVertex[12] - m_fVertexWidth * m_fBackgroundFlowPercent;
	m_BackgrounVertex[9] = m_BackgrounVertex[6];
	//==============================//

	//------UV ���� ���----------//
	m_fBackgroundUV_WidthPercent = m_fBackGroundUV_Width*m_fBackgroundFlowPercent;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------��׶��� ���μ���-------------------------------------------------------------------------------//
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

pbBackground* pbBackgroundProcessor::AddScrollBackGround(float Width, float Height, float X, float Y, float SpeedRatio, screenplayTag Tag) {
	pbBackground* pCreateBG = new pbScrollBackground();
	sprite* pSprite = npContainerDAO::GetInstance().getSpriteByTAG(Tag);

	pCreateBG->Initialize(Width, Height, pSprite->currentScreenplay->getKernel());
	pCreateBG->SetPos(X, Y);
	pCreateBG->SetFlowSpeedRatio(SpeedRatio);

	registControled(pCreateBG);

	return pCreateBG;
}

pbBackground* pbBackgroundProcessor::AddMoveBackGround(float Width, float Height, float X, float Y, float SpeedRatio, screenplayTag Tag) {
	pbBackground* pCreateBG = new pbMoveBackground();
	sprite* pSprite = npContainerDAO::GetInstance().getSpriteByTAG(Tag);

	pCreateBG->Initialize(Width, Height, pSprite->currentScreenplay->getKernel());
	pCreateBG->SetPos(X, Y);
	pCreateBG->SetFlowSpeedRatio(SpeedRatio);

	registControled(pCreateBG);

	return pCreateBG;
}
pbBackground* pbBackgroundProcessor::AddStaticBackGround(float Width, float Height, float X, float Y, screenplayTag Tag) {
	pbBackground* pCreateBG = new pbStaticBackground();
	sprite* pSprite = npContainerDAO::GetInstance().getSpriteByTAG(Tag);

	pCreateBG->Initialize(Width, Height, pSprite->currentScreenplay->getKernel());
	pCreateBG->SetPos(X, Y);

	registControled(pCreateBG);

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

	LOGfloatString("ClearUI :", m_iBackgroundCount);

	m_iBackgroundCount = 0;

	LOGI("pbBackgroundProcessor::Release() Complete");
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
