/*
 * Sprite.h
 *
 *  Created on: 2012. 10. 25.
 *      Author: Ophelia
 */

#ifndef SPRITE_H_
#define SPRITE_H_
//#include "../NitroFrame.h"
#include <string>
#include "ScreenplayStore.h"
#include "npRenderTypedef.h"

/**
 * \author 박성민
 * \brief 이 객체는 머 이런식으로
 * \details 이 객체는 디테일 하게 이렇게
 * \version 0.5
 *
 */

//typedef std::string screenplayTag;

typedef void (*SEL_SCHEDULE)(float);
class sprite {
public:
	void BindThisSprite();

	void ReadyForNextScreenplay();

	npLinkNode<int>* screenplay;
	npLinkNode<int>* currentScreenplay;
	screenplayTag tag;

	sprite();
	~sprite();
};


#endif /* SPRITE_H_ */
