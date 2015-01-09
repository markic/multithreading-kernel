// File: PCB.cpp

#include <string.h>
#include <stdlib.h>
#include <dos.h>

#include "PCB.h"
#include "Kernel.h"
#include "KerSem.h"


int PCB::lastID = -1;

/*methods - PCB class*/

PCB::PCB(char* _threadName, unsigned long _stackSize, uint _timeSlice, Thread *_parent)
		:stackSize( _stackSize ), timeSlice( _timeSlice ), defaultTimeSlice( _timeSlice ), 
		threadID( ++lastID ), parent( _parent ), stack(0) {

	timeToSleep = isFinished = isBlocked = isWokenUp = isSleeping = 0;
	
	if((strcmp(_threadName, "") == 0) || _threadName == 0) {
		threadName = new char[20];
		strcpy(threadName, "thread");
		
		char *temp = new char[20];
		temp = itoa(threadID, temp, 10);
		
		strcat(threadName, temp);
	}
	else {
		threadName = new char[strlen( _threadName ) + 1];
		strcpy(threadName, _threadName);
	}

	semaphore = new KernelSem(0);
	blockedOnSemaphore = 0;
	
	if(threadID == 0) return;

	//creating stack for all except mainThread
	stack = new unsigned[stackSize];
	
	stack[stackSize - 1] = 0x200; //PSW
	stack[stackSize - 2] = FP_SEG( kernelRun ); //CS
	stack[stackSize - 3] = FP_OFF( kernelRun ); //IP

	sp = FP_OFF(stack + stackSize - 12); // 3 x 32bit
	ss = FP_SEG(stack + stackSize - 12);  // 3 x 32bit
}


void PCB::run() {
	parent->run();
	
	semaphore->signalAll();
	isFinished = 1;	
	
	dispatch(); //Thread.h
}

PCB::~PCB() { 
	if(stack) delete stack; 
	delete semaphore; 
}