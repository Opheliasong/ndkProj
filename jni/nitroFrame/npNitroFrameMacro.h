/*
 * npNitroFrameMacro.h
 *
 *  Created on: 2012. 10. 29.
 *      Author: Ophelia
 */

#ifndef NPNITROFRAMEMACRO_H_
#define NPNITROFRAMEMACRO_H_

/**
 * Nitro Frame에서 사용하는 Macro 함수들을 선언하여두는 부분
 * Macro 함수들만 정의한다!
 * 나머지 필요한 Define들은 Typedef에서 처리 하자
 */

#define NP_SAFE_DELETE(T) if(T != NULL){delete T;}
#define NP_SAFE_DELETE_ARR(T_ARRAY) if(T_ARRAY != NULL){delete[] T_ARRAY;}

#define NP_SAFE_FREE(T) if(T != NULL){free(T);}

/**
 * 클래스 Property관련 매크로 함수
 */

/***
 * @fn
 * @param varType
 * @param  varName
 * @param  funName
 * @warning
 */
#define NP_DEFINE_PROPERTY(varType, varName, funcName)\
	protected: varType varName;\
	public: virtual varType get##funcName(void){return varName;}\
	public: virtual void set##funcName(varType var){varName = var;}


/***
 * @fn
 * @param varType
 * @param  varName
 * @param  funName
 * @warning
 */
#define NP_DEFINE_PROPERTY_GET_BY_REF(varType, varName, funcName)\
		protected: varType varName;\
		public: virtual const varType& get##funcName(void){return &this->varName}\
		public: virtual void set##funcName(const varType& var){this->varName = var};

/***
 * @fn
 * @param varType
 * @param varName
 * @param funName
 * @warning
 */

#define NP_DEFINE_PROPERTY_READONLY(varType, varName, funcName)\
protected: varType varName;\
public: virtual varType get##funcName(void){return this->varName;};

/***
 * @fn
 */
#define NP_DEFINE_PROPERTY_READONLY_GET_BY_REF(varType, varName, funcName)\
		protected: varType varName;\
		public: virtual const varType& get##funcName(void){return &this->varName};


#define NP_IS_EMPTY(T) T==NULL
#define NP_IS_NOT_EMPTY(T) T!=NULL



#endif /* NPNITROFRAMEMACRO_H_ */
