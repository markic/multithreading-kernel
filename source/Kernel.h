// File: Kernel.h

#include "Thread.h"

#ifndef _KERNEL_H
#define _KERNEL_H

//--------------------------- Lock / Unlock ------------------------------------------------------
#define lock asm{\
pushf;\
cli;\
}
#define unlock asm popf;

//--------------------------- System Mode --------------------------------------------------------
//TODO:
#define systemMod
#ifdef systemMod
#define systemModOn macroSpace
#define systemModOff macroSpace
#endif


//----------------------- Main system thread with userMain ---------------------------------------
class MainThread : public Thread {
public:
	virtual void run();
	MainThread(int _argc, char* _argv[]) : Thread( "mainThread" ), argc( _argc ),argv( _argv ) {}
private:
	int argc;
	char **argv;
};

//--------------------------------- Idle Thread --------------------------------------------------
class IdleThread : public Thread {
public:
	IdleThread() : Thread( "IdleThread" ) {}
	virtual void run();
};

//----------------------------- Kernel Prototypes -------------------------------------------------
int userMain(int, char*[]);
void tick();

void kernelRun();

void setTimerISR();
void interrupt timerISR();
void restoreTimerISR();


#endif