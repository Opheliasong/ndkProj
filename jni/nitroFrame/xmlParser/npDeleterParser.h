/*
 * npDeleterParser.h
 *
 *  Created on: 2012. 10. 29.
 *      Author: Ophelia
 */

#ifndef NPDELETERPARSER_H_
#define NPDELETERPARSER_H_

#include <jni.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include "npXMLParer.h"
#include "../Render/npContainerDAO.h"
#include "../Render/npRenderTypedef.h"

class npContainerDAO;
/***
 * @author 박성민
 * @brief  Scene에서 사용한 Texture의 Tag 정보를 읽어오는 Parser
 * @details Scene에서는
 */
class npDeleterParser: public npXMLParser {
public:
	npDeleterParser(JavaVM* javaVM, jobject* aAssetManager);
	virtual ~npDeleterParser();
	npDeleterParser();

	void DoParsing();
};

#endif /* NPDELETERPARSER_H_ */
