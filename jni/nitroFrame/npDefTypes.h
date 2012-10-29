#ifndef __NPDEFTYPES_H__
#define __NPDEFTYPES_H__

//TypeDef
typedef float npV2Vector[2];
typedef int npSize_WH[2];
typedef float npSizef_WH[2];
typedef int npIntPair[2];

typedef struct _COLOR_RGBA{
	void Init(float r, float g, float b, float a) {
		R = r; G = g; B = b; A = a;
	}
	float R;float G;float B;float A;
}COLOR_RGBA;

// 공용 구조체
typedef struct _Rect{
	int Left;
	int Right;
	int Top;
	int Bottom;
}npRect;

class AREA {
public:
	npV2Vector m_vPos;
	npSize_WH m_HalfWH;
	float m_StandY;
public:
	void Init(float PosX, float PosY, int Width, int Height);
};

//=======================================================================///
// UV 좌표계로 변환해서 검사
inline bool CompPosInArea(float x, float y, AREA* pArea )
{
	if( x > pArea->m_vPos[0] - pArea->m_HalfWH[0] && x < pArea->m_vPos[0] + pArea->m_HalfWH[0]){
		if( y < -(pArea->m_vPos[1] - pArea->m_HalfWH[1] /*+ pArea->m_StandY*/) && y > -(pArea->m_vPos[1] + pArea->m_HalfWH[1]/*+ pArea->m_StandY*/) )
			return true;
	}

	return false;
}

#endif
