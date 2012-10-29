/*
 * npTextureGenerator.h
 *
 *  Created on: 2012. 10. 26.
 *      Author: Ophelia
 */

#ifndef NPTEXTUREGENERATOR_H_
#define NPTEXTUREGENERATOR_H_

#include "../NitroFrame.h"
#include <string>

typedef std::string texturePath;

/***
 * @author 박성민
 * @brief Texture를 Asset에서 읽어들여서 GL 상태기계에 Generate하고, 이와 함께 Texture ID를 얻어오는 역할을 한다.
 * @details Asset에서 PNG를 얻어오기 위해서 ziplib와 libpng를 사용한다.
 * texture를 generate하고 Texture ID를 가져와서 TextureRAWPacket의 TextureID를 입력한다.
 * xmlParser가 TextureGenerator에게 Texture경로를 알려주면, TextureGenerator는 입력된 경로들을 기반으로 Texture정보를 가져온다.
 */
class npTextureGenerator{
public:
	GLuint GenerateTextureByPNGPath(texturePath path);

	static npTextureGenerator* getInstance();

private:
	npTextureGenerator();
	~npTextureGenerator();
};



#endif /* NPTEXTUREGENERATOR_H_ */
