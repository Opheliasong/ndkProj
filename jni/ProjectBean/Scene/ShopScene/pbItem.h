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

	void SetItemTag(screenplayTag Tag, float fWidth, float fHeight);
	void SetDescriptionTag(screenplayTag Tag, float fWidth, float fHeight);

	void SetPos(float X, float Y) { m_vPos[0] = X; m_vPos[1] = Y; }
	void SetPurchaseState(int State) { m_PurchaseState = State; }
	void SetSelectState(int State) { m_SelectState = State; }
	void SetPrice(int Price) { m_Price = Price; }

	void PriceCheck(int gold);
	bool IsTouched();

	virtual void PreSettingDraw() = 0;
	virtual void DrawThis() = 0;

	virtual void notify() = 0;

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
private:
};

#endif /* PBITEM_H_ */
