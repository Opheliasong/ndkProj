/*
 * npSubject.h
 *
 *  Created on: 2012. 7. 10.
 *      Author: NitroPigSoft02
 */

#ifndef NPSUBJECT_H_
#define NPSUBJECT_H_
#include "npObserver.h"

class npSubject{
public:
	npSubject(){};
	virtual ~npSubject(){};

	virtual void registerObserver(npObserver* apObserver)=0;
	//virtual void removeObserver(npObserver* apObserver);
	virtual void notifyAllObservers()=0;
};


#endif /* NPSUBJECT_H_ */
