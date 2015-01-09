// File: PCB.h

#ifndef _PCB_H_
#define _PCB_H_

typedef unsigned int uint;

class KernelSem;
class Thread;

/*class PCB (actually thread control block)*/
class PCB {
public:
	uint sp;
	uint ss;
	uint *stack;
	
	int threadID;
	static int lastID;
	
	char* threadName;
	unsigned long stackSize;

	uint defaultTimeSlice;
	uint timeSlice;
	uint timeToSleep;

	uint isFinished;
	uint isBlocked;
	uint isSleeping;
	uint isWokenUp;
	
	Thread *parent;
	KernelSem *semaphore; //KerSem.h
	KernelSem *blockedOnSemaphore; //KerSem.h

	PCB(char* _threadName = "", unsigned long _stackSize, uint _timeSlice, Thread* _parent);
	
	void run();
	~PCB();
};

#endif