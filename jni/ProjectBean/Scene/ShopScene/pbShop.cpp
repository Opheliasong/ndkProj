/*
 * pbShop.cpp
 *
 *  Created on: 2012. 12. 10.
 *      Author: NitroSoft
 */

#include "pbShop.h"

pbShop::pbShop() {
	m_ItemPointerVector.reserve(10);
	m_bChangedItems = false;
}
pbShop::~pbShop() {
	ClearData();
}

void pbShop::RegistItem(pbItem* pItem) {
	m_ItemPointerVector.push_back(pItem);
}
void pbShop::RemoveItem(pbItem* pItem) {
	ItemVector::iterator iter = std::find(m_ItemPointerVector.begin(), m_ItemPointerVector.end(), pItem);
	delete (*iter);
	m_ItemPointerVector.erase(iter);
}


void pbShop::AddItem_Potion(float X, float Y, TAGDATA& ItemTagData, TAGDATA& DescriptionTagData, ItemCode Code, int Price) {
	pbItem_Potion* createItem = new pbItem_Potion();
	createItem->SetItemTag(ItemTagData);
	createItem->SetDescriptionTag(DescriptionTagData);
	createItem->SetItemCode(Code);
	createItem->SetPos(X, Y);
	createItem->SetPrice(Price);

	RegistItem(createItem);
}

/*
void pbShop::AddItem_Vehicle(float X, float Y, screenplayTag Tag, float fWidth, float fHeight, int ItemCode) {

}
*/


void pbShop::LoadData() {
	//TODO: XML 파싱


	//계속 리셋
	pbGoldPouch::GetInstance().SetGold(500);

	//임시 아이템 추가 코드
	TAGDATA ItemTagData, DescriptionTagData;
	int Price;

	ItemTagData.SetData("run", 80, 80);
	DescriptionTagData.SetData("ci", 150, 150);
	Price = 10;
	AddItem_Potion(100, 400, ItemTagData, DescriptionTagData, ITEMCODE_LIFE, Price);

	ItemTagData.SetData("run", 80, 80);
	DescriptionTagData.SetData("ci", 150, 150);
	Price = 50;
	AddItem_Potion(200, 400, ItemTagData, DescriptionTagData, ITEMCODE_FEVERPOINT, Price);

	ItemTagData.SetData("run", 80, 80);
	DescriptionTagData.SetData("ci", 150, 150);
	Price = 110;
	AddItem_Potion(100, 300, ItemTagData, DescriptionTagData, ITEMCODE_SPEEDDOWN, Price);

	ItemTagData.SetData("run", 80, 80);
	DescriptionTagData.SetData("ci", 150, 150);
	Price = 250;
	AddItem_Potion(200, 300, ItemTagData, DescriptionTagData, ITEMCODE_SCOREPLUS, Price);
}

void pbShop::ClearData() {
	ItemVector::iterator Iter = m_ItemPointerVector.begin();
	for( ;Iter != m_ItemPointerVector.end(); Iter++ ) {
		ItemVector::iterator NextIter = Iter+1;
		delete (*Iter);
		Iter = NextIter;
	}
	m_ItemPointerVector.clear();
}

void pbShop::PreSettingDraw() {
	glPushMatrix();
	glTranslatef(m_vPos[0], m_vPos[1], 0.0f);
}
void pbShop::DrawThis() {
	ItemVector::iterator Iter = m_ItemPointerVector.begin();
	for( ;Iter != m_ItemPointerVector.end(); Iter++ ) {
		pbItem* pItem = (*Iter);
		pItem->PreSettingDraw();
		pItem->DrawThis();
	}
	glPopMatrix();
}

pbShop& pbShop::GetInstance() {
	static pbShop SingleTon;

	return SingleTon;
}

void pbShop::Update(float fTime) {
	if( GetChangedItem() ) {
		ItemVector::iterator Iter = m_ItemPointerVector.begin();
		for( ;Iter != m_ItemPointerVector.end(); Iter++ ) {
			pbItem* pItem = (*Iter);
			pItem->PriceCheck(pbGoldPouch::GetInstance().GetGold());
		}
		ResetChangedItem();
	}

}
