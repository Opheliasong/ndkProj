/*
 * npTextureGenerator.cpp
 *
 *  Created on: 2012. 10. 26.
 *      Author: Ophelia
 */

#include "npTextureGenerator.h"

#define TEXTURE_LOAD_ERROR 0

/***
 * @fn 텍스처 Name을 이용하여 PNG를 생성하고 Bind해서 Texture Index를 넘겨주는 함수
 * @param textureName
 * @return GL상태기계의 Texture Index
 * @warning 기본 texturePath가 assets/sprite/로 설정되어 있으며, Sub-directory를 가지게 된다면 이에 맞추어서 Path를 설정해야 한다.
 * @details textureName은 PNG의 이름을 말하며, 내부에서 PNG Path를 설정하게 된다.
 * 추후에 Path를 조금 더 유연하게 처리 할수 있도록 편의성을 제공하기 위해 수정해야 할 것이다.
 *
 */
GLuint npTextureGenerator::GenerateTextureByPNGPath(const char* aTextureName) {
	if(NP_IS_EMPTY(aTextureName)){
		LOGE("TextureGenerator) PLZ Input TextureName");
		return TEXTURE_LOAD_ERROR;
	}

	char pngPath[128];
	LOGE("Texture Name :");
	LOGE(aTextureName);
	sprintf(pngPath,"assets/sprite/%s\0",aTextureName);

	this->apkZipFile = zip_fopen(this->apkArchive,pngPath,0);
	if(NP_IS_EMPTY(this->apkZipFile)){
		LOGE("TextureGenerator)Texture opening has error");
		return TEXTURE_LOAD_ERROR;
	}

	png_byte header[8];

	//read the header
	zip_fread(this->apkZipFile, header, 8);

	//test if png
	int is_png = !png_sig_cmp(header, 0, 8);
	if(!is_png){
		zip_fclose(this->apkZipFile);
		LOGE("TextureGenerator) Not Png file");
		return TEXTURE_LOAD_ERROR;
	}

	//Create png struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(NP_IS_EMPTY(png_ptr)){
		zip_fclose(this->apkZipFile);
		LOGE("TextureGenerator)Unable to create png struct");
		return (TEXTURE_LOAD_ERROR);
	}

	//create png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if(NP_IS_EMPTY(info_ptr)){
		png_destroy_read_struct(&png_ptr,(png_infopp)NULL,(png_infopp)NULL);
		LOGE("TextureGenerator)Unalbe Create png info");
		zip_fclose(this->apkZipFile);
		return (TEXTURE_LOAD_ERROR);
	}

	//create png end info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if(NP_IS_EMPTY(end_info)){
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		LOGE("TextureGenerator)Unable to create png end info");
		zip_fclose(this->apkZipFile);
	}

	//png error stuff, nt sure libpng man suggest this
	if(setjmp(png_jmpbuf(png_ptr))){
		zip_fclose(this->apkZipFile);
		LOGE("TextureGenerator)Error during setimp");
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		return TEXTURE_LOAD_ERROR;
	}

	//init png reading
	//TODO 맴버 함수 포인터 처리.
	png_set_read_fn(png_ptr, NULL, png_zip_read);

	//let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr,8);

	//read all the info up to the image data
	png_read_info(png_ptr,info_ptr);

	//variables to pass to get info
	int bit_depth, color_type;
	png_uint_32 tWidth, tHeight;

	//get info about png
	png_get_IHDR(png_ptr, info_ptr, &tWidth, &tHeight, &bit_depth,&color_type, NULL, NULL, NULL);

	//Update width and height based on png info
	//TODO width, height 를 업데이트 한다. (필요한 곳에 넘겨주자)

	//Update the png info struct
	png_read_update_info(png_ptr, info_ptr);

	//Row size in bytes
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	//Allocate the image_data as a big block, to be given to openGL
	png_byte* image_data = new png_byte[rowbytes * tHeight];
	if(NP_IS_EMPTY(image_data)){
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		LOGE("TextureGenerator)Unaable to allocate image_data while loading");
		zip_fclose(this->apkZipFile);
		return TEXTURE_LOAD_ERROR;
	}

	//row_pointers is for pointing to image_data for reading the png with libpng
	png_bytep* row_pointers = new png_bytep[tHeight];
	if(NP_IS_EMPTY(row_pointers)){
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr,&info_ptr,&end_info);
		delete[] image_data;
		LOGE("TextureGenerator)Unalbe to allocate row_pointer while loading ");
		zip_fclose(this->apkZipFile);
		return TEXTURE_LOAD_ERROR;
	}

	//상하반전
	int count = 0;
	for(int i= tHeight -1;i>0;--i){
		row_pointers[tHeight - 1 - count] = image_data + i * rowbytes;
		count++;
	}

	//read the png info image_data through row_pointers
	png_read_image(png_ptr, row_pointers);

	//OpenGL Texture Generate
	//TODO TextureGenerator 내부에 Binding ID값을 관리할 로직이 필요하다.
	GLuint textureID;
	glGenTextures(1,&textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tWidth, tHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)image_data);
	glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//clean up memory and close stuff
	png_destroy_read_struct(&png_ptr,&info_ptr, &end_info);
	delete[] image_data;
	delete[] row_pointers;
	zip_fclose(this->apkZipFile);

	//TODO Texture Generator Binding ID 값을 리턴해야 한다.
	return textureID;
}

void png_zip_read(png_structp png_ptr, png_bytep data, png_size_t length) {
	zip_fread(npContainerDAO::GetInstance().getTextureGenerator()->apkZipFile, data, length);

}

npTextureGenerator::npTextureGenerator(char* apkPath):apkRootPath(NULL),apkArchive(NULL),apkZipFile(NULL) {
	Initializing(apkPath);
}

void npTextureGenerator::Initializing(char* apkPath) {
	size_t strLength = strlen(apkRootPath);
	if(strLength != 0){
		apkRootPath = new char[strLength];
		strcpy(this->apkRootPath,apkPath);
	}else{
		//TODO Stop And Throw Exception
	}
	this->apkArchive = zip_open(this->apkRootPath, 0, NULL);
	//TODO Stop And Loading Error excetpion throw
	if(this->apkArchive == NULL){
		LOGE("npTextureGenerator) Error Loading APK");
	}
}

/*
 * npTextureGenerator& npTextureGenerator::GetInstance() {
}
*/

npTextureGenerator::npTextureGenerator():apkRootPath(NULL),apkArchive(NULL),apkZipFile(NULL){
}

npTextureGenerator::~npTextureGenerator() {
	Releasing();
}

void npTextureGenerator::Releasing(){
	NP_SAFE_DELETE_ARR(this->apkRootPath);
	NP_SAFE_FREE(this->apkArchive);
}
