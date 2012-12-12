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

	void AddItemStatus(ItemCode Code);
	void PlusItemCount(ItemCode Code, int Count);

	static int GetItemCodeValue(ItemCode Code);

	void DataTransferToStageValue();

	void LoadData();
	void ClearData();

	static pbInventory& GetInstance();
private:
	typedef std::map<ItemCode, int> StatusMap;
	StatusMap m_StatusMap;
};


#endif /* PBINVENTORY_H_ */
