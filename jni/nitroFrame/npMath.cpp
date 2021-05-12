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

float Vec3Dot(const float* v1, const float* v2){
        return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

//러프 함수
//=======================================================================///
float npLerp(float start,float end, float time){
	return start * (1-time) + end*time;
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



BEZIER_2POINT GetPointFromBezier4(BEZIER_2POINT* p1,BEZIER_2POINT* p2,BEZIER_2POINT* p3,BEZIER_2POINT* p4,float mu)
{
    float mum1,mum13,mu3;
    BEZIER_2POINT p;
    mum1 = 1 - mu;
    mum13 = mum1 * mum1 * mum1;
    mu3 = mu * mu * mu;
    p.x = mum13*p1->x + 3*mu*mum1*mum1*p2->x + 3*mu*mu*mum1*p3->x + mu3*p4->x;
    p.y = mum13*p1->y + 3*mu*mum1*mum1*p2->y + 3*mu*mu*mum1*p3->y + mu3*p4->y;
    return(p);
}

BEZIER_2POINT GetPointFromBezier3(BEZIER_2POINT* p1,BEZIER_2POINT* p2,BEZIER_2POINT* p3, float mu)
{
	double mum1,mum12,mu2;
	BEZIER_2POINT p;
	mu2 = mu * mu;
	mum1 = 1 - mu;
	mum12 = mum1 * mum1;
	p.x = p1->x * mum12 + 2 * p2->x * mum1 * mu + p3->x * mu2;
	p.y = p1->y * mum12 + 2 * p2->y * mum1 * mu + p3->y * mu2;
	 return(p);
}
