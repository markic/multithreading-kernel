// File: KernelEv.h

#ifndef _KERNELEV_H_
#define _KERNELEV_H_

class PCB;

class KernelEv {
public:
	KernelEv(int _ivtNo);
	~KernelEv();

	int wait();
	void signal();

	int getIvtNo() const { return ivtNo; }

private:
	int ivtNo;
	int value;
	PCB* blocked;
};

#endif