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

void pbShop::AddItem_Vehicle(float X, float Y, TAGDATA& ItemTagData, TAGDATA& DescriptionTagData, ItemCode Code, int Price) {
	pbItem_Vehicle* createItem = new pbItem_Vehicle();
	createItem->SetItemTag(ItemTagData);
	createItem->SetDescriptionTag(DescriptionTagData);
	createItem->SetItemCode(Code);
	createItem->SetPos(X, Y);
	createItem->SetPrice(Price);
	createItem->VehicleStateCheck();

	RegistItem(createItem);

}

void pbShop::AddItem_Pass(float X, float Y, TAGDATA& ItemTagData, TAGDATA& DescriptionTagData, int Level, int Price) {
	pbItem_Pass* createItem = new pbItem_Pass();
	createItem->SetItemTag(ItemTagData);
	createItem->SetDescriptionTag(DescriptionTagData);
	createItem->SetDestLevel(Level);
	createItem->SetPos(X, Y);
	createItem->SetPrice(Price);

	RegistItem(createItem);

}


void pbShop::LoadData() {
	TAGDATA ItemTagData, DescriptionTagData;
	int Price;

	//테스트 : 골드 계속 리셋
	pbGoldPouch::GetInstance().SetGold(500);



	//TODO: XML 파싱
	int ShopLevel = pbStageValue::GetShopLevel();
	if( ShopLevel == 0) {
		///---------------------------------------패스-------------------------------------------------------//
		ItemTagData.SetData("run", 80, 80);
		DescriptionTagData.SetData("ci", 150, 150);
		Price = 250;
		AddItem_Pass(670, 380, ItemTagData, DescriptionTagData, 1, Price);
		///---------------------------------------포션-------------------------------------------------------//
		ItemTagData.SetData("item01", 80, 80);
		DescriptionTagData.SetData("ci", 150, 150);
		Price = 10;
		AddItem_Potion(100, 400, ItemTagData, DescriptionTagData, ITEMCODE_POTION_LIFE, Price);

		ItemTagData.SetData("item02", 80, 80);
		DescriptionTagData.SetData("ci", 150, 150);
		Price = 50;
		AddItem_Potion(200, 400, ItemTagData, DescriptionTagData, ITEMCODE_POTION_FEVERPOINT, Price);

		ItemTagData.SetData("item03", 80, 80);
		DescriptionTagData.SetData("ci", 150, 150);
		Price = 110;
		AddItem_Potion(100, 300, ItemTagData, DescriptionTagData, ITEMCODE_POTION_SPEEDDOWN, Price);

		ItemTagData.SetData("item04", 80, 80);
		DescriptionTagData.SetData("ci", 150, 150);
		Price = 250;
		AddItem_Potion(200, 300, ItemTagData, DescriptionTagData, ITEMCODE_POTION_SCOREPLUS, Price);

		///---------------------------------------탈것-------------------------------------------------------//
		ItemTagData.SetData("airframe01", 115, 80);
		DescriptionTagData.SetData("ci", 150, 150);
		Price = 150;
		AddItem_Vehicle(80, 100, ItemTagData, DescriptionTagData, ITEMCODE_VEHICLE_DEFAULT, Price);

		ItemTagData.SetData("airframe02", 115, 80);
		DescriptionTagData.SetData("ci", 150, 150);
		Price = 150;
		AddItem_Vehicle(200, 100, ItemTagData, DescriptionTagData, ITEMCODE_VEHICLE_NO_1, Price);

		ItemTagData.SetData("airframe03", 115, 80);
		DescriptionTagData.SetData("ci", 150, 150);
		Price = 150;
		AddItem_Vehicle(320, 100, ItemTagData, DescriptionTagData, ITEMCODE_VEHICLE_NO_2, Price);
	}
	else if( ShopLevel == 1) {
		///---------------------------------------패스-------------------------------------------------------//
		ItemTagData.SetData("run", 80, 80);
		DescriptionTagData.SetData("ci", 150, 150);
		Price = 250;
		AddItem_Pass(670, 380, ItemTagData, DescriptionTagData, 0, Price);
		///---------------------------------------포션-------------------------------------------------------//
		ItemTagData.SetData("run", 80, 80);
		DescriptionTagData.SetData("ci", 150, 150);
		Price = 250;
		AddItem_Potion(200, 400, ItemTagData, DescriptionTagData, ITEMCODE_POTION_FEVERPOINT, Price);

		ItemTagData.SetData("run", 80, 80);
		DescriptionTagData.SetData("ci", 150, 150);
		Price = 510;
		AddItem_Potion(100, 300, ItemTagData, DescriptionTagData, ITEMCODE_POTION_SPEEDDOWN, Price);

		ItemTagData.SetData("run", 80, 80);
		DescriptionTagData.SetData("ci", 150, 150);
		Price = 550;
		AddItem_Potion(200, 300, ItemTagData, DescriptionTagData, ITEMCODE_POTION_SCOREPLUS, Price);

	}
}

void pbShop::ClearData() {
	ItemVector::iterator Iter = m_ItemPointerVector.begin();
	while( Iter != m_ItemPointerVector.end() ) {
		ItemVector::iterator NextIter = Iter+1;
		delete (*Iter);
		Iter = NextIter;
	}
	m_ItemPointerVector.clear();

	LOGE("pbShop::ClearData() : Complete");
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
