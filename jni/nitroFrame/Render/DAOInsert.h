/*
 * DAOInsert.h
 *
 *  Created on: 2012. 10. 23.
 *      Author: Ophelia
 */

#ifndef DAOINSERT_H_
#define DAOINSERT_H_

#include "../NitroFrame.h"
#include "FramePalette.h"
#include "ScreenplayStore.h"
/**
 * @author 박성민
 * @brif Container와의 통신을 위한 module 중 하나로 Insert에 관련된 기능을 수행한다.
 * InsertModule은 Texture Generater와 XML Parser와 관련이 깊다.
 */
class npInsertModuleDAO{
public:
	//Logical 더미 테스팅용
	void InsertDummyRawSource();		//Testing용 Dummy를 삽입한다.
	void DeleteDummyRawSource();		//Testing용 Dummy를 하나 지운다.
	void PrintDummyRawSource();		//삽입된 Dummy의 ID를 Print한다.

	npInsertModuleDAO();
	~npInsertModuleDAO();
};



#endif /* DAOINSERT_H_ */
