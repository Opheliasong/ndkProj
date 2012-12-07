/*
 * pbTouchItem.cpp
 *
 *  Created on: 2012. 12. 7.
 *      Author: NitroSoft
 */

#include "pbItem.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------		pbItem			------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pbItem::pbItem() {
	m_pItemDrawUnit = new pbBasicDrawUnit();
	m_fItemY = 0.0f;
	m_pDescriptionDrawUnit = new pbBasicDrawUnit();
	m_fDescriptionY = 0.0f;

	m_PurchaseState = ITEM_PURCHASE_AVAILABLE;
	m_SelectState = ITEM_SELECT_NONE;
	m_Price = 0;

	TouchLayer::GetInstance().RegistedObserver(this);
}
pbItem::~pbItem() {
	TouchLayer::GetInstance().RemovedObserver(this);
	delete m_pItemDrawUnit;
	delete m_pDescriptionDrawUnit;
}

void pbItem::SetItemTag(screenplayTag Tag, float fWidth, float fHeight) {
	m_pItemDrawUnit->SetTextureTAG(Tag);
	m_pItemDrawUnit->SetSize(fWidth, fHeight);

	m_fItemY = fHeight/2;
}

void pbItem::SetDescriptionTag(screenplayTag Tag, float fWidth, float fHeight) {
	m_pDescriptionDrawUnit->SetTextureTAG(Tag);
	m_pDescriptionDrawUnit->SetSize(fWidth, fHeight);

	m_fDescriptionY = fHeight/2;
}

void pbItem::PriceCheck(int gold) {
	if( m_Price <= gold)
		m_PurchaseState = ITEM_PURCHASE_AVAILABLE;
	else
		m_PurchaseState = ITEM_PURCHASE_IMPOSIBLE;
}

bool pbItem::IsTouched() {
	int x = TouchLayer::GetInstance().pointX;
	int y = TouchLayer::GetInstance().pointY;

	int HalfWidth = m_pItemDrawUnit->getWidth()/2;
	int HalfHeight =m_pItemDrawUnit->getHeight()/2;

	int left = m_vPos[0] - HalfWidth;
	int right = m_vPos[0] + HalfWidth;
	int top = m_vPos[1] + HalfHeight;
	int bottom = m_vPos[1] - HalfHeight;

	if (x >= left && x <= right) {
		if (y >= bottom && y <= top) {
			LOGE("[DEBUG]pbItem_Potion:: Touched");
			return true;
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------		pbItem_Potion			------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pbItem_Potion::pbItem_Potion() {

}

pbItem_Potion::~pbItem_Potion() {

}

void pbItem_Potion::PreSettingDraw() {
	glPushMatrix();
	glTranslatef(m_vPos[0], m_vPos[1], 0.0f);
}

void pbItem_Potion::DrawThis() {
		/////----------------------아이템-----------------------------////
		if( m_PurchaseState == ITEM_PURCHASE_IMPOSIBLE )
			glColor4f(1.0f, 0.2f, 0.2f, 1.0f);
		else if( m_PurchaseState == ITEM_PURCHASE_LOCKED )
			glColor4f(0.1f, 0.1f, 0.1f, 1.0f);

		glPushMatrix();
		m_pItemDrawUnit->PreSettingDraw();
		m_pItemDrawUnit->DrawThis();
		glPopMatrix();

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		/////---------------설명---------------------------------////
		if( m_SelectState == ITEM_SELECT_ONE) {
			glPushMatrix();
			glTranslatef(0, m_fItemY+m_fDescriptionY, 0.0f);
			m_pDescriptionDrawUnit->PreSettingDraw();
			m_pDescriptionDrawUnit->DrawThis();
			glPopMatrix();
		}


	glPopMatrix();
}

void pbItem_Potion::notify() {
	if(TouchLayer::GetInstance().touchFlag == TOUCHFLAGS::TAPDOWN) {
		if( IsTouched() ) {
			if( m_SelectState == ITEM_SELECT_NONE) {
				m_SelectState = ITEM_SELECT_ONE;
			}
			else if( m_SelectState == ITEM_SELECT_ONE) {
				if( m_PurchaseState == ITEM_PURCHASE_AVAILABLE ) {
					// TODO:: 구매
					//PriceCheck(10);	//테스트 코드
					SetPurchaseState(ITEM_PURCHASE_LOCKED);
				}// end if PURCHASE STATE
			}//end if touchcount

		}// end if isTouched
		else
			m_SelectState = ITEM_SELECT_NONE;
	}// end if TOUCHFLAGS
}
