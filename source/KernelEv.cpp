// File: KernelEv.cpp

#include "KernelEv.h"
#include "Schedule.h"
#include "PCB.h"
#include "Kernel.h"
#include "IVTEntry.h"

extern PCB* running; //Kernel.cpp
extern IVTEntry* ivtList[MAX_ENTRY]; //IVTEntry.cpp

/*methods - KernelEv class*/

KernelEv::KernelEv(int _ivtNo) {
	lock
	
	ivtNo = _ivtNo;
	value = 0;

	ivtList[ivtNo]->addEvent(this);

	unlock
}

int KernelEv::wait() { 
	lock
	if(value == 0) {
		running->isBlocked = 1;
		blocked = running;

		unlock
		dispatch();

		if( !running->isWokenUp ) return 1; //deblocked on signal
		
		lock

		running->isWokenUp = 0;
		if(blocked) {
			value = blocked->isBlocked = 0;
			blocked = 0;
		}
		
		unlock
		return 0; //woken up
	}

	value = 0;
	unlock
	return 1;
}

void KernelEv::signal() {
	lock

	value = 1;
	
	if(blocked) {
		value = blocked->isBlocked = 0;
		Scheduler::put( blocked );
		blocked = 0;
	}

	unlock
}

KernelEv::~KernelEv() { }