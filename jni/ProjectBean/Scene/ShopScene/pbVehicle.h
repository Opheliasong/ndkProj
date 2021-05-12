/*
 * pbVehicle.h
 *
 *  Created on: 2012. 12. 12.
 *      Author: NitroSoft
 */

#ifndef PBVEHICLE_H_
#define PBVEHICLE_H_

#include "../../stdafx.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////----------------------------------------------------		pbVehicle		------------------------------------------------------------------------------///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// 탈것의 그림정보와 능력치 정보를 가짐
//// 캐릭터와 통신 ( 그림정보)
//// 인벤토리와 통신 ( 능력치 정보)

class pbVehicle {
public:
	pbVehicle();
	~pbVehicle();

	void LoadData();

	void ChangeVehicle(ItemCode Code);

	void AddVehicleData(ItemCode Code, TAGDATA& TagData, pbStatusWeights& Weights);

	pbStatusWeights& GetVehicleData(ItemCode Code);

	void ClearData();

	static pbVehicle& GetInstance();
private:
	typedef std::pair<TAGDATA, pbStatusWeights> VehicleData;
	typedef std::map<ItemCode, VehicleData> VehicleDataMap;
	VehicleDataMap m_VehicleMap;
};


#endif /* PBVEHICLE_H_ */
