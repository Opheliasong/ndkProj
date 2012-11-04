/*
 * DAOBinder.h
 *
 *  Created on: 2012. 10. 26.
 *      Author: Ophelia
 */

#ifndef DAOBINDER_H_
#define DAOBINDER_H_

//#include "../NitroFrame.h"
#include "TextureAtlas.h"
#include "Sprite.h"

/***
 * @author 박성민
 * @brief DAO객체중 Sprite와 연동하여 Texture와 UV를 Bind하는 객체를 말한다.
 * @details DAO객체중 Interface의 상태를 변경하는 함수이다. Factory는 아예 생성하고, Inserter와 Deleter는 RAW Source와 관련이 있다.
 * Sprite가 가진 CurrentScreenplay의 FrameID를 가져와서 이 ID를 가지고 FramePalette에서 Frame정보를 가져온다.
 * 이 Frame정보는 GL Texture ID와 UV배열이 들어있으며, 이를 가지고 GL API를 이용해 Texture를 Bind하게 된다.
 * 이후 Drawable에서 삽입되어진 Vertex 정보와 함께, 위치값(Transform)을 지정후 glDrawArrays 를 하게 되면 Drawing 이 된다.
 *
 */
//npInsertModuleDAO
class sprite;

class npBindingDAO{
public:
	void BindingFrameBySprite(sprite& sprite);
	
	npBindingDAO();
	~npBindingDAO();
private:
	//복사 대입연산자를 막는다.
	npBindingDAO(const npBindingDAO& copyRefence){};

};


#endif /* DAOBINDER_H_ */
