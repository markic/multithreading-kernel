// File: Thread.cpp

#include "PCB.h"
#include "Schedule.h"
#include "KerSem.h"
#include "Queue.h"
#include "Kernel.h"

extern PCB *running; //Kernel.cpp
extern Queue allThreads, sleeping; //Kernel.cpp

/*methods - Thread class*/

Thread::Thread(TName name, StackSize stackSize, Time timeSlice) {	
	lock
	myPCB = new PCB(name, stackSize, timeSlice, this);
	unlock
	
	allThreads.push( myPCB );
}

ID Thread::getId() { return myPCB->threadID; }

TName Thread::getName() { return myPCB->threadName; }

void Thread::start() { lock Scheduler::put( myPCB ); unlock }

Thread* Thread::getThreadById(ID id) { return allThreads.getPCBById( id )->parent; }

ID Thread::getIdOf(TName name) { return allThreads.getIdByName( name ); }

//----------------------------- Sleep -----------------------------------------
int Thread::sleep(uint timeToSleep) {
	lock
	running->timeToSleep = timeToSleep;
	running->isSleeping = 1;
	
	sleeping.push( running );
	unlock
	dispatch();
	
	//we will be here when this thread runs again
	if(running->timeToSleep > 0) return 0; //woken up
	return 1;
}

//----------------------------- WakeUp -----------------------------------------
int Thread::wakeUp() {
	if(!myPCB->isSleeping && !myPCB->isBlocked) return 0; //no effects
	
	lock

	if (myPCB->isBlocked) {
		myPCB->isWokenUp = 1;
		myPCB->isBlocked = 0;

		myPCB->blockedOnSemaphore->value++;
		myPCB->blockedOnSemaphore->blocked->removePCBById( myPCB->threadID );
	}

	if(myPCB->isSleeping) {
		myPCB->isSleeping = 0;
		sleeping.removePCBById( myPCB->threadID );
	}

	
	Scheduler::put( myPCB );
	unlock
	
	return 1;
}

//----------------------------- WaitToComplete -----------------------------------------
int Thread::waitToComplete() {
	if(this->myPCB == running) return -1;
	if(this->myPCB->isFinished) return 1;
	return this->myPCB->semaphore->wait();	
}

Thread::~Thread() {
	allThreads.removePCBById( this->getId() );
	delete myPCB; //no more stack
}
