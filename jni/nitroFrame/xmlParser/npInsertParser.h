/*
 * npInsertParser.h
 *
 *  Created on: 2012. 10. 29.
 *      Author: Ophelia
 */

#ifndef NPINSERTPARSER_H_
#define NPINSERTPARSER_H_

#include <jni.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include "../include/tinyxml.h"
#include "../include/tinystr.h"
#include "../npDebugUtill.h"

#include "npXMLParer.h"
#include "../Render/npRenderTypedef.h"
#include "../Render/npContainerDAO.h"

/***
 * @author 박성민
 * @brief XML texture Data Insert 기능을 담당하는 파서
 * @details npXMLParser를 상속받아 확장시킨 클래스, 기본적인 npXMLParser의 Path를 통해 Asset을 생성하고 XML파일을 읽어온다.
 *
 */

class RawDataPacket;
class npInsertParser: public npXMLParser {
public:
	void DoParsing();
	npInsertParser(JNIEnv* aEnv, jobject* aAssetManager);

public:
	npInsertParser();
	virtual ~npInsertParser();

};

#endif /* NPINSERTPARSER_H_ */
