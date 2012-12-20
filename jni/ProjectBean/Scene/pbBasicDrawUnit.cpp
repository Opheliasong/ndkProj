#include "pbBasicDrawUnit.h"

/*void pbDrawUnit::registLinkNode(pbDrawUnit* pUnit){
	pbDrawUnit_List::addTail(pUnit, m_pDrawableUnitListHeader);
}

void pbDrawUnit::removeLinkNode(pbDrawUnit* pUnit){
	pbDrawUnit_List::findDelete(pUnit, m_pDrawableUnitListHeader);
}


pbDrawUnit::pbDrawUnit() {
	m_vPos[iX] = 0;
	m_vPos[iY] = 0;
	m_SizefWH[iW] = 0;
	m_SizefWH[iH] = 0;
	m_UVIndex = 0;
	m_ID = -1;
	m_pDrawableUnitListHeader = NULL;

	m_Color.Init(1.0f, 1.0f, 1.0f, 1.0f);
}

pbDrawUnit::~pbDrawUnit() {
	if( m_pDrawableUnitListHeader != NULL) {
		LinkNodeDeleteAllKernel(pbDrawUnit*, m_pDrawableUnitListHeader);
		pbDrawUnit_List::destroyList(m_pDrawableUnitListHeader);

		LOGI("pbDrawableUnit::~pbDrawableUnit() destroy List");
	}
	LOGI("pbDrawableUnit::~pbDrawableUnit() complete");
}


void pbDrawUnit::Draw() {
	glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);

	glPushMatrix();

	pbDataStorage::BindVertexAndTexture(m_UVIndex);

	glTranslatef(m_vPos[iX], m_vPos[iY],  0);
	glScalef(m_SizefWH[iW], m_SizefWH[iH], 1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glScalef(1/m_SizefWH[iW], 1/m_SizefWH[iH], 1.0f);

	if( m_pDrawableUnitListHeader != NULL ) {
		pbDrawUnit_List* Iter = m_pDrawableUnitListHeader->getNext();

		while(Iter != m_pDrawableUnitListHeader ) {
			pbDrawUnit* pDraw = Iter->getKernel();
			pDraw->Draw();
			Iter = Iter->getNext();
		}
	}

	glPopMatrix();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void pbDrawUnit::AddChildUnit(pbDrawUnit* pChild) {
	if( m_pDrawableUnitListHeader == NULL ) {
		m_pDrawableUnitListHeader = new pbDrawUnit_List;
		m_pDrawableUnitListHeader->setHeader();
	}

	registLinkNode(pChild);

}
void pbDrawUnit::DeleteChildUnit(GLuint ID) {
	if( m_pDrawableUnitListHeader != NULL ) {
		pbDrawUnit_List* Iter = m_pDrawableUnitListHeader->getNext();

		while(Iter != m_pDrawableUnitListHeader ) {
			pbDrawUnit* pDraw = Iter->getKernel();
			Iter = Iter->getNext();

			if( pDraw->GetID() == ID) {
				removeLinkNode(pDraw);
				delete pDraw;
			}

		}
	}
}*/


GLfloat pbBasicDrawUnit::vertex[12] = {0,};

pbBasicDrawUnit::pbBasicDrawUnit(){
	SetVertexByCenter(this->vertex, 1, 1);
	sprtie = NULL;
	m_Width = 0;
	m_Height = 0;
	LOGE("pbBasicDrawUnit :: Call Constructor");
}

pbBasicDrawUnit::pbBasicDrawUnit(screenplayTag TAG){
	SetVertexByCenter(this->vertex, 1, 1);

	this->tag = TAG;
	this->sprtie = npContainerDAO::GetInstance().getSpriteByTAG(this->tag);
	m_Width = 0;
	m_Height = 0;

}

pbBasicDrawUnit::pbBasicDrawUnit(const pbBasicDrawUnit& source){
	this->tag = source.tag;
	this->sprtie = source.sprtie;
	this->m_Height = source.m_Height;
	this->m_Width = source.m_Width;
}

pbBasicDrawUnit::~pbBasicDrawUnit(){
}

void pbBasicDrawUnit::PreSettingDraw() {
	npContainerDAO::GetInstance().BindingFrameBySprite(*this->sprtie);
}

void pbBasicDrawUnit::DrawThis() {
	glScalef(this->m_Width, this->m_Height, 1.0f);
	glVertexPointer(3, GL_FLOAT, 0,this->vertex);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//	glScalef(1/this->m_Width, 1/this->m_Height, 1.0f);
}

void pbBasicDrawUnit::SetSize(float width, float Height) {
	this->m_Width = width;
	this->m_Height = Height;
}

bool pbBasicDrawUnit::SetTextureTAG(screenplayTag TAG) {
	this->sprtie = npContainerDAO::GetInstance().getSpriteByTAG(TAG);

	if(NP_IS_EMPTY(this->sprtie)){
		LOGE("not Fine Sprite Tag");
		return false;
	}
	return true;
}

sprite* pbBasicDrawUnit::getSprite() {
	return this->sprtie;
}
