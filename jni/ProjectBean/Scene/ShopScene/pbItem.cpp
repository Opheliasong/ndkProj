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
	m_ItemCode.clear();
	TouchLayer::GetInstance().RegistedObserver(this);
}
pbItem::~pbItem() {
	TouchLayer::GetInstance().RemovedObserver(this);
	delete m_pItemDrawUnit;
	delete m_pDescriptionDrawUnit;
}

void pbItem::SetItemTag(TAGDATA& TagData) {
	m_pItemDrawUnit->SetTextureTAG(TagData.Tag);
	m_pItemDrawUnit->SetSize(TagData.fWidth, TagData.fHeight);

	m_fItemY = TagData.fHeight/2;
}

void pbItem::SetDescriptionTag(TAGDATA& TagData) {
	m_pDescriptionDrawUnit->SetTextureTAG(TagData.Tag);
	m_pDescriptionDrawUnit->SetSize(TagData.fWidth, TagData.fHeight);

	m_fDescriptionY = TagData.fHeight/2;
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
//			LOGE("[DEBUG]pbItem_Potion:: Touched");
			return true;
		}
	}

	return false;
}


void pbItem::notify() {
	if(TouchLayer::GetInstance().touchFlag == TOUCHFLAGS::TAPDOWN) {
		if( IsTouched() ) {
			if( m_SelectState == ITEM_SELECT_NONE) {
				SelectNone();
			}
			else if( m_SelectState == ITEM_SELECT_ONE) {
				SelectOne();
			}//end if touchcount

		}// end if isTouched
		else {
			m_SelectState = ITEM_SELECT_NONE;
		}

	}// end if TOUCHFLAGS
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

void pbItem_Potion::SelectNone() {
	m_SelectState = ITEM_SELECT_ONE;
}

void pbItem_Potion::SelectOne() {
	if (m_PurchaseState == ITEM_PURCHASE_AVAILABLE) {
		//구매 시도
		if (pbGoldPouch::GetInstance().DecreaseGold(m_Price)) {
			// 성공
			pbShop::GetInstance().ChangedItem();

			pbInventory::GetInstance().AddItem(m_ItemCode);
		}
	}
}

void pbItem_Potion::notify() {
	pbItem::notify();
/*
	if(TouchLayer::GetInstance().touchFlag == TOUCHFLAGS::TAPDOWN) {
		if( IsTouched() ) {
			if( m_SelectState == ITEM_SELECT_NONE) {
				SelectNone();
			}
			else if( m_SelectState == ITEM_SELECT_ONE) {
				SelectOne();
			}//end if touchcount

		}// end if isTouched
		else {
			m_SelectState = ITEM_SELECT_NONE;
		}

	}// end if TOUCHFLAGS
*/
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------		pbItem_Potion			------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pbItem_Vehicle::pbItem_Vehicle() {
	m_VehicleState = VEHICLE_PURCHASE_IS_REQUIRED;
	m_pOwnCaption = new pbBasicDrawUnit();

	m_pOwnCaption->SetTextureTAG("ci");
	m_pOwnCaption->SetSize(70, 70);
}

pbItem_Vehicle::~pbItem_Vehicle() {
	delete m_pOwnCaption;
}

void pbItem_Vehicle::PreSettingDraw() {
	glPushMatrix();
	glTranslatef(m_vPos[0], m_vPos[1], 0.0f);
}

void pbItem_Vehicle::DrawThis() {
		/////----------------------아이템-----------------------------////
		if( m_VehicleState == VEHICLE_PURCHASE_IS_REQUIRED) {	// 장비 안샀을때 구매관련 표시
			if( m_PurchaseState == ITEM_PURCHASE_IMPOSIBLE )
				glColor4f(1.0f, 0.2f, 0.2f, 1.0f);
			else if( m_PurchaseState == ITEM_PURCHASE_LOCKED )
				glColor4f(0.1f, 0.1f, 0.1f, 1.0f);
		}

		glPushMatrix();
		m_pItemDrawUnit->PreSettingDraw();
		m_pItemDrawUnit->DrawThis();
		glPopMatrix();

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		//소유중 마크 표시
		if( m_VehicleState == VEHICLE_CHANGE_ABLE) {
			glPushMatrix();
			m_pOwnCaption->PreSettingDraw();
			m_pOwnCaption->DrawThis();
			glPopMatrix();
		}

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

void pbItem_Vehicle::SelectNone() {
	m_SelectState = ITEM_SELECT_ONE;

	if (m_VehicleState == VEHICLE_CHANGE_ABLE) {
		VehicleChange();
	}
}

void pbItem_Vehicle::SelectOne() {
	if (m_VehicleState == VEHICLE_PURCHASE_IS_REQUIRED) {
		if (m_PurchaseState == ITEM_PURCHASE_AVAILABLE) {
			//구매 시도
			if (pbGoldPouch::GetInstance().DecreaseGold(m_Price)) {
				// 성공
				pbShop::GetInstance().ChangedItem();

				pbInventory::GetInstance().AddItem(m_ItemCode);

				VehicleStateCheck();
				VehicleChange();
			}

		}// end if PURCHASE STATE

	}// end if VehicleState
}

void pbItem_Vehicle::VehicleChange() {
	pbInventory::GetInstance().SetMountedVehicle(m_ItemCode);
}

void pbItem_Vehicle::VehicleStateCheck(){
	 if( pbInventory::GetItemCodeValue(m_ItemCode) != -1)
		 m_VehicleState = VEHICLE_CHANGE_ABLE;
	 else
		 m_VehicleState = VEHICLE_PURCHASE_IS_REQUIRED;
}

void pbItem_Vehicle::SetCaptionTagData(TAGDATA& TagData){
	m_pOwnCaption->SetTextureTAG(TagData.Tag);
	m_pOwnCaption->SetSize(TagData.fWidth, TagData.fHeight);

}

void pbItem_Vehicle::notify() {
	pbItem::notify();
/*
	if(TouchLayer::GetInstance().touchFlag == TOUCHFLAGS::TAPDOWN) {
		if( IsTouched() ) {
			if( m_SelectState == ITEM_SELECT_NONE) {
				SelectNone();
			}
			else if( m_SelectState == ITEM_SELECT_ONE) {
				SelectOne();
			}//end if touchcount

		}// end if isTouched
		else {
			m_SelectState = ITEM_SELECT_NONE;
		}

	}// end if TOUCHFLAGS
*/
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------		pbItem_Pass		------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pbItem_Pass::pbItem_Pass() {
	m_DestLevel = 0;
}

pbItem_Pass::~pbItem_Pass() {

}

void pbItem_Pass::PreSettingDraw() {
	glPushMatrix();
	glTranslatef(m_vPos[0], m_vPos[1], 0.0f);
}

void pbItem_Pass::DrawThis() {
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

void pbItem_Pass::SelectNone() {
	m_SelectState = ITEM_SELECT_ONE;
}

void pbItem_Pass::SelectOne() {
	if (m_PurchaseState == ITEM_PURCHASE_AVAILABLE) {
		//구매 시도
		if (pbGoldPouch::GetInstance().DecreaseGold(m_Price)) {
			// 성공
			pbShop::GetInstance().ChangedItem();

			//TODO:: 샵 레벨 올림, 네비게이터 작동
			pbStageValue::SetShopLevel(m_DestLevel);
			pbSceneNavigator::GetInstance().SearchAndReadyToMoveScene(SCENESTATE::ACTION_RECURSIVE);
		}
	}
}

void pbItem_Pass::notify() {
	pbItem::notify();
/*
	if(TouchLayer::GetInstance().touchFlag == TOUCHFLAGS::TAPDOWN) {
		if( IsTouched() ) {
			if( m_SelectState == ITEM_SELECT_NONE) {
				SelectNone();
			}
			else if( m_SelectState == ITEM_SELECT_ONE) {
				SelectOne();
			}//end if touchcount

		}// end if isTouched
		else {
			m_SelectState = ITEM_SELECT_NONE;
		}

	}// end if TOUCHFLAGS
*/
}
