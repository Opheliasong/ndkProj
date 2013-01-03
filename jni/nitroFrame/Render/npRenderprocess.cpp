/*
 * npRenderprocess.cpp
 *
 *  Created on: 2012. 10. 26.
 *      Author: Ophelia
 */

#include "npRenderProcess.h"

npRenderprocess::npRenderprocess():deviceWidth(0),deviceHeight(0),widthRatio(0),heightRatio(0),renderWidth(0),renderHeight(0) {
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


/***
 * @fn Drawable의 인터페이스를 이용하여 Draw logic을 호출 하게 하는 인터페이스\
 * @details Drawable객체를 보다 쉽게 Draw하기 위해 호출하게 하는 인터페이스. 이를 사용하지 않고, 사용자가 직접 DrawThis()를 호출하여도 된다.
 * @param drawTarget
 */
void npRenderprocess::DoDraw(npDrawable& drawTarget) {
	drawTarget.PreSettingDraw();
	drawTarget.DrawThis();
}

void npRenderprocess::setRenderingResolution(int width, int height) {
	this->renderWidth = width;
	this->renderHeight = height;

	//만약 deviceWidth와 deviceHeight가 지정이 되어있을 경우 Ratio를 구한다.
	if(this->deviceWidth != 0 && this->deviceHeight != 0){
		this->widthRatio = this->deviceWidth / (float)this->renderWidth;
		this->heightRatio = this->deviceHeight / (float)this->renderHeight;
	}
/*
	LOGE("Device Resolution Width : %d",this->renderWidth);
	LOGE("Device Resolution Height : %d",this->renderHeight);

	LOGE("Width Ratio : %f",this->widthRatio);
	LOGE("Height Ratio : %f",this->heightRatio);
*/
}

void npRenderprocess::setLookat(GLfloat eyex, GLfloat eyey, GLfloat eyez,
		GLfloat centerx, GLfloat centery, GLfloat centerz, GLfloat upx,
		GLfloat upy, GLfloat upz) {

	gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
}

void npRenderprocess::setDeviceResolution(int width, int height) {
	this->deviceWidth = width;
	this->deviceHeight = height;

	//만약 renderWidth와 renderHeight가 지정이 되어있을 경우 Ratio를 구한다.
	if(this->renderWidth != 0 && this->renderHeight != 0){
				this->widthRatio = this->deviceWidth / (float)this->renderWidth;
				this->heightRatio = this->deviceHeight / (float)this->renderHeight;
	}
}
