/*
 * npRenderProcess.h
 *
 *  Created on: 2012. 10. 26.
 *      Author: Ophelia
 */

#ifndef NPRENDERPROCESS_H_
#define NPRENDERPROCESS_H_

#include <GLES/gl.h>
#include <GLES/glext.h>
#include "../gluLookAt.h"
#include "../npDebugUtill.h"
#include "../Drawable/npDrawable.h"
#include "../npNitroFrameMacro.h"
#include "../Render/npTextureGenerator.h"

class npRenderprocess{
public:
	npRenderprocess();
	~npRenderprocess();

public:
	static npRenderprocess& getInstance();

	virtual void DoDraw(npDrawable& drawTarget);

	void setOrth(int width, int height, float zNear, float zFar);
	void setFrustum(int width, int hegith, float zNear, float zFar);

	void setLookat(GLfloat eyex, GLfloat eyey, GLfloat eyez,
            GLfloat centerx, GLfloat centery, GLfloat centerz,
            GLfloat upx, GLfloat upy, GLfloat upz);

	NP_DEFINE_PROPERTY_READONLY(int,deviceWidth,DeviceWidth);
	NP_DEFINE_PROPERTY_READONLY(int,deviceHeight,DeviceHeight);
	NP_DEFINE_PROPERTY_READONLY(float, widthRatio, WidthRatio);
	NP_DEFINE_PROPERTY_READONLY(float, heightRatio, HeightRatio);
	NP_DEFINE_PROPERTY_READONLY(int, renderWidth, RenderWidth);
	NP_DEFINE_PROPERTY_READONLY(int, renderHeight, RenderHeight);

	void setRenderingResolution(int width,int height);
	void setDeviceResolution(int width,int height);

};


#endif /* NPRENDERPROCESS_H_ */
