#include "npDebugUtill.h"


char* My_itoa(int val, int base)
{
    static char buf[32] = {0};
    int i = 30;
    for(; val && i ; --i, val /= base)
        buf[i] = "0123456789abcdef"[val % base];
    return &buf[i+1];
}

void LOGfloatString(const char* string, float val){
	char logString[256];
	sprintf(logString,"%s : %f\n",string,val);
	LOGI(logString);
}

//=======================================================================///
void SetVertexByLeftTop(GLfloat* pVertex, float width, float height)
{
	//왼쪽 위
	pVertex[0] = 0;
	pVertex[1] = 0;
	pVertex[2] = 0;
	//왼쪽아래
	pVertex[3] = 0;
	pVertex[4] = -height;
	pVertex[5] = 0;
	//오른쪽 위
	pVertex[6] = width;
	pVertex[7] = 0;
	pVertex[8] = 0;
	//오른쪽 아래
	pVertex[9] = width;
	pVertex[10] = -height;
	pVertex[11] = 0;
}

void SetVertexByCenter(GLfloat* pVertex, float width, float height)
{
	//왼쪽 위
	pVertex[0] = -width/2;
	pVertex[1] =  height/2;
	pVertex[2] = 0;
	//왼쪽아래
	pVertex[3] = -width/2;
	pVertex[4] = -height/2;
	pVertex[5] = 0;
	//오른쪽 위
	pVertex[6] = width/2;
	pVertex[7] = height/2;
	pVertex[8] = 0;
	//오른쪽 아래
	pVertex[9] = width/2;
	pVertex[10] = -height/2;
	pVertex[11] = 0;
}

// UV 좌표계 : 	↓→ u
//                      v
void SetUVByLeftTop(GLfloat* pTexUV, GLfloat TexSourceW,  GLfloat TexSourceH, GLfloat X, GLfloat Y, GLfloat width, GLfloat height)
{
	pTexUV[0] = X/ TexSourceW;
	pTexUV[1] = Y / TexSourceH;

	pTexUV[2] = X / TexSourceW;
	pTexUV[3] = (Y + height) / TexSourceH;

	pTexUV[4] = (X + width) / TexSourceW;
	pTexUV[5] = Y / TexSourceH;

	pTexUV[6] = (X + width) / TexSourceW;
	pTexUV[7] = (Y + height) / TexSourceH;
}



//=======================================================================///


