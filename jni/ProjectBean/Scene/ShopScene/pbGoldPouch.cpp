/*
 * pbGoldPouch.cpp
 *
 *  Created on: 2012. 12. 7.
 *      Author: NitroSoft
 */

#include "pbGoldPouch.h"

pbGoldPouch& pbGoldPouch::GetInstance() {
	static pbGoldPouch SingleTon;
	return SingleTon;
}

pbGoldPouch::pbGoldPouch() {
	m_Gold = 0.0;
	m_bGoldChange = false;
}
pbGoldPouch::~pbGoldPouch() {

}

void pbGoldPouch::IncreaseGold(int GoldValue) {
	m_Gold += GoldValue;
	m_bGoldChange = true;
}

bool pbGoldPouch::DecreaseGold(int GoldValue) {
	if( m_Gold - GoldValue >= 0) {
		m_Gold -= GoldValue;
		m_bGoldChange = true;
		return true;
	}

	return false;
}

int pbGoldPouch::CalcGold() {
	if( GetInstance().m_bGoldChange)
		return GetInstance().m_Gold;

	return -1;
}
