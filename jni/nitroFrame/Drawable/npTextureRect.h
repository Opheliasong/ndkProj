/*
 * npTextureRect.h
 *
 *  Created on: 2012. 11. 3.
 *      Author: Ophelia
 */

#ifndef NPTEXTURERECT_H_
#define NPTEXTURERECT_H_

#include <GLES/gl.h>
#include <GLES/glext.h>
#include "../npNitroFrameMacro.h"
#include "npDrawable.h"
#include "../Render/npRenderTypedef.h"
#include "../Render/Sprite.h"
#include "../Render/npContainerDAO.h"

class npTextureRect: public npDrawable {
public:
	npTextureRect();
	npTextureRect(screenplayTag TAG);
	npTextureRect(const npTextureRect& source);
	virtual ~npTextureRect();

	void PreSettingDraw();
	void DrawThis();

	NP_DEFINE_PROPERTY(int, width, WidthSize);
	NP_DEFINE_PROPERTY(int, height, HeightSize);
	NP_DEFINE_PROPERTY(int, positionX, X);
	NP_DEFINE_PROPERTY(int, positionY, Y);

	void SetSize(int width, int Height);
	bool SetTextureTAG(screenplayTag TAG);
private:
	GLfloat vertex[12];
	sprite* sprtie;
	screenplayTag tag;
};

#endif /* NPTEXTURERECT_H_ */
