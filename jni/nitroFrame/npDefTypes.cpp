/*
 * npDefTypes.cpp
 *
 *  Created on: 2012. 10. 24.
 *      Author: Ophelia
 */

#include "npDefTypes.h"

//=======================================================================///
void AREA::Init(float PosX, float PosY, int Width, int Height)
{
	m_vPos[0] = PosX;
	m_vPos[1] = PosY;
	m_HalfWH[0] = Width/2;
	m_HalfWH[1] = Height/2;
	m_StandY = Height/2;
}

