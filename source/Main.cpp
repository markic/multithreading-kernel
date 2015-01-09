// File: Main.cpp

#include "PCB.h"
#include "Kernel.h"
#include "Schedule.h"
#include "Queue.h"

static MainThread *mainThread; 

extern PCB *running, *idle; //Kernel.cpp
extern Queue allThreads, sleeping; //Kernel.cpp

static unsigned system_sp, system_ss;

//--------------------------- Main Entry Point for this application-----------------------------------

int main(int argc, char* argv[]) {
	
	MainThread *mainThread = new MainThread(argc, argv);
	mainThread->start();
	running = Scheduler::get();
	
	IdleThread *idleThread = new IdleThread();
	idleThread->start();
	idle = Scheduler::get();
	idle->isFinished = 1; //never put in Scheduler
	idle->defaultTimeSlice = 1;

	//saving system context to system stack
	asm{
		push ax
		push bx
		push cx
		push dx
		push es
		push ds
		push si
		push di
		push bp
		mov system_sp, SP
		mov system_ss, SS
	}
	
	mainThread->run();

	//restoring system context
	asm{
	 	mov SP, system_sp
	 	mov SS, system_ss
	 	pop bp
	 	pop di
	 	pop si
	 	pop ds
	 	pop es
	 	pop dx
	 	pop cx
	 	pop bx
	 	pop ax
	}

	delete mainThread;
	delete idleThread;

    return 0;
}