/*
 * pbInventory.cpp
 *
 *  Created on: 2012. 12. 10.
 *      Author: NitroSoft
 */

#include "pbInventory.h"

pbInventory::pbInventory() {

}
pbInventory::~pbInventory() {
	ClearData();
}


void pbInventory::AddItemStatus(ItemCode Code) {
	m_StatusMap.insert(StatusMap::value_type(Code, 0));

	LOGE("Add Inventory %s : %d", Code.c_str(), m_StatusMap[Code]);
}

void pbInventory::PlusItemCount(ItemCode Code, int Count) {
	if( m_StatusMap.count(Code) > 0) {
		m_StatusMap[Code] += Count;

		LOGE("Change Inventory %s : %d", Code.c_str(), m_StatusMap[Code]);

		return;
	}

	LOGE("pbInventory::PlusItemStatus() : Not Exist ItemCode");
}

int pbInventory::GetItemCodeValue(ItemCode Code) {
	if( GetInstance().m_StatusMap.count(Code) > 0)
		return GetInstance().m_StatusMap[Code];

	return -1;
}

void pbInventory::DataTransferToStageValue() {
	int count;
	count = GetItemCodeValue(ITEMCODE_LIFE);
	if( count != -1 )
		pbStageValue::SetLifeWeight( count );

	count = GetItemCodeValue(ITEMCODE_FEVERPOINT);
	if( count != -1 )
		pbStageValue::SetFeverPoint( count );

	count = GetItemCodeValue(ITEMCODE_SPEEDDOWN);
	if( count != -1 )
		pbStageValue::SetSpeedWeight( count );

	count = GetItemCodeValue(ITEMCODE_SCOREPLUS);
	if( count != -1 )
		pbStageValue::SetScoreWeight( count );

}

void pbInventory::LoadData() {
	//TODO: XML Parsing

	//임시 추가 코드
	AddItemStatus(ITEMCODE_LIFE);
	AddItemStatus(ITEMCODE_FEVERPOINT);
	AddItemStatus(ITEMCODE_SPEEDDOWN);
	AddItemStatus(ITEMCODE_SCOREPLUS);
}

void pbInventory::ClearData() {
	m_StatusMap.clear();
}

pbInventory& pbInventory::GetInstance() {
	static pbInventory SingleTon;
	return SingleTon;
}
