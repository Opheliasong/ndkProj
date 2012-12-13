/*
 * pbVehicle.cpp
 *
 *  Created on: 2012. 12. 12.
 *      Author: NitroSoft
 */

#include "pbVehicle.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////----------------------------------------------------		pbVehicle		------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pbVehicle& pbVehicle::GetInstance() {
	static pbVehicle SingleTon;
	return SingleTon;
}

pbVehicle::pbVehicle() {

}
pbVehicle::~pbVehicle() {
	ClearData();
}

void pbVehicle::LoadData() {
	TAGDATA VehicleTagData; pbStatusWeights Weights;
	VehicleTagData.SetData("ci", 150, 20);
	Weights.InitValue(0, 0.0f, 0.0f);
	AddVehicleData(ITEMCODE_VEHICLE_DEFAULT, VehicleTagData, Weights);

	VehicleTagData.SetData("ci", 200, 30);
	Weights.InitValue(500, -0.05f, 0.0f);
	AddVehicleData(ITEMCODE_VEHICLE_NO_1, VehicleTagData, Weights);

	VehicleTagData.SetData("ci", 300, 40);
	Weights.InitValue(12, 0.0f, 15.2f);
	AddVehicleData(ITEMCODE_VEHICLE_NO_2, VehicleTagData, Weights);
}

void pbVehicle::AddVehicleData(ItemCode Code, TAGDATA& TagData, pbStatusWeights& Weights) {
	m_VehicleMap.insert(VehicleDataMap::value_type(Code, VehicleData::pair(TagData, Weights) ) );
}

void pbVehicle::ChangeVehicle(ItemCode Code) {
	if( m_VehicleMap.count(Code) > 0) {
		pbCharacter::GetInstance()->SetVehicleTagData(m_VehicleMap[Code].first);
	}
}

pbStatusWeights& pbVehicle::GetVehicleData(ItemCode Code) {
	static pbStatusWeights Weights;

	if( m_VehicleMap.count(Code) > 0) {
		return m_VehicleMap[Code].second;
	}

	return Weights;
}

void pbVehicle::ClearData() {
	m_VehicleMap.clear();
}
