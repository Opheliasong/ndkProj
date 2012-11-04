#ifndef __NPDEBUGUTILL_H__
#define __NPDEBUGUTILL_H__

#include <GLES/gl.h>
#include <GLES/glext.h>
#include <stdio.h>
#include "log.h"
//#include "NitroFrame.h"

//-----게임 프레임 관련-------------------------------//
const float WIDTH = 800;
const float HEIGHT = 480;
const float HALF_WIDTH = WIDTH/2;
const float HALF_HEIGHT = HEIGHT/2;
const float WORLD_MOVESPEED = 580;
const int TOUCH_CORRECTION_RANGE = 20;

namespace Android{
	enum ACTION {ACTION_NONE = -1, ACTION_DOWN = 0, ACTION_UP = 1, ACTION_MOVE = 2, ACTION_POINTER_DOWN = 5, ACTION_POINTER_UP = 6 };

}

namespace Direction{
	enum HEADING {FOWARD = 1, NEUTRAL  = 0 , BACKWARD  = -1};
	enum DIR{ HORIZONTAL = 0, VERTICAL = 1, NONE  = -1};
}


///// 이클립스 로그캣 매크로
#define LOGV(...)	__android_log_print(ANDROID_LOG_VERBOSE, "Bean", __VA_ARGS__)
#define LOGD(...)	__android_log_print(ANDROID_LOG_DEBUG, "Bean", __VA_ARGS__)
#define LOGI(...)	__android_log_print(ANDROID_LOG_INFO, "Bean", __VA_ARGS__)
#define LOGW(...)	__android_log_print(ANDROID_LOG_WARN, "Bean", __VA_ARGS__)
#define LOGE(...)	__android_log_print(ANDROID_LOG_ERROR, "Bean", __VA_ARGS__)

#define 	LOGpointerAddress(TYPE,logString, String, pointer) \
	sprintf(logString,"%s : %x\n", #String, &pointer);\
	TYPE(logString);


////유틸 함수
char* My_itoa(int val, int base);
float Vec3Dot(const float* v1, const float* v2);
void LOGfloatString(const char* string, float val);
bool CompDistanceByTwoPoints(float Tolerance, float regionX,float regionY, float posX, float posY);

class ANIMATION_IDS{
public:
	int m_StartTexID;
	int m_EndTexID;
	float m_fTime;
};

void SetVertexByLeftTop(GLfloat* pVertex, float width, float height);
void SetVertexByCenter(GLfloat* pVertex, float width, float height);
//--------------------------------------------
void SetUVByLeftTop(GLfloat* pTexUV, GLfloat TexSourceW,  GLfloat TexSourceH, GLfloat X, GLfloat Y, GLfloat width, GLfloat height);

#define AnimationLoop_EqualInfinite(fTime, AniTickTime, AniCountTime, IncreaseValue, StartValue, EndValue)\
AniCountTime += fTime;\
if( AniCountTime > AniTickTime){\
AniCountTime = 0.0f;\
IncreaseValue++;\
if(IncreaseValue == EndValue)	{IncreaseValue = StartValue;}\
}



#endif /*__ANDROID_LOG_H__*/

