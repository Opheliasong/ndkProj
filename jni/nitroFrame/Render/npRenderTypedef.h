/*
 * npRenderTypedef.h
 *
 *  Created on: 2012. 11. 1.
 *      Author: Ophelia
 */

#ifndef NPRENDERTYPEDEF_H_
#define NPRENDERTYPEDEF_H_

#include <GLES/gl.h>
#include <GLES/glext.h>
#include "../npLinkNode.h"
#include <map>
#include <string>

/**
 * @author 박성민
 * @brief screenplay에 사용되는 Tag String.
 */
typedef std::string screenplayTag;

/**
 * @author 박성민
 * @brief screenplay(각본)을 담당하는 객체
 * 내부는 UV Packet의 ID의 리스트로 구성되어져 있다.
 * FramePalette가 각 장면(필름의 한 장면을 말한다)들을 모두 가지고 있고 Screenplays가 이 장면들을 이어주는 대본이다.
 * 이 각본들은 배우(Actor)들에게 넘겨지고, 이 배우들은 이 각본에 따라 자신의 위치에서 Texture를 바꾸면서 Rener하게 된다.
 * screenplay는 단순하게 각 프레임들의 다음,이전 프레임들을 관리하게 된다.
 * @version 0.5
 */
typedef npLinkNode< int > screenplays;
typedef std::map<screenplayTag, screenplays > screenplayMap;
typedef std::map<screenplayTag, screenplays >::iterator screenMapIter;
typedef std::pair<screenplayTag, screenplays > ScreenplayPair;

/**
 * @author 박성민
 * @brief FramePalette에 key와 Pair할 Packet을 말한다.
 * @details unsigned int type의 bindTextureID와 Texture Array로 구성되어져 있다.
 */
class UVPacket{
public:
	UVPacket():bindTextureID(0){};
	~UVPacket(){};

	GLuint bindTextureID;
	GLfloat texture[8];

	UVPacket &operator= (const UVPacket& rightSide){	/**
	 * @fn UVPacket& UVPacket::operator=(const UVPacket& rightSide)
	 * @brief UVPacket의 Deep Copy를 실행하는 대입 연산자
	 * @details textureID는 int로 단일 복사하고, UV Array는 GLfloat의 8개 만큼 복사한다.
	 * @param const UVpacket& rightSide 대입연산자의 우측항
	 * @return UVPacket& 복사연산자의 좌측항
	 *
	*/
	this->bindTextureID = rightSide.bindTextureID;
	memcpy(this->texture, rightSide.texture, 8*sizeof(GLfloat));
	return *this;};
};

/**
 * @author 박성민
 * @brif Container와의 통신을 위한 module 중 하나로 Insert에 관련된 기능을 수행한다.
 * InsertModule은 Texture Generater와 XML Parser와 관련이 깊다.
 */
class RawDataPacket{
public:
	screenplayTag tag;
	GLuint textureBindIndex;
	int x;
	int y;
	int width;
	int height;
	int textureWidth;
	int textureHeight;
};

typedef std::map<int, UVPacket> TextureAtlasMap;
typedef std::map<int, UVPacket>::iterator TextureAtlasIter;
typedef std::pair < int, UVPacket > TextureAtlasPair;


#endif /* NPRENDERTYPEDEF_H_ */
