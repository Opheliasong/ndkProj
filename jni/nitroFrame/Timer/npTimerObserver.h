/*
 * npTimerObserver.h
 *
 *  Created on: 2012. 11. 26.
 *      Author: Ophelia
 */

#ifndef NPTIMEROBSERVER_H_
#define NPTIMEROBSERVER_H_

class npTimerObserver {
public:
	npTimerObserver();
	virtual ~npTimerObserver()=0;

	virtual void onTimeAlerts()=0;
};

#endif /* NPTIMEROBSERVER_H_ */
