/*
 * pbTypes.h
 *
 *  Created on: 2012. 7. 24.
 *      Author: NitroPigSoft02
 */

#ifndef PBTYPES_H_
#define PBTYPES_H_

namespace projectBean{
	namespace TOUCHSTATUS{
		typedef enum {TOUCHNONE=0,TAPDOWN=1,TAPUP=2,TAPMOVE=3,TOUCHMODEEND=4}TYPE;
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
		//노트의 성장 상태
		//노트의 성장 상태는 4개의 상태를 가지며 이 상태에 따라 판단을 하게 된다.
		/*NOTEBABY : 아직 건드리면 안되는 노트. = Fail
		 * NOTEGROWUP : 성장중인 노트 = FAST
		 * NOTEADULT : 성장이 완료된 어른이 된 노트 = GREAT
		 * NOTEOLDER: 성장이 끝나 노화되는 노트 = SLOW
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
			JUDGE = 0, COMBO, STICKER, STEPUP, FEVER_AVAILABLE, MISS,
		}TYPE;
	}

	namespace SCENESTATE{
		typedef enum {GAME_NONE = 0, GAME_CREATE , GAME_INTRO,GAME_MAINMENU, GAME_PLAY,GAME_PAUSE,GAME_DESTROY}GAME;
		typedef enum {LOAD_NONE = 100, LOAD_INTRO ,LOAD_MAINMENU, LOAD_PLAY, }LOAD;
		typedef enum {ACTION_NONE = 200, ACTION_FOWARD, ACTION_BACKWARD, ACTION_SELECT_1, ACTION_SELECT_2, ACTION_SELECT_3,	}ACTION;
	}
}

#endif /* PBTYPES_H_ */
