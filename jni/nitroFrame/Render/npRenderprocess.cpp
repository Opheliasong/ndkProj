/*
 * npRenderprocess.cpp
 *
 *  Created on: 2012. 10. 26.
 *      Author: Ophelia
 */

#include "npRenderProcess.h"

npRenderprocess::npRenderprocess() {
}

npRenderprocess::~npRenderprocess() {
}

void npRenderprocess::setOrth(int width, int height, float zNear, float zFar) {
	glOrthof(-width/2, width/2, -height/2, height/2, zNear, -zFar);
}

void npRenderprocess::setFrustum(int width, int hegith, float zNear,float zFar) {
}

npRenderprocess& npRenderprocess::getInstance() {
	static npRenderprocess Instace;
	return Instace;
}


void npRenderprocess::DoDraw(npDrawable& drawTarget) {
	drawTarget.PreSettingDraw();
	drawTarget.DrawThis();
}
