/*
 * TextureRawPacket.h
 *
 *  Created on: 2012. 10. 26.
 *      Author: Ophelia
 */

#ifndef TEXTURERAWPACKET_H_
#define TEXTURERAWPACKET_H_

//#include "../NitroFrame.h"
#include "TextureAtlas.h"
#include "ScreenplayStore.h"

/***
 * @author 박성민
 * @brief Texture관련 RAW데이터 OBJ를 말한다.
 * @details Teture Bind에 필요한 GL Texture ID, UV Array Data,Frame ID ,TAG 정보들을 담고 있다.
 * 이 정보들은 Inserter와 Deleter가 사용하게 된다.
 * Inserter는 GL Texture ID와 UV Array Data를 FramePalette로,
 * TAG정보를 읽어서 ScreenplayStore에 Frame ID와 함께 ScreenplayStorage에 입력한다.
 * Deleter는 TAG를 읽어 들여서 ScreenplayStore의 Container와 FramePalette의 Container를 earse한다.
 *
 */
class UVPacket;

class npTextureRawPacket{
public:
	UVPacket UvPacket;
	screenplayTag tag;
};


#endif /* TEXTURERAWPACKET_H_ */
