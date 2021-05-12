/*
 * pbInventory.h
 *
 *  Created on: 2012. 12. 10.
 *      Author: NitroSoft
 */

#ifndef PBINVENTORY_H_
#define PBINVENTORY_H_

#include "../../stdafx.h"

class pbInventory {
public:
	pbInventory();
	~pbInventory();

	void AddItem(ItemCode Code);

	void SetMountedVehicle(ItemCode Code);

	static int GetItemCodeValue(ItemCode Code);
	ItemCode GetMountedItemCode() { return m_MountedCode;}

	void DataTransferToStageValue();

	void LoadData();
	void ClearData();

	static pbInventory& GetInstance();
private:
	typedef std::map<ItemCode, int> ItemMap;
	ItemMap m_ItemMap;
	ItemCode m_MountedCode;
};


#endif /* PBINVENTORY_H_ */
