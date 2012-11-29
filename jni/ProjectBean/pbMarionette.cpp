#include "pbMarionette.h"
//-----------------------------------------------------������Ʈ ����-----------------------------------------------------------------------------//
float* pbLineMover::GetV2PosByTime(float fAccumulateTime) {
	m_vPos[0] = m_vStartPos[0] + m_vDir[0] * fAccumulateTime;
	m_vPos[1] = m_vStartPos[1] + m_vDir[1] * fAccumulateTime;

	return m_vPos;
}

float* pbZigZagMover::GetV2PosByTime(float fAccumulateTime) {
	m_vPos[0] = m_vStartPos[0] + m_vDir[0]* fAccumulateTime + ( m_fAmplitudeX * sin( m_fCycle * fAccumulateTime )  );
	m_vPos[1] = m_vStartPos[1] + m_vDir[1]* fAccumulateTime + ( m_fAmplitudeY * sin( m_fCycle * fAccumulateTime )  );

	return m_vPos;
}


//-----------------------------------------------------��������Ʈ-----------------------------------------------------------------------------//

pbMarionette::pbMarionette() {
	m_vPos[0] = 0;
	m_vPos[1] = 0;
	m_pCurrentMover = NULL;
	m_pMoverHead = new pbObjectMoverList;
	m_pMoverHead->setHeader();
	m_CurrentState = -1;
	m_fAccumulateTime = 0.0f;
	m_bUpdatePause = false;
}
pbMarionette::~pbMarionette() {
	ClearDataStore();
	delete m_pMoverHead;
}

void pbMarionette::AddLineMoveState(int ID, float dirX, float dirY){
	pbObjectMover* createMover = new pbLineMover(ID, dirX, dirY);

	pbObjectMoverList* pTargetNode =  pbObjectMoverList::makeLinkNode(createMover);
	pbObjectMoverList::addTail(pTargetNode, m_pMoverHead);
}

void pbMarionette::AddZigZagMoveState(int ID, float dirX, float dirY, float fAmplitude, float fCycle){
	pbObjectMover* createMover = new pbZigZagMover(ID, dirX, dirY, fAmplitude, fCycle);

	pbObjectMoverList* pTargetNode =  pbObjectMoverList::makeLinkNode(createMover);
	pbObjectMoverList::addTail(pTargetNode, m_pMoverHead);
}

void pbMarionette::SelectMoveState(int ID) {
	pbObjectMoverList* iterator;
	pbObjectMoverList* head = m_pMoverHead;
	iterator = head->getNext();
	while (iterator != head) {
		pbObjectMover* pkernel = iterator->getKernel();
		iterator = iterator->getNext();

		if( pkernel->GetID() == ID ) {
			m_pCurrentMover = pkernel;
			m_pCurrentMover->SetStartPos(m_vPos[0], m_vPos[1]);
			m_CurrentState = ID;
			m_fAccumulateTime = 0.0f;
			return;
		}

	}

	//ID�� ������ NULL
	m_pCurrentMover = NULL;
	m_CurrentState = ID;
	m_fAccumulateTime = 0.0f;
}

void pbMarionette::DeleteMoveState(int ID) {
	pbObjectMoverList* iterator;
	pbObjectMoverList* head = m_pMoverHead;
	iterator = head->getNext();
	while (iterator != head) {
		pbObjectMover* pkernel = iterator->getKernel();
		iterator = iterator->getNext();

		if( pkernel->GetID() == ID ) {
			pbObjectMoverList::destroyNode(iterator);
			delete pkernel;
			return;
		}

	}
}

void pbMarionette::ClearDataStore() {
	LinkNodeDeleteAllKernel(pbObjectMover*, m_pMoverHead);
	pbObjectMoverList::clearList(m_pMoverHead);
}

void pbMarionette::MoveUpdate(float fTime) {
	if( !m_bUpdatePause ) {
		if( m_pCurrentMover != NULL) {
			m_fAccumulateTime += fTime;
			float* pMoverV2Pos =  m_pCurrentMover->GetV2PosByTime(m_fAccumulateTime);
			m_vPos[0] = pMoverV2Pos[0];
			m_vPos[1] = pMoverV2Pos[1];

//			LOGE("DEBUG pbMarionette::MoveUpdate ");
		}
//		else
//			LOGE("pbMarionette::Translate() CurrentState is NULL");
	}
}

void pbMarionette::Translate() {
	glTranslatef(m_vPos[0], m_vPos[1], 0);
}
