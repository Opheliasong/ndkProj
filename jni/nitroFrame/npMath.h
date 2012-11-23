/*
 * npMath.h
 *
 *  Created on: 2012. 4. 13.
 *      Author: NitroPigSoft02
 */

#ifndef NPMATH_H_
#define NPMATH_H_

//#include "NitroFrame.h"
//#include "stddef.h"
#include <cmath>

/// 수학
#define PI 3.14159265
#define degreesToRadians (x) (PI * (x) / 180.0)
#define RadiansToDegree(radian) ((radian) * (180.f / PI))

void npMatrixTranslate(float* Out, float x, float y, float z);
void npMatrixScale(float* Out, float x, float y, float z);
//러프 함수
//=======================================================================///
float npLerp(float start,float end, float time);


float Vec3Dot(const float* v1, const float* v2);

float CalcDistanceToABS(float* pV2start, float* pV2end);

//------------------------------------------------------------------------------------------------------------------------------///
//목적 : 거리 계산 결과가 Tolerance 의 범위보다 작으면 true를 리턴한다
//필요조건 : regionX,Y,Z에 고정된 좌표를 넣어야하고, 유동 좌표(터치)를 posXYZ에 넣어야한다
//계약사항 : 거리 계산시에 절대값을 사용한다
//예외 :
bool CompDistanceByTwoPoints(float Tolerance, float regionX,float regionY,float posX, float posY);

//영역 안에 있는지 검사한다
inline bool CompareTouchPointInArea(float x, float y, float* pAreaV2Pos, float* pAreaSizeHalf);

typedef struct _BEZIER_2POINT{
    float x;
    float y;
}BEZIER_2POINT;

/*
조절점이 4개인 베지어 곡선 예제
mu는 0(곡선의 시작)부터 1(곡선의 끝) 사이의 값을 가질 수 있습니다.
*/
BEZIER_2POINT GetPointFromBezier4(BEZIER_2POINT* p1,BEZIER_2POINT* p2,BEZIER_2POINT* p3,BEZIER_2POINT* p4,float mu);
BEZIER_2POINT GetPointFromBezier3(BEZIER_2POINT* p1,BEZIER_2POINT* p2,BEZIER_2POINT* p4,float mu);


#endif /* NPMATH_H_ */
