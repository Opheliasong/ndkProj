/*
 * npTextureGenerator.h
 *
 *  Created on: 2012. 10. 26.
 *      Author: Ophelia
 */

#ifndef NPTEXTUREGENERATOR_H_
#define NPTEXTUREGENERATOR_H_

#include <GLES/gl.h>
#include <GLES/glext.h>
#include <string>
#include "../npDebugUtill.h"
#include "../npNitroFrameMacro.h"
#include "../include/zipint.h"
#include "../include/png.h"
#include "../include/pngconf.h"
#include "../include/pngpriv.h"
#include "../xmlParser/npInsertParser.h"

#include "../include/zip.h"
extern "C"{
#include "../include/png.h"
}

typedef std::string texturePath;

/***
 * @author 박성민
 * @brief Texture를 Asset에서 읽어들여서 GL 상태기계에 Generate하고, 이와 함께 Texture ID를 얻어오는 역할을 한다.
 * @details Asset에서 PNG를 얻어오기 위해서 ziplib와 libpng를 사용한다.
 * texture를 generate하고 Texture ID를 가져와서 TextureRAWPacket의 TextureID를 입력한다.
 * xmlParser가 TextureGenerator에게 Texture경로를 알려주면, TextureGenerator는 입력된 경로들을 기반으로 Texture정보를 가져온다.
 * 싱글톤 객체이다
 */
class npTextureGenerator{
private:
	char* apkRootPath;
	zip* apkArchive;
public :zip_file* apkZipFile;		//TODO png_zip_read()를 위해서 public으로 열어두었다. 이를 해결할 방법을 찾아야 하지 않을까?

	void Initializing(char* apkPath);
	void Releasing();
public:
	GLuint GenerateTextureByPNGPath(const char* textureName);

	//static npTextureGenerator& GetInstance();
	npTextureGenerator(char* apkPath);
	~npTextureGenerator();
private:

	npTextureGenerator();
};
void png_zip_read(png_structp png_ptr, png_bytep data, png_size_t length);



#endif /* NPTEXTUREGENERATOR_H_ */
