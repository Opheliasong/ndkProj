/*
 * ScreenplayStore.h
 *
 *  Created on: 2012. 10. 24.
 *      Author: Ophelia
 */

#ifndef SCREENPLAYSTORE_H_
#define SCREENPLAYSTORE_H_

#include "../NitroFrame.h"
#include <string>
#include <map>

/**
 * @author 박성민
 * @brief
 * @details
 * 각본들을 모아두는 창고(Store)를 말한다. Scene(장면:특정한 연출 장면,극의 한 막과 같은)들에게 필요한 각본들을 가지고 있는 곳을 말한다.
 * DAO중 Factory역할을 하는 개체와 관계가 있다.
 * SCREENPLAY_TAG를 Key로 하는 Map으로 구성되어져 있으며, Data는 ScreenPlays를 가지고 있다.
 * @version 0.5
 */

class screenplayStore{
public:
	screenplayMap screenplayContainer;
	static screenplayStore& getInstance();

private:
	screenplayStore();
	~screenplayStore();
};


#endif /* SCREENPLAYSTORE_H_ */
