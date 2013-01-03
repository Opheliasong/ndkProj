/*
 * npNitroFrame.h
 *
 *  Created on: 2012. 10. 21.
 *      Author: Ophelia
 */

#ifndef NPNITROFRAME_H_
#define NPNITROFRAME_H_

//#include <stddef.h.>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <GLES/gl.h>
#include <GLES/glext.h>

#include "log.h"
#include "npDefTypes.h"
#include "npDebugUtill.h"
#include "npMath.h"
#include "npLinkNode.h"
#include "npRentalMemory.h"
#include "npMath.h"
#include "gluLookAt.h"
#include "npEngineFrames.h"
#include "npAudioSystem.h"

//XML Parsing 관련
#include "xmlParser/npXMLParser.h"
#include "xmlParser/npInsertParser.h"
#include "xmlParser/npDeleterParser.h"

//Render관련 Include
#include "Render/npRenderTypedef.h"
#include "Render/npTextureGenerator.h"
#include "Render/TextureAtlas.h"
#include "Render/ScreenplayStore.h"
#include "Render/DAOInsert.h"
#include "Render/Sprite.h"
#include "Render/DAOFactory.h"
#include "Render/DAOBinder.h"
#include "Render/DAODeleter.h"
#include "Render/TextureRawPacket.h"
#include "Render/npContainerDAO.h"
#include "Render/npRenderProcess.h"

//drawable 관련
#include "Drawable/npDrawable.h"
#include "Drawable/npTextureRect.h"

//Touch 관련 Interface와 Layer
#include "TouchLayer/TouchObserver.h"
#include "TouchLayer/TouchLayer.h"


#endif /* NPNITROFRAME_H_ */
