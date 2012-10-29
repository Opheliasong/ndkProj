/*
 * npMath.cpp
 *
 *  Created on: 2012. 10. 21.
 *      Author: Ophelia
 */

#include "npMath.h"



void npMatrixTranslate(float* Out, float x, float y, float z)
{
	Out[4*3 + 0] = x;
	Out[4*3 + 1] = y;
	Out[4*3 + 2] = z;
}

void npMatrixScale(float* Out, float x, float y, float z){
	Out[4*0 + 0] = x;
	Out[4*1 + 1] = y;
	Out[4*2 + 2] = z;
}

float npLerp(float start,float end, float time){
	return start * (1-time) + end*time;
}

//러프 함수
//=======================================================================///
inline float Lerp(float Start, float End, float Percent)
{
	return Start*(1.0f- Percent) + End*Percent;
}


float CalcDistanceToABS(float* pV2start, float* pV2end)
{
	if(pV2start == NULL || pV2end == NULL )
	{
		//LOGE("CalcDistanceToABS : In Factor Error");
		return 0;
	}

	int distanceX = std::abs((double)pV2start[0] - (double)pV2end[0]);
	int distanceY = std::abs((double)pV2start[1] - (double)pV2end[1]);

	return (float)sqrt((double)(distanceX*distanceX + distanceY* distanceY));
}

//------------------------------------------------------------------------------------------------------------------------------///
//목적 : 거리 계산 결과가 Tolerance 의 범위보다 작으면 true를 리턴한다
//필요조건 : regionX,Y,Z에 고정된 좌표를 넣어야하고, 유동 좌표(터치)를 posXYZ에 넣어야한다
//계약사항 : 거리 계산시에 절대값을 사용한다
//예외 :
bool CompDistanceByTwoPoints(float Tolerance, float regionX,float regionY,float posX, float posY)
{
	//LOGE("CompDistance Loop");
	float X = (posX - regionX);
	float Y = (posY - regionY);
	float realDistance = sqrt(  (X*X) + (Y*Y) );

	realDistance = ( realDistance < 0) ? realDistance*(-1) : realDistance;

	if(  realDistance < Tolerance )
	{
		//LOGI("CompDistance TRUE");
		return true;
	}
	//LOGE("CompDistance LOOP");
	return false;
}
