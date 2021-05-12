/*
 * TexturePalette.h
 *
 *  Created on: 2012. 10. 21.
 *      Author: Ophelia
 */

#ifndef FRAMEPALETTE_H_
#define FRAMEPALETTE_H_

//#include "../NitroFrame.h"
#include "../npDebugUtill.h"
#include "npRenderTypedef.h"
#include <map>


/**
 * @author 박성민
 * @brief
 * @details Container 의 한 부분으로 전체 실제로 GL함수에서 Texture Binding및 UV값을 가지기 위한 것들을 가지고 있다.
 * 이전의 DataStorage와 같은 책임을 가지나 몸집을 줄이고 Container 로서의 역할만 충실히 한다.
 * */
class npAtlasMap{
public:
	TextureAtlasMap FrameContainer;
	static npAtlasMap& getInstance();

private:
	//static npFramePalette* PaletteInstance;
	npAtlasMap();
	~npAtlasMap();
};


#endif /* FRAMEPALETTE_H_ */
