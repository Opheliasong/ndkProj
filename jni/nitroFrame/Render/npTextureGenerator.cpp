/*
 * npTextureGenerator.cpp
 *
 *  Created on: 2012. 10. 26.
 *      Author: Ophelia
 */

#include "npTextureGenerator.h"

GLuint npTextureGenerator::GenerateTextureByPNGPath(texturePath path) {
}

npTextureGenerator::npTextureGenerator() {
}

npTextureGenerator* npTextureGenerator::getInstance() {
	static npTextureGenerator instance;
	return instance;
}

npTextureGenerator::~npTextureGenerator() {
}
