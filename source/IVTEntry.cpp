// File: IVTEntry.cpp

#include <dos.h>

#include "IVTEntry.h"
#include "Kernel.h"
#include "KernelEv.h"

IVTEntry *ivtList[MAX_ENTRY];

/*methods - IVTEntry class*/

IVTEntry::IVTEntry(int _ivtNo, void interrupt (*handler)(...)) {
	lock
	
	oldISR = getvect( ivtNo = _ivtNo );
	setvect(ivtNo, handler);
	
	ivtList[ivtNo] = this;

	unlock
}

void IVTEntry::addEvent(KernelEv *e) { 
	lock
	event = e; 
	unlock
}

void IVTEntry::signal() { 
	lock
	if(event) 
		event->signal(); 
	unlock
}

void interrupt (* IVTEntry::getOldISR())(...) { return oldISR; }

//static
IVTEntry *IVTEntry::getEntry(int ivtNo) { return ivtList[ivtNo]; }
void interrupt (*IVTEntry::getOldISR(int ivtNo))(...) { return ivtList[ivtNo]->getOldISR(); }


IVTEntry::~IVTEntry() {
	lock
	setvect(ivtNo, oldISR);
	unlock
}