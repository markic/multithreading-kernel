// File: KerSem.h

#ifndef _KERNELSEM_H
#define _KERNELSEM_H

class Queue;

/*class KernelSem*/
class KernelSem {
public:
	KernelSem(int init);
	
	int val() const;
	int wait();
	void signal();
	void signalAll();
	
	~KernelSem();

	int value;
	Queue *blocked;
};



#endif