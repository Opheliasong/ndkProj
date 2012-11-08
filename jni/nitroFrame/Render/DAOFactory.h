/*
 * DAOFactory.h
 *
 *  Created on: 2012. 10. 25.
 *      Author: Ophelia
 */

#ifndef DAOFACTORY_H_
#define DAOFACTORY_H_

//#include "../NitroFrame.h"
#include "Sprite.h"
#include "npRenderTypedef.h"

//typedef std::string screenplayTag;

class sprite;

class npFactoryDAO{
	/***
	 * @author 박성민
	 * @brief Sprite를 ScreenplayStore에서 가져와 실제 사용할 Instance로 생성하는 객체
	 * @details Container와 통신하는 DAO객체중 하나로 Sprite를 생성하는 것에 책임을 가지고 있는 객체.
	 * 상태나 변수를 가지지 않고 method기반으로 이루어져 있다.
	 */

private:
//	void copyLinkedList();
public:
	sprite* getSpriteByTAG(screenplayTag tag);

	npFactoryDAO();
	~npFactoryDAO();
};



#endif /* DAOFACTORY_H_ */
