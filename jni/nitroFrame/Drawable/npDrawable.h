/*
 * npDrawable.h
 *
 *  Created on: 2012. 10. 29.
 *      Author: Ophelia
 */

#ifndef NPDRAWABLE_H_
#define NPDRAWABLE_H_

/***
 * @author 박성민
 * @brief RenderProcess과 Node간의 공통된 인터페이스 객체
 * @details RenderProcess에서는 Drawable의 DrawThis()를 통해 Draw를 호출하게 된다.
 * Draw에 필요한 내용들은 이 Drawable객체를 상속받아서 구현하면 된다.
 *
 * PreSettingDraw()에서 DrawThis() 전에 필요한 내용들을 삽입해두고
 * DrawThis()에서 Draw에 필요한 로직들을 작성하면 된다.
 */

class npDrawable {
public:
	npDrawable();
	virtual ~npDrawable()=0;

	virtual void PreSettingDraw()=0;
	virtual void DrawThis()=0;
};


#endif /* NPDRAWABLE_H_ */
