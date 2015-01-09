// File: Event.h

#include "IVTEntry.h"

#ifndef _EVENT_H_
#define _EVENT_H_

typedef unsigned char IVTNo;

class KernelEv;

class Event {
public:
	Event(IVTNo ivtNo);
	~Event();

	int wait();
	void signal();
private:
	KernelEv* myImpl;
};

#endif