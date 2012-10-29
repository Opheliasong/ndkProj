/*
 * npRentalMemory.h
 *
 *  Created on: 2012. 10. 21.
 *      Author: Ophelia
 */

#ifndef NPRENTALMEMORY_H_
#define NPRENTALMEMORY_H_
#include "NitroFrame.h"
//--------------------------------노트 메모리 빌려주는 클래스 -----------------------------------------------------------------------------------//
// 계약사항 : 이 클래스를 사용하는 클래스에 반드시  public 변수 int m_RentalIndex; 를 정의 해 놓아야 한다
template <typename T>
class pbMemoryRentalUnit{
public:
	pbMemoryRentalUnit();
	~pbMemoryRentalUnit();

	void Initialize(int Size);
	void Release();

	//자원 빌려주기
	T* RentalMemory();
	//자원 반환받기
	void ReturningRentalMemory(T* pObject);

private:
	T* m_pMemoryStore;
	int m_UnitSize;
	int m_iRentaltIndex;
	int m_iLastDestroyedIndex;
	int* m_piDestroyedIndexArray;
};

template <typename T>
pbMemoryRentalUnit<T>::pbMemoryRentalUnit(){
	m_pMemoryStore = NULL;
	m_UnitSize = 0;
	m_iRentaltIndex = 0;
	m_iLastDestroyedIndex = 0;
	m_piDestroyedIndexArray = NULL;
}
template <typename T>
pbMemoryRentalUnit<T>::~pbMemoryRentalUnit(){
	//Release();
}

template <typename T>
void pbMemoryRentalUnit<T>::Initialize(int Size){
	if( m_pMemoryStore != NULL)
		delete[] m_pMemoryStore;
	m_pMemoryStore = new T[Size];
	//LOGI("pbMemoryRentalUnit : Memory Create");
	if( m_piDestroyedIndexArray != NULL)
		free(m_piDestroyedIndexArray);
	m_piDestroyedIndexArray = (int*)calloc(Size, sizeof(int));
	//LOGI("pbMemoryRentalUnit : Index Create");
	m_UnitSize = Size;
	m_iRentaltIndex = 0;
	m_iLastDestroyedIndex = -1;
}
template <typename T>
void pbMemoryRentalUnit<T>::Release(){
	if( m_pMemoryStore != NULL)
	{
		delete[] m_pMemoryStore;
	}

	if( m_piDestroyedIndexArray != NULL)
		free(m_piDestroyedIndexArray);

	m_pMemoryStore = NULL;
	m_piDestroyedIndexArray = NULL;
	//LOGI("RentalUnit Release");
}

// 계약사항 : 이 클래스를 사용하는 클래스에 반드시  public 변수 int m_RentalIndex; 를 정의 해 놓아야 한다
template <typename T>
T* pbMemoryRentalUnit<T>::RentalMemory(){
	if(m_pMemoryStore == NULL){
		//LOGE("MemoryRentalUnit is Not Initialized!!");
		return NULL;
	}


	int index = -1;

	if( m_iLastDestroyedIndex != -1)
	{
		int laseIndex = m_iLastDestroyedIndex;
		index = m_piDestroyedIndexArray[laseIndex];

		m_iLastDestroyedIndex--;

	}
	else if(m_iRentaltIndex < m_UnitSize)
	{
		index = m_iRentaltIndex;
		m_iRentaltIndex++;
	}
	if( index == -1)
	{
		//LOGE("pbNoteRentalUnit : Note Rental is Full ");
		return NULL;
	}

	m_pMemoryStore[index].m_RentalIndex = index; //빌려간 인덱스 마킹

	return &(m_pMemoryStore[index]);
}
template <typename T>
void pbMemoryRentalUnit<T>::ReturningRentalMemory(T* pObject	){
	int RentalIndex = pObject->m_RentalIndex;
	m_iLastDestroyedIndex++;
	int lastIndex = m_iLastDestroyedIndex;
	m_piDestroyedIndexArray[lastIndex] = RentalIndex;

	pObject->m_RentalIndex = -1;
}

//======================================================================================//


#endif /* NPRENTALMEMORY_H_ */
