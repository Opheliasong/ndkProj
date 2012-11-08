/*
 * npContainerDAO.h
 *
 *  Created on: 2012. 10. 29.
 *      Author: Ophelia
 */

#ifndef NPCONTAINERDAO_H_
#define NPCONTAINERDAO_H_

#include "npRenderTypedef.h"
#include "DAOInsert.h"
#include "DAOBinder.h"
#include "DAODeleter.h"
#include "DAOFactory.h"
#include "npTextureGenerator.h"
/*
#include "../xmlParser/npInsertParser.h"
#include "../xmlParser/npDeleterParser.h"
*/
#include "../npNitroFrameMacro.h"
#include "../npDebugUtill.h"

class npInsertParser;
class npDeleterParser;
class RawDataPacket;
class npTextureGenerator;
class npBindingDAO;
class npInsertDAO;
class npDeleteDAO;
class npFactoryDAO;
class sprite;

class npContainerDAO {
public:
	static void SetupTheAPKPath(char* apkPath);
	static void SetupBaseInitialize(JavaVM* javaVM, jobject& AssetManager,char* apkPath);
	static npContainerDAO& GetInstance();
	void InsertUVData(const RawDataPacket& rawPacket);	//InsertParser에서 호출하는 Interface
	void DeleteUVData(const screenplayTag& deleteTag);	//DeleteParser에서 호출하는 Interface
	sprite* getSpriteByTAG(screenplayTag tag);
	void BindingFrameBySprite(sprite& sprite);

	void LoadTextureByXMLpath(const char* xmlPath);
	void DeleteTextureByXMlpath(const char* xmlPath);
	GLuint GenerateTextureByPNGPath(const char* textureName);
	npTextureGenerator* getTextureGenerator();

	static jobject* getAssetManager();

private:
	npBindingDAO* TextureBindingModule;
	npInsertDAO* InsertModule;
	npDeleteDAO* DeleteModule;
	npFactoryDAO* FactoryModule;
	npInsertParser* InsertXMLParser;
	npDeleterParser* DeleteXMLParser;
	npTextureGenerator* TextureGenerator;

	static char* apkPath;
	//static JNIEnv* env;
	static JavaVM* javaVm;
	static jobject AssetManager;

	npContainerDAO();
	~npContainerDAO();
};

#endif /* NPCONTAINERDAO_H_ */
