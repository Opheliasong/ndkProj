/*
 * pbTypes.h
 *
 *  Created on: 2012. 7. 24.
 *      Author: NitroPigSoft02
 */

#ifndef PBTYPES_H_
#define PBTYPES_H_

typedef std::string sceneTag;

typedef std::string ItemCode;
#define ITEMCODE_POTION_LIFE "LIFE"
#define ITEMCODE_POTION_FEVERPOINT "FEVERPOINT"
#define ITEMCODE_POTION_SPEEDDOWN "SPEEDDOWN"
#define ITEMCODE_POTION_SCOREPLUS "SCOREPLUS"

#define ITEMCODE_VEHICLE_DEFAULT "BAMBOO_BROOMSTICK"
#define ITEMCODE_VEHICLE_NO_1 "MAPLE_BROOMSTICK"
#define ITEMCODE_VEHICLE_NO_2 "WHITEOAK_BROOMSTICK"

typedef struct _SCREENPLAYTAGDATA {
	screenplayTag Tag;
	float fWidth;
	float fHeight;
	void SetData(screenplayTag tag, float width, float height) {
		Tag = tag; fWidth = width; fHeight = height;
	}
} TAGDATA;

namespace projectBean{
/*	namespace TOUCHSTATUS{
		typedef enum {TOUCHNONE=0,TAPDOWN=1,TAPUP=2,TAPMOVE=3,TOUCHMODEEND=4}TYPE;
	}*/
namespace TOUCHFLAGS{
	typedef enum {TAPDOWN=0,TAPUP=1,TAPMOVE=2,}TYPE;
}
	namespace NOTETYPE{
		typedef enum {
			NONETYPE = 0,
			SWIPE = 100,
			TAP = 200,
			DTAP = 300,
			LPRESS = 400,
			MULTI1 = 500,
			MUTLI2 = 600,
			TARGETING = 700,
			LISTHEADER = 800
		}TYPE;
	}
	namespace NOTEJUDGEMENT{
		typedef enum {
			NOTJUDGE=0, GREAT = 100, SLOW, FAST, FAIL, MISS,JUDGEEND
		}TYPE;
	}
	namespace NOTEGROWSTATE{
		//��Ʈ�� ���� ����
		//��Ʈ�� ���� ���´� 4���� ���¸� ������ �� ���¿� ��� �Ǵ��� �ϰ� �ȴ�.
		/*NOTEBABY : ���� �ǵ帮�� �ȵǴ� ��Ʈ. = Fail
		 * NOTEGROWUP : �������� ��Ʈ = FAST
		 * NOTEADULT : ������ �Ϸ�� ��� �� ��Ʈ = GREAT
		 * NOTEOLDER: ������ ���� ��ȭ�Ǵ� ��Ʈ = SLOW
		 */
		typedef enum {
			NOTEBABY = 1000, NOTEGROWUP, NOTEADULT, NOTEOLDER
		}TYPE;
	}
	namespace UITYPE{
		typedef enum {
			LISTHEADER = 0, UI, ABILITYPOINT, SCORE, LIFE,
		}TYPE;
	}

	namespace EFFECTTYPE{
		typedef enum {
			NONE = -1, JUDGE = 0, COMBO, STICKER, STEPUP, FEVER_AVAILABLE, MISS,
		}TYPE;
	}

}

#endif /* PBTYPES_H_ */
