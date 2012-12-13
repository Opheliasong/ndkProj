/*
 * pbShop.h
 *
 *  Created on: 2012. 12. 10.
 *      Author: NitroSoft
 */

#ifndef PBSHOP_H_
#define PBSHOP_H_

#include "../../stdafx.h"

class pbItem;
class pbShop : public npDrawable{
	void RegistItem(pbItem* pItem);
	void RemoveItem(pbItem* pItem);
public:
	pbShop();
	~pbShop();

	void AddItem_Potion(float X, float Y, TAGDATA& ItemTagData, TAGDATA& DescriptionTagData, ItemCode Code, int Price);
	void AddItem_Vehicle(float X, float Y, TAGDATA& ItemTagData, TAGDATA& DescriptionTagData, ItemCode Code, int Price);
	void AddItem_Pass(float X, float Y, TAGDATA& ItemTagData, TAGDATA& DescriptionTagData, int Level, int Price);
	void LoadData();
	void ClearData();

	void ChangedItem() { m_bChangedItems = true; }
	void ResetChangedItem() { m_bChangedItems = false; }
	bool GetChangedItem() { return m_bChangedItems; }

	virtual void PreSettingDraw();
	virtual void DrawThis();

	void SetPos(float X, float Y) {m_vPos[0] = X; m_vPos[1] = Y; }

	void Update(float fTime);

	static pbShop& GetInstance();
private:
	npV2Vector m_vPos;
	bool m_bChangedItems;

	typedef std::vector<pbItem*> ItemVector;
	ItemVector m_ItemPointerVector;
};



#endif /* PBSHOP_H_ */
