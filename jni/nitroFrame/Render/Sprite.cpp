/*
 * Sprite.cpp
 *
 *  Created on: 2012. 10. 25.
 *      Author: Ophelia
 */
#include "Sprite.h"

/** \fn sprite *sprite::GetSpriteByTAG(SCREENPLAY_TAG tagName)
 *  \brief TAG를 기반으로 SpriteFactory를 통해 Instance를 생성해서 가져온다.
 *  \param tagName 이 sprite에 사용할 TAG를 말한다.
 *  \return sprite 객체를 넘겨받는다.
 */
/*
sprite* sprite::GetSpriteByTAG(SCREENPLAY_TAG tagName) {


}*/

void sprite::BindThisSprite() {

}

void sprite::ReadyForNextScreenplay() {
	this->currentScreenplay = this->currentScreenplay->getNext();
}


sprite::sprite(){
	screenplay = new npLinkNode<int>;
}


sprite::~sprite() {
}
