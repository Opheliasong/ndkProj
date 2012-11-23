#include "pbDrawUnit.h"

void pbDrawUnit::registLinkNode(pbDrawUnit* pUnit){
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
}
