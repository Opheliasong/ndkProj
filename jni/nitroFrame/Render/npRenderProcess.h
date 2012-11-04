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
#include "../Drawable/npDrawable.h"
#include "../npNitroFrameMacro.h"
#include "../Render/npTextureGenerator.h"

class npRenderprocess{
public:
	~npRenderprocess();

	npRenderprocess& getInstance();

	void DoDraw(npDrawable& drawTarget);


	void setOrth(int width, int height, float zNear, float zFar);
	void setFrustum(int width, int hegith, float zNear, float zFar);

private:	npRenderprocess();
};


#endif /* NPRENDERPROCESS_H_ */
