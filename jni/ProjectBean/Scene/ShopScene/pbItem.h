/*
 * pbItem.h
 *
 *  Created on: 2012. 12. 7.
 *      Author: NitroSoft
 */

#ifndef PBITEM_H_
#define PBITEM_H_

#include "../../stdafx.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------		pbItem			------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class pbItem : public iTouchObserver, public npDrawable{
public:
	pbItem();
	virtual ~pbItem();

	void SetItemTag(TAGDATA& TagData);
	void SetDescriptionTag(TAGDATA& TagData);

	void SetPos(float X, float Y) { m_vPos[0] = X; m_vPos[1] = Y; }

	void SetPurchaseState(int State) { m_PurchaseState = State; }
	void SetSelectState(int State) { m_SelectState = State; }
	void SetPrice(int Price) { m_Price = Price; }
	void SetItemCode(ItemCode Code) { m_ItemCode = Code; }

	ItemCode GetItemCode() {return m_ItemCode;}

	void PriceCheck(int gold);
	bool IsTouched();

	virtual void PreSettingDraw() = 0;
	virtual void DrawThis() = 0;

	virtual void notify() = 0;

	virtual void SelectNone() = 0;
	virtual void SelectOne() = 0;

	enum { ITEM_PURCHASE_AVAILABLE = 0, ITEM_PURCHASE_IMPOSIBLE, ITEM_PURCHASE_LOCKED};
	enum { ITEM_SELECT_NONE = 0, ITEM_SELECT_ONE};
protected:
	pbBasicDrawUnit* m_pItemDrawUnit;
	float m_fItemY;

	pbBasicDrawUnit* m_pDescriptionDrawUnit;
	float m_fDescriptionY;
	int m_SelectState;

	npV2Vector m_vPos;
	int m_PurchaseState;
	int m_Price;

	ItemCode m_ItemCode;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------		pbItem_Potion			------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class pbItem_Potion : public pbItem{
public:
	pbItem_Potion();
	virtual ~pbItem_Potion();

	virtual void PreSettingDraw();
	virtual void DrawThis();

	virtual void notify();

	virtual void SelectNone();
	virtual void SelectOne();
private:
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------		pbItem_Vehicle			------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class pbItem_Vehicle : public pbItem{
public:
	pbItem_Vehicle();
	virtual ~pbItem_Vehicle();

	virtual void PreSettingDraw();
	virtual void DrawThis();

	virtual void notify();

	virtual void SelectNone();
	virtual void SelectOne();

	void VehicleStateCheck();
	void VehicleChange();

	void SetCaptionTagData(TAGDATA& TagData);

	//void SetStatePurchase(int State) {m_VehicleState = State; }

	enum { VEHICLE_PURCHASE_IS_REQUIRED = 0, VEHICLE_CHANGE_ABLE};
private:
	int m_VehicleState;

	pbBasicDrawUnit* m_pOwnCaption;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////-----------------------------------------------------		pbItem_Pass			------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class pbItem_Pass : public pbItem{
public:
	pbItem_Pass();
	virtual ~pbItem_Pass();

	virtual void PreSettingDraw();
	virtual void DrawThis();

	virtual void notify();

	virtual void SelectNone();
	virtual void SelectOne();

	void SetDestLevel(int Level) { m_DestLevel = Level; }
private:
	int m_DestLevel;
};

#endif /* PBITEM_H_ */
