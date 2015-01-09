// File: Kernel.cpp

#include "Kernel.h"
#include "Schedule.h"
#include "Queue.h"
#include "PCB.h"

PCB *running, *idle, *temp;
Queue allThreads, sleeping;

static unsigned temp_sp, temp_ss;

static volatile int timeIsUp = 0;
static volatile int contextSwitchRequest = 0;

/*method run - MainThread - Kernel.h*/
void MainThread::run() { setTimerISR(); userMain(argc, argv); restoreTimerISR(); }

/*method run - IdleThread - Kernel.h*/
void IdleThread::run() { while(1); }

void kernelRun() { running->run(); }

/*dispatch - Thread.h*/
void dispatch() { 
	lock

	contextSwitchRequest = 1;
	timerISR();	
	
	unlock	
}

//------------------------------ Timer Interrupt Service Routine -----------------------------
void interrupt timerISR() {

	if(!contextSwitchRequest) {
		
		for(sleeping.set(); ( temp = sleeping.get() ) != 0; sleeping.next()) {

				if(--temp->timeToSleep == 0) {
					Scheduler::put( temp );
					sleeping.removePCBById( temp->threadID );
					temp->isSleeping = 0; 
				}
		}

		tick();
	}
	
	if(running->defaultTimeSlice != 0 && !contextSwitchRequest) {
		running->timeSlice--; 

		if(running->timeSlice == 0) {
			timeIsUp = 1;
			running->timeSlice = running->defaultTimeSlice;
		}

	} 

	if(contextSwitchRequest || timeIsUp) {
		//context switch 
		asm{
			mov temp_sp, sp
			mov temp_ss, ss
		}
		running->sp = temp_sp;
		running->ss = temp_ss;
		
	
		if(running && !running->isFinished && !running->isBlocked && !running->isSleeping)
			Scheduler::put( running );
		running = Scheduler::get();

		if( !running && sleeping.size() ) running = idle;

		temp_sp = running->sp;
		temp_ss = running->ss;

		running->timeSlice = running->defaultTimeSlice;		

		asm{
			mov sp, temp_sp
			mov ss, temp_ss
		}
	}
	
	timeIsUp = 0;

	if(!contextSwitchRequest) asm int 60h;
	contextSwitchRequest = 0;
}

//--------------------------------------------Set and Restore - Kernel.h---------------------------

static unsigned oldTimerSEG, oldTimerOFF;

void setTimerISR() {
	asm{
		cli
		push es
		push ax
  	
  		mov ax, 0   
    	mov es, ax

		//save old ISR
		mov ax, word ptr es:0022h 
		mov word ptr oldTimerSEG, ax
		
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax
		
		//set new ISR
		mov word ptr es:0022h, seg timerISR
		mov word ptr es:0020h, offset timerISR
		
		//set old ISR on int 60h
		mov ax, oldTimerSEG 
		mov word ptr es:0182h, ax 
		
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax
	  	
	  	pop ax
		pop es
		sti
	}
}

void restoreTimerISR() {
	asm{
		cli
		push es
		push ax
		
		mov ax, 0
	    mov es, ax

	    //restore oldTimer
	    mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
}