#ifndef __DEBUGUTILL_H__
#define __DEBUGUTILL_H__

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "log.h"
#include <GLES/gl.h>
#include <GLES/glext.h>
//#include "npList.h"

#define DEBUG_WORLDMOVE
//#define DEBUG_LIFEGAUGEMODE
//#define __DEBUG_COMPLETE_LOOPLOG
//#define __DEBUG_NULL_LOG
//#define DEBUG_LOG_TOUCH

//TypeDef
//  V2Vector �ε��� ������ ���� : ������
const int iX = 0;
const int iY = 1;
//  sizeWH �ε��� ������ ���� : ������
const int iW = 0;
const int iH = 1;

typedef float npV2Vector[2];
typedef int npSize_WH[2];
typedef float npSizef_WH[2];
typedef int npIntPair[2];


//-----���� ������ ����-------------------------------//
const float WIDTH = 800;
const float HEIGHT = 480;
const float HALF_WIDTH = WIDTH/2;
const float HALF_HEIGHT = HEIGHT/2;
const float WORLD_MOVESPEED = 300;
const int TOUCH_CORRECTION_RANGE = 20;

////-------UI ����---------////////
const float MAX_AP  = 1000.0f;
const float AP_FAIL = 100.0f;
const float AP_FAST = 40.f;
const float AP_SLOW = 40.f;
const float AP_GREAT = 0.0f;
const int MAX_LIFE  = 3;
//~~~~~~~~~~~~~~~~~~~~~//
namespace Android{
	enum ACTION {ACTION_NONE = -1, ACTION_DOWN = 0, ACTION_UP = 1, ACTION_MOVE = 2, ACTION_POINTER_DOWN = 5, ACTION_POINTER_UP = 6 };

}

namespace Direction{
	enum HEADING {FOWARD = 1, NEUTRAL  = 0 , BACKWARD  = -1};
	enum DIR{ HORIZONTAL = 0, VERTICAL = 1, NONE  = -1};
}

namespace BindTexture{
	enum ID{ NOTE = 0, BACKGROUND = 1, CHARACTER = 2, TUTORIAL = 3, MAX_BINDTEXTURE};
}

namespace NOTEGROW_LOOPTIME{
	const float BABY = 600.0f;
	const float GROWUP = 1000.0f;
	const float ADULT = 350.0f;
	const float OLDER = 500.0f;
}


//Ÿ�� ����
namespace NoteState{
	enum STATE{ DEFAULT = 0, FAIL, TOUCHED, END, MISS};
}

/// ����
#define PI 3.14159265
#define degreesToRadians (x) (PI * (x) / 180.0)
#define RadiansToDegree(radian) ((radian) * (180.f / PI))


///// ��Ŭ���� �α�Ĺ ��ũ��
#define LOGV(...)	__android_log_print(ANDROID_LOG_VERBOSE, "Bean", __VA_ARGS__)
#define LOGD(...)	__android_log_print(ANDROID_LOG_DEBUG, "Bean", __VA_ARGS__)
#define LOGI(...)	__android_log_print(ANDROID_LOG_INFO, "Bean", __VA_ARGS__)
#define LOGW(...)	__android_log_print(ANDROID_LOG_WARN, "Bean", __VA_ARGS__)
#define LOGE(...)	__android_log_print(ANDROID_LOG_ERROR, "Bean", __VA_ARGS__)

#define 	LOGpointerAddress(TYPE,logString, String, pointer) \
	sprintf(logString,"%s : %x\n", #String, &pointer);\
	TYPE(logString);


////��ƿ �Լ�
char* My_itoa(int val, int base);

void LOGfloatString(const char* string, float val);


/*
�������� 3���� ������ � ����
mu�� 0(��� ����)���� 1(��� ��) ������ ���� ���� �� �ֽ��ϴ�.

BEZIERPOINT Bezier3(BEZIERPOINT p1,BEZIERPOINT p2,BEZIERPOINT p3,double mu)
{
    double mum1,mum12,mu2;
    BEZIERPOINT p;
    mu2 = mu * mu;
    mum1 = 1 - mu;
    mum12 = mum1 * mum1;
    p.x = p1.x * mum12 + 2 * p2.x * mum1 * mu + p3.x * mu2;
    p.y = p1.y * mum12 + 2 * p2.y * mum1 * mu + p3.y * mu2;
    p.z = p1.z * mum12 + 2 * p2.z * mum1 * mu + p3.z * mu2;
    return(p);
}


�������� 4���� ������ � ����
mu�� 0(��� ����)���� 1(��� ��) ������ ���� ���� �� �ֽ��ϴ�.

BEZIERPOINT Bezier4(BEZIERPOINT p1,BEZIERPOINT p2,BEZIERPOINT p3,BEZIERPOINT p4,double mu)
{
    double mum1,mum13,mu3;
    BEZIERPOINT p;
    mum1 = 1 - mu;
    mum13 = mum1 * mum1 * mum1;
    mu3 = mu * mu * mu;
    p.x = mum13*p1.x + 3*mu*mum1*mum1*p2.x + 3*mu*mu*mum1*p3.x + mu3*p4.x;
    p.y = mum13*p1.y + 3*mu*mum1*mum1*p2.y + 3*mu*mu*mum1*p3.y + mu3*p4.y;
    p.z = mum13*p1.z + 3*mu*mum1*mum1*p2.z + 3*mu*mu*mum1*p3.z + mu3*p4.z;
    return(p);
}*/

void SetVertexByLeftTop(GLfloat* pVertex, float width, float height);
void SetVertexByCenter(GLfloat* pVertex, float width, float height);
//--------------------------------------------
void SetUVByLeftTop(GLfloat* pTexUV, GLfloat TexSourceW,  GLfloat TexSourceH, GLfloat X, GLfloat Y, GLfloat width, GLfloat height);
//void SetUVByLeftBottom(GLfloat* pTexUV, GLfloat TexSourceW,  GLfloat TexSourceH, GLfloat X, GLfloat Y, GLfloat width, GLfloat height);

//======================================================================================//


#endif /*__ANDROID_LOG_H__*/

