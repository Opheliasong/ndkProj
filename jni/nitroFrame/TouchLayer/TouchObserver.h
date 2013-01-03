/*
 * TouchObserver.h
 *
 *  Created on: 2012. 11. 7.
 *      Author: Ophelia
 */

#ifndef TOUCHOBSERVER_H_
#define TOUCHOBSERVER_H_

class iTouchObserver {
	/***
	 * @author 박성민
	 * @brief 터치 이벤트를 받기 위한 interface 객체
	 */
public:
	iTouchObserver();
	virtual ~iTouchObserver()=0;
	virtual void notify()=0;
};

#endif /* TOUCHOBSERVER_H_ */
