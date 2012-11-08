/*
 * pbLinkNode.h
 *
 *  Created on: 2012. 9. 6.
 *      Author: NitroPigSoft02
 */

#ifndef NPLINKNODE_H_
#define NPLINKNODE_H_

#include "npDebugUtill.h"


template <typename T>
class npLinkNode{
	/***
	 * @author 박성민
	 * @brief 링크드 리스트를 사용 할 수 있게 하는 객체
	 * @details prev와 next 그리고 실제 Data를 말하는 kernel 을 가지고 있는 구조이다.
	 * prev와 next는 end-user 가 함부로 삭제 및 대입을 할수 없도록 private 로 선언되어 있다.
	 * ndk-test project(used 'C' project)에서 만들어진 것을 c++로 converting하면서 template class로 변화 시켰다.
	 * 기본 골격은 linux kernel 에서 사용하는 list 를 참조하여 제작하였으나 현재는 그 의미가 많이 퇴색되어져 있다.
	 * (typeof 매크로를 사용하게 될경우 매우 편리한 구조를 갖추게 되는데 typeof는 아쉽게도 gcc expend macro이다)
	 * @warning npLinkNode는 m_kernel 객체의 타입은 복사대입연산자가 deep copy가 된다는 가정하에서 사용하길 권고한다.
	 * swallow copy 의 경우 container 로서의 역할을 100% 수행하지 못하는 경우가 발생한다.
	 */

private:
	npLinkNode<T>* prev;
	npLinkNode<T>* next;
	T m_kernel;

public:
	npLinkNode():next(NULL),prev(NULL){this->setHeader();};

	npLinkNode(T* kernelInstance){
		this = makeLinkNode(*kernelInstance);
	}

	npLinkNode(T kernelInstance){
		this = makeLinkNode(kernelInstance);
	}

	npLinkNode(const npLinkNode<T>& deepCopyTarget){
		/***
		 * @author 박성민
		 * @fn npLinkNode의 copy constructor. list의 deep copy를 위하여 설정.
		 * @details copyList를 사용하는데 이는 전체 리스트를 카피하므로 loop logic이 들어가 있다.
		 * 속도의 문제가 분명 존재하며, 이를 처리할 방편을 마련해야 할지도 모른다.
		 */
		copyList(this,&deepCopyTarget);
	}
	~npLinkNode(){};

	//해당 Node를 리스트의 헤더로 설정한다. 리스트를 사용하기 위해서는 꼭 해주어야 하는 작업.
	void setHeader(){
		this->next = this;
		this->prev = this;
	}

	//해당 노드를 더한다.
	void add(npLinkNode<T>* pAdder){
		this->next = pAdder;
		pAdder->prev = this;
	}

	//List의 제일 끝(Tail)에 해당 노드를 추가하는 작업
	static void addTail(npLinkNode<T>* pAdder, npLinkNode<T>* pHeader){
		npLinkNode::list_add(pAdder,pHeader->getPrev(),pHeader);
	}

	static void addTail(T addKernal, npLinkNode<T>* pHeader){
		/***
		 * @fn Kernal값만으로 List에 addTail을 할 수 있는 함수
		 * @param addKernal List의 Kernal값을 말한다. 이때 addKernal은
		 * @param pHeader
		 */
		list_add(makeLinkNode(addKernal),pHeader->prev,pHeader);
	}

	static npLinkNode<T>* remove(npLinkNode<T>* node)
	{
		if(node->next 	!= NULL && node->prev != NULL){
			node->next->prev = node->prev;
			node->prev->next = node->next;
		}

		node->prev = NULL;
		node->next = NULL;

		return node;
	}

	//삭제할 대상을 List(Head)에서 찾아내어 삭제한다.
	static void findRemove(npLinkNode<T>* remover, npLinkNode<T>* head)
	{
		npLinkNode<T>* iterator;
		for(iterator =  head->next; iterator != head; iterator = iterator->next){
			if(iterator->m_kernel == remover->m_kernel){
				remove(iterator);
				return;
			}
		}
	}

	static void findRemove(T removeKernel, npLinkNode<T>* head){
		npLinkNode<T>* iterator;
		for(iterator = head->next; iterator != head; iterator = iterator->next){
			if(iterator->m_kernel == removeKernel ){
				remove(iterator);
			}
		}
	}

	//삭제할 대상을 List(Head)에서 찾아내어 삭제한다.
	static void findDelete(npLinkNode<T>* remover, npLinkNode<T>* head)
	{
		npLinkNode<T>* iterator;
		for(iterator =  head->next; iterator != head; iterator = iterator->next){
			if(iterator->m_kernel == remover->m_kernel){
				destroyNode(iterator);
				return;
			}
		}
	}

	static void findDelete(T* remover, npLinkNode<T>* head)
	{
		npLinkNode<T>* iterator;
		for(iterator =  head->next; iterator != head; iterator = iterator->next){
			if(iterator->m_kernel == remover){
				destroyNode(iterator);
				return;
			}
		}
	}

	static void destroyNode(npLinkNode<T>* destroyTarget){
		if(destroyTarget == 0){
			return;
		}
		remove(destroyTarget);
		delete destroyTarget;
	}

	static void destroyList(npLinkNode<T>* destroyListHead){
		if(destroyListHead == 0){
			return;
		}
		npLinkNode* iterator;

		for(iterator = destroyListHead->next; iterator != destroyListHead; ){
			npLinkNode<T>* nextTarget = iterator->prev;
			destroyNode(iterator);
			iterator = nextTarget;
		}

		//list의 Head를 삭제한다.
		delete destroyListHead;
	}

	T& getKernel(){
		return this->m_kernel;
	}

	static npLinkNode* makeLinkNode(T pInstance){
		/***
		 * @fn LinkNode를 새로 생성(Allocate)하여 return하는 함수, 실제 Instance의 Pointer를 넘기게 된다.
		 * @param pInstance
		 * @return 생성된 LinkNode를 말한다.
		 * @warning pInstance로 넘겨지는 객체는 복사 대입 연산자를 사용하게 되므로
		 * 반드시 Deep copy가 가능한 복사대인 연사자 객체를 사용하길 바란다.
		 */
		npLinkNode<T>* newOne = new npLinkNode<T>;
		newOne->setKernel(pInstance);
		return newOne;
	}

	void setKernel(T pInstance){
		/***
		 * @fn LinkNode의 Kernel값을 설정하는 함수.
		 * @param pInstance 복사 대입을 할 Instace객체.
		 * @warning 복사대입연산자가 호출되게 되는데 반드시 Deep copy가 가능한 객체를 사용하길 바란다.
		 * @details 기존에 사용하던 setNote()함수가 변경, Note는 Kernel이라는 명칭으로 교체되고, getter와 setter가 변경되었다.
		 */

		this->m_kernel = pInstance;
	}

	npLinkNode* getNext(){
		return this->next;
	}
	npLinkNode* getPrev(){
		return this->prev;
	}

	static bool IsEmpty(npLinkNode<T>* Queue) {
		return Queue->getNext() == Queue;
	}

	static void copyList(npLinkNode<T>* destList,const npLinkNode<T>* sourceList){
		/***
		 * @fn destList가 빈 리스트라는 가정하에 sourceList의 Kernel 내용을 destList로 복사하는 기능을 하는 함수
		 * @param destList 복사될 대상 list 객체
		 * @param sourceList 복사의 원본 list 객체
		 * @warning 반드시 destList는 빈 리스트여야 한다. 만약 내용이 체워져 있는 list 라면 위험하다.
		 * 내부에서 destList를 초기화(kernel NULL주입, setHeader호출)을 한다. 반드시 비워진 list만 copyList를 사용해야 한다.
		 */
		destList->setKernel(NULL);
		destList->setHeader();

		npLinkNode<T>* iter = const_cast<npLinkNode<T>*>(sourceList);
		destList->setKernel(iter->getKernel());
		iter = iter->next;
		for(; iter != sourceList; iter = iter->next){
			addTail(makeLinkNode(iter->getKernel()),destList);
		}
	}


private:
	static void list_add(npLinkNode* newOne, npLinkNode* prev,npLinkNode* next){
		next->prev = newOne;
		newOne->next = next;
		newOne->prev = prev;
		prev->next = newOne;
	}
};



#endif /* PBLINKNODE_H_ */
