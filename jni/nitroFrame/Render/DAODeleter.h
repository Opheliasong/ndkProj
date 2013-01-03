/*
 * DAODeleter.h
 *
 *  Created on: 2012. 10. 26.
 *      Author: Ophelia
 */

#ifndef DAODELETER_H_
#define DAODELETER_H_

#include <GLES/gl.h>
#include <GLES/glext.h>
#include "../xmlParser/npDeleterParser.h"
#include "TextureAtlas.h"
#include "ScreenplayStore.h"
#include "npRenderTypedef.h"

class npDeleteDAO {
	/***
	 * @author 박성민
	 * @brief DAO객체중 Container 들의 삭제를 담당하는 객체
	 * @details Scene 이 변경 될 때 DAO Interface 를 통해 Deleter를 호출하게 되면,
	 * Deleter는 Scene을 연출 하기 위해 사용된 texture XML들을 읽어들이면서 객체를 Container에 담겨져 있는 내용을 삭제 한다.
	 *
	 */
public:
	npDeleteDAO();
	~npDeleteDAO();

	void DeleteFrameByTag(const screenplayTag& deleteTag); //ContainerDAO 에서 호출할 Module 실행 함수

private:
	bool IsFindingTagFromScreenplayStore(screenMapIter& iterator);
};


#endif /* DAODELETER_H_ */
