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
	sprintf(pngPath,"assets/sprite/%s\0",aTextureName);
	LOGE("TextureGenerator)Texture Name :%s",pngPath);

	this->apkZipFile = zip_fopen(this->apkArchive,pngPath ,0);

	if(NP_IS_EMPTY(this->apkZipFile)){
		LOGE("TextureGenerator)Texture opening has error!!!");
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

	//init png reading
	png_set_read_fn(png_ptr, NULL, npTextureGenerator::png_zip_read);

	//png error stuff, not sure libpng man suggest this
	if(setjmp(png_jmpbuf(png_ptr))){
		zip_fclose(this->apkZipFile);
		LOGE("TextureGenerator)Error during set imp");
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		return TEXTURE_LOAD_ERROR;
	}



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

//	LOGE("rowbytes : %d",rowbytes);
//	LOGE("tHeights: %d", tHeight);
//	LOGE("tWeights: %d", tWidth);

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
/*	for(int i= tHeight -1;i>0;--i){
		row_pointers[tHeight - 1 - count] = image_data + i * rowbytes;
		count++;
	}*/

	for (int i = 0; i < tHeight; ++i) {
		row_pointers[i] = image_data + i * rowbytes;
	}

	/*
	for(int i= 0;i < tHeight;i++){
			row_pointers[tHeight - 1 - i] = image_data + i * rowbytes;
	}
	*/

	LOGE("Before png_read_image");
	//read the png info image_data through row_pointers
	png_read_image(png_ptr, row_pointers);

	//OpenGL Texture Generate
	GLuint textureID;
	LOGE("before glGenTexture Call");
	glGenTextures(1,&textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tWidth, tHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)image_data);
	glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//clean up memory and close stuff
	png_destroy_read_struct(&png_ptr,&info_ptr, &end_info);
	delete[] image_data;
	delete[] row_pointers;
	zip_fclose(this->apkZipFile);

	return textureID;
}

void npTextureGenerator::png_zip_read(png_structp png_ptr, png_bytep data, png_size_t length) {
	zip_fread(npContainerDAO::GetInstance().getTextureGenerator()->apkZipFile, data, length);
}

npTextureGenerator::npTextureGenerator(char* apkPath):apkRootPath(NULL),apkArchive(NULL),apkZipFile(NULL) {
	Initializing(apkPath);
}

void npTextureGenerator::Initializing(char* apkPath) {
	//this->assetManager = assetManager;

	size_t strLength = strlen(apkPath);
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

    //Just for debug, print APK contents
    int numFiles = zip_get_num_files(this->apkArchive);
    for (int i=0; i<numFiles; i++) {
        const char* name = zip_get_name(this->apkArchive, i, 0);
        if (name == NULL) {
        LOGE("Error reading zip file name at index %i : %s", zip_strerror(this->apkArchive));
        return;
        }
    //zip Files print LOG
    //LOGI("File %i : %s\n", i, name);
 	}
}

GLuint npTextureGenerator::GenerateTextureUseAssetManager(const char* textureName) {

}

/***
 * @fn libPNG 를 이용한 실제 PNG 파싱 작업
 * @return png파일을 통해 읽어온 내용의 array를 리턴한다
 * @details NDK Beginners의 내용을 토대로 작성한 함수.
 */
uint8_t* npTextureGenerator::loadImage() {
	png_byte header[8];
	png_structp PngPtr = NULL;
	png_infop InfoPtr = NULL;
	png_byte* ImageBuffer = NULL;
	png_bytep* RowPtrs = NULL;
	png_int_32 RowSize;
	bool Transparency;

	PngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(NP_IS_EMPTY(PngPtr)){
		LOGE("Not create pngPTR!");
		delete[] RowPtrs; delete[] ImageBuffer;
		if (PngPtr != NULL) {
		png_infop* lInfoPtrP = InfoPtr != NULL ? &InfoPtr: NULL;
		png_destroy_read_struct(&PngPtr, lInfoPtrP, NULL);
		}
		return NULL;
		return NULL;
	}
	InfoPtr = png_create_info_struct(PngPtr);
	if(NP_IS_EMPTY(InfoPtr)){
		LOGE("Not create Info PTR");
		delete[] RowPtrs; delete[] ImageBuffer;
		if (PngPtr != NULL) {
		png_infop* lInfoPtrP = InfoPtr != NULL ? &InfoPtr: NULL;
		png_destroy_read_struct(&PngPtr, lInfoPtrP, NULL);
		}
		return NULL;
	}

	png_set_read_fn(PngPtr,NULL,npTextureGenerator::png_zip_read);

	//Start Reading PNG file Header with png_read_info()
	//the first 8bytes read for file signature with png_set_sig_bytes().
	//PNG files can be encoded in several formats RGB, RGBA, 256 color with palette, grayscale ...
	png_set_sig_bytes(PngPtr,8);
	png_read_info(PngPtr,InfoPtr);

	png_int_32 depth, colorType;
	png_uint_32 texWidth, texHeight;
	png_get_IHDR(PngPtr,InfoPtr,&texWidth,&texHeight,&depth,&colorType,NULL,NULL,NULL);

	//Creates a full alpha channal if transparency is encoded as an array of palettes entries or a single transparent color.
	Transparency = false;
	if(png_get_valid(PngPtr,InfoPtr,PNG_INFO_tRNS)){
		png_set_tRNS_to_alpha(PngPtr);
		Transparency = true;
		delete[] RowPtrs; delete[] ImageBuffer;
		if (PngPtr != NULL) {
		png_infop* lInfoPtrP = InfoPtr != NULL ? &InfoPtr: NULL;
		png_destroy_read_struct(&PngPtr, lInfoPtrP, NULL);
		}
		return NULL;
	}

	//expends PNG with less than 8bits per channel to 8bits
	if(depth<8){
		png_set_packing(PngPtr);
		//Shrinks PNG with 16bits per color channel down to 8bits.
	}else if(depth == 16){
		png_set_strip_16(PngPtr);
	}
	//Indicates that image needs conversion to RGBA if need.
	switch(colorType){
	case PNG_COLOR_TYPE_PALETTE:
		png_set_palette_to_rgb(PngPtr);
		break;

	//RGB
	case PNG_COLOR_TYPE_RGB:
		break;

	//RGBA
	case PNG_COLOR_TYPE_RGBA:
		break;

	//Grayscale
	case PNG_COLOR_TYPE_GRAY:
		png_set_expand_gray_1_2_4_to_8(PngPtr);
		break;
	//Gray + Alpha
	case PNG_COLOR_TYPE_GA:
		png_set_expand_gray_1_2_4_to_8(PngPtr);
		break;
	}
	png_read_update_info(PngPtr,InfoPtr);

	//Allocate the necessary temporary buffer to hold image data and a second one with the address of each output image row for libPNG.
	//row order is inverted. because OpenGL uses a different coordinate system.
	//(first pixel is at bottom-left) then PNG(first pixel at top-left). Then start reading effectively image content with png_read_image()

	RowSize = png_get_rowbytes(PngPtr,InfoPtr);
	if(RowSize <= 0){
		LOGE("RowSize under zero!");
		delete[] RowPtrs; delete[] ImageBuffer;
		if (PngPtr != NULL) {
		png_infop* lInfoPtrP = InfoPtr != NULL ? &InfoPtr: NULL;
		png_destroy_read_struct(&PngPtr, lInfoPtrP, NULL);
		}
		return NULL;
	}
	ImageBuffer = new png_byte[RowSize * texHeight];
	if(NP_IS_EMPTY(ImageBuffer)){
		LOGE("ImageBuffer is empty!");
		delete[] RowPtrs; delete[] ImageBuffer;
		if (PngPtr != NULL) {
		png_infop* lInfoPtrP = InfoPtr != NULL ? &InfoPtr: NULL;
		png_destroy_read_struct(&PngPtr, lInfoPtrP, NULL);
		}
		return NULL;
	}
	for(int32_t i = 0; i < texHeight; ++i){
		RowPtrs[texHeight - (i + 1)] = ImageBuffer + i * RowSize;
	}
	png_read_image(PngPtr,RowPtrs);

	png_destroy_read_struct(&PngPtr, &InfoPtr, NULL);
	delete[] RowPtrs;
	return ImageBuffer;
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
