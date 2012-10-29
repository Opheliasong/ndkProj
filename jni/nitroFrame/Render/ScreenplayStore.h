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


typedef std::string screenplayTag;
/**
 * @author 박성민
 * @brief screenplay(각본)을 담당하는 객체
 * 내부는 UV Packet의 ID의 리스트로 구성되어져 있다.
 * FramePalette가 각 장면(필름의 한 장면을 말한다)들을 모두 가지고 있고 Screenplays가 이 장면들을 이어주는 대본이다.
 * 이 각본들은 배우(Actor)들에게 넘겨지고, 이 배우들은 이 각본에 따라 자신의 위치에서 Texture를 바꾸면서 Rener하게 된다.
 * screenplay는 단순하게 각 프레임들의 다음,이전 프레임들을 관리하게 된다.
 * @version 0.5
 */
//typedef npLinkNode< int > screenplays;
typedef std::map<screenplayTag, npLinkNode< int > > screenplayMap;
typedef std::map<screenplayTag, npLinkNode< int > >::iterator screenMapIter;
typedef std::pair<screenplayTag, npLinkNode< int > > ScreenplayPair;

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
