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

	void AddItem_Potion(float X, float Y,TAGDATA& ItemTagData, TAGDATA& DescriptionTagData, int ItemCode, int Price);
//	void AddItem_Vehicle(float X, float Y, screenplayTag Tag, float fWidth, float fHeight, int ItemCode);

	void LoadData();
	void ClearData();

	virtual void PreSettingDraw();
	virtual void DrawThis();

	void SetPos(float X, float Y) {m_vPos[0] = X; m_vPos[1] = Y; }



	static pbShop& GetInstance();
private:
	npV2Vector m_vPos;

	typedef std::vector<pbItem*> ItemVector;
	ItemVector m_ItemPointerVector;
};



#endif /* PBSHOP_H_ */
