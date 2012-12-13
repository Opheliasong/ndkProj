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


void pbInventory::AddItem(ItemCode Code) {
	if( m_ItemMap.count(Code) > 0) {//아이템 존재하면 더함
		m_ItemMap[Code]++;

		LOGE("Change Inventory %s : %d", Code.c_str(), m_ItemMap[Code]);

		return;
	}
	else {	// 존재하지않으면 새로 만듬
		m_ItemMap.insert(ItemMap::value_type(Code, 1));

		LOGE("Add Inventory %s : %d", Code.c_str(), m_ItemMap[Code]);
	}
}

int pbInventory::GetItemCodeValue(ItemCode Code) {
	if( GetInstance().m_ItemMap.count(Code) > 0)
		return GetInstance().m_ItemMap[Code];

	return -1;
}

void pbInventory::SetMountedVehicle(ItemCode Code) {
	m_MountedCode = Code;
	pbVehicle::GetInstance().ChangeVehicle(Code);
	LOGE("Mount Vehicle : %s", m_MountedCode.c_str());
}

void pbInventory::DataTransferToStageValue() {
	pbStageValue::GetStatusWeight().InitValue(0, 0.0f, 0.0f);
	////----------------------포션류 데이터 전송--------------------------------------//
	int count;
	count = GetItemCodeValue(ITEMCODE_POTION_LIFE);
	if( count != -1 )
		pbStageValue::CalcLifeWeight( count );

	count = GetItemCodeValue(ITEMCODE_POTION_FEVERPOINT);
	if( count != -1 )
		pbStageValue::CalcItemFeverPointWeight(count );

	count = GetItemCodeValue(ITEMCODE_POTION_SPEEDDOWN);
	if( count != -1 )
		pbStageValue::CalcItemSpeedWeight(count );

	count = GetItemCodeValue(ITEMCODE_POTION_SCOREPLUS);
	if( count != -1 )
		pbStageValue::CalcItemScoreWeight(count );
	////----------------------장착된 탈것 데이터 전송--------------------------------------//
	// 탈것들의 능력치를 저장& 모아놓은 클래스를 생성
	// 클래스에 아이템 코드를 전송하면 구조체를 받아와서 능력치를 추가한다.
	pbStatusWeights& refWeights = pbVehicle::GetInstance().GetVehicleData(m_MountedCode);

	pbStageValue::GetStatusWeight().PlusStatusWeight(refWeights);
	pbStageValue::PrintWeightLog();
}

void pbInventory::LoadData() {
	//TODO: XML Parsing

	AddItem(ITEMCODE_VEHICLE_DEFAULT);
	SetMountedVehicle(ITEMCODE_VEHICLE_DEFAULT);
}

void pbInventory::ClearData() {
	m_ItemMap.clear();
}

pbInventory& pbInventory::GetInstance() {
	static pbInventory SingleTon;
	return SingleTon;
}
