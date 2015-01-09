// File: IVTEntry.h

#define PREPAREENTRY(ivtNo, callOldISR)\
void interrupt eventISR##ivtNo(...) {\
	IVTEntry::getEntry( ivtNo )->signal();\
	if( callOldISR ) {\
		asm pushf;\
		( *IVTEntry::getOldISR( ivtNo ) )();\
		asm popf; }\
	dispatch();\
} IVTEntry ivtEntry##ivtNo(ivtNo, eventISR##ivtNo);


#define MAX_ENTRY 10

#ifndef _IVTENTRY_H_
#define _IVTENTRY_H_

class KernelEv;

class IVTEntry {
public:
	IVTEntry(int ivtNo, void interrupt (*handler)(...));
	
	void addEvent(KernelEv *e);
	void signal();
	void interrupt (*getOldISR())(...);

	static IVTEntry *getEntry(int ivtNo);
	static void interrupt (*getOldISR(int ivtNo))(...);

	~IVTEntry();

private:
	int ivtNo;
	void interrupt (*oldISR)(...);
	KernelEv *event;
};

#endif