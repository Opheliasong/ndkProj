/*
 * pbGoldPouch.h
 *
 *  Created on: 2012. 12. 7.
 *      Author: NitroSoft
 */

#ifndef PBGOLDPOUCH_H_
#define PBGOLDPOUCH_H_

#include "../../stdafx.h"

class pbGoldPouch{
public:
	pbGoldPouch();
	~pbGoldPouch();

	void SetGold(int Gold) { m_Gold = Gold;}
	int GetGold() { return m_Gold;}

	void IncreaseGold(int GoldValue);
	bool DecreaseGold(int GoldValue);

	static int CalcGold();

	static pbGoldPouch& GetInstance();
private:
	int m_Gold;
	bool m_bGoldChange;
};



#endif /* PBGOLDPOUCH_H_ */

