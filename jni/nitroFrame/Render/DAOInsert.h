/*
 * DAOInsert.h
 *
 *  Created on: 2012. 10. 23.
 *      Author: Ophelia
 */

#ifndef DAOINSERT_H_
#define DAOINSERT_H_

//#include "../NitroFrame.h"
#include "TextureAtlas.h"
#include "ScreenplayStore.h"
#include "TextureRawPacket.h"
#include "npRenderTypedef.h"


class npInsertDAO{
protected:
	class ScreenplayPacket{
	public:
		screenplayTag tag;
		int screenplayID;

		ScreenplayPacket& operator=(const ScreenplayPacket& rightSide){
			this->screenplayID = rightSide.screenplayID;
			this->tag = rightSide.tag;
			return *this;
		}
	};
public:
	//Logical 테스팅용 더미 함수
	void InsertDummyRawSource();		//Testing용 Dummy를 삽입한다.
	void DeleteDummyRawSource();		//Testing용 Dummy를 하나 지운다.
	void PrintDummyRawSource();		//삽입된 Dummy의 ID를 Print한다.

	npInsertDAO();
	~npInsertDAO();

	void SendRawSourceByModule(const RawDataPacket& rawSource);

private:
	void ParsingRawSource(const RawDataPacket& rawSource, UVPacket& outputUvPacket, ScreenplayPacket& screenOutputPacket);
	void CalcUVArrayByRawSource(const RawDataPacket& rawSource, UVPacket& UVoutputPacket);
	void InsertUVPacket(UVPacket& UvPacketsUvPackets,int UvKeys);	//
	void InsertScreenplayPackets(ScreenplayPacket& screenplayPacket);

	int GetFramewIndex();
};



#endif /* DAOINSERT_H_ */
