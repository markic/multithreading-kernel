// File: KerSem.cpp

#include "KerSem.h"
#include "PCB.h"
#include "Schedule.h"
#include "Kernel.h"
#include "Queue.h"

extern PCB* running; //Kernel.cpp
extern int semPreempt; //User.cpp

/*methods - KernelSem class*/

KernelSem::KernelSem(int init): value( init ) { blocked = new Queue(); }

int KernelSem::val() const { return value; }

//----------------------------- Wait -----------------------------------------
int KernelSem::wait() {
	lock
	if(--value < 0) {
		
		running->isBlocked = 1;
		blocked->push( running );

		unlock
		dispatch();

		if( !running->isWokenUp ) return 1; //deblocked on signal
		
		lock
		running->isWokenUp = 0;
		unlock
		
		return 0; //woken up	
	}
	
	else if( semPreempt ) dispatch();
	unlock
	return 1;
}

//----------------------------- Signal -----------------------------------------
void KernelSem::signal() { 
	lock
	if(++value <= 0) {
		
		PCB *temp = blocked->removeFirst();
		temp->isBlocked = 0;
		temp->blockedOnSemaphore = 0;
		Scheduler::put( temp );	

	}
	unlock
	if( semPreempt ) dispatch();
}

//----------------------------- SignalAll -----------------------------------------
void KernelSem::signalAll() {
	lock
	
	while( blocked->size() ){
		PCB *temp = blocked->removeFirst();
	
		if(temp) { 
			temp->isBlocked = 0;
			Scheduler::put( temp );
		}
	}
	
	unlock
}
	
KernelSem::~KernelSem() { delete blocked; }