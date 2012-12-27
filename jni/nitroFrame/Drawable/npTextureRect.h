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

	virtual void PreSettingDraw();
	virtual void DrawThis();

	NP_DEFINE_PROPERTY(float, width, WidthSize);
	NP_DEFINE_PROPERTY(float, height, HeightSize);
	NP_DEFINE_PROPERTY(float*, positionX, X);
	NP_DEFINE_PROPERTY(float*, positionY, Y);

	//회전 함수
	void SetRotationByXAxis(float rotationAngle);
	void SetRotationByYAxis(float rotationAngle);
	void SetRotationByZAxis(float rotationAngle);
	void SetRotation(float rotationAngle, float x, float y, float z);

	float getRotationAngle(){return this->rotationAngle;};

	void SetSize(int width, int Height);
	void SetPosition(float* x, float* y);
	bool SetTextureTAG(screenplayTag TAG);
	sprite* getSprite();

	bool IsFinish();
protected:
	static GLfloat vertex[12];
	sprite* sprtie;
	screenplayTag tag;

	float rotationAngle;
	struct RotationAxis{
		float x;
		float y;
		float z;
	};
	RotationAxis rotationAxis;
};

#endif /* NPTEXTURERECT_H_ */
