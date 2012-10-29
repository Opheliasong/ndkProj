/*
 * npMath.h
 *
 *  Created on: 2012. 4. 13.
 *      Author: NitroPigSoft02
 */

#ifndef NPMATH_H_
#define NPMATH_H_

//#include "NitroFrame.h"
#include "stddef.h"
#include <cmath>

/// 수학
#define PI 3.14159265
#define degreesToRadians (x) (PI * (x) / 180.0)
#define RadiansToDegree(radian) ((radian) * (180.f / PI))

void npMatrixTranslate(float* Out, float x, float y, float z);
void npMatrixScale(float* Out, float x, float y, float z);
float npLerp(float start,float end, float time);

//러프 함수
//=======================================================================///
inline float Lerp(float Start, float End, float Percent);
float CalcDistanceToABS(float* pV2start, float* pV2end);

//------------------------------------------------------------------------------------------------------------------------------///
//목적 : 거리 계산 결과가 Tolerance 의 범위보다 작으면 true를 리턴한다
//필요조건 : regionX,Y,Z에 고정된 좌표를 넣어야하고, 유동 좌표(터치)를 posXYZ에 넣어야한다
//계약사항 : 거리 계산시에 절대값을 사용한다
//예외 :
bool CompDistanceByTwoPoints(float Tolerance, float regionX,float regionY,float posX, float posY);
#endif /* NPMATH_H_ */
