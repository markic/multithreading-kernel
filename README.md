# multithreading-kernel
Multithreading operating system custom kernel with time sharing and thread scheduling. Semaphores and interrupt triggered events are implemented. Developed in C++ for intel 8086.
Borland C++, huge memory model.


This was a project for the course ”Operating systems 1” on my faculty.


Department of Computer Engineering and Information Theory.


School of Electrical Engineering, University of Belgrade, Serbia.


Developed by Marin Markić. No licence. April - September 2012.
- marinmarkic@mail.com


### How to use OS kernel: (see example)

 - Implement function userMain(argc, argv) in your user code, this is the main entry point for your application, arguments are passed from command line.

 - Create other user threads by extending OS class thread. You can specify thread name, thread stack size and time slice (minimum run time before thread can be interrupted). Implement custom thread code inside run method. Start thread by calling start method, thread will be started and run will be called on thread selection that depends on scheduler. 

 ### API:
```C++
 	* virtual void run() {}
 	* void start();
	* int waitToComplete(); // returns: 1 – run completed, 0 – woken up
	* int wakeUp(); // returns: 1 – deblocked, 0 – no effect
	* static int sleep(Time timeToSleep); // returns: 1 – sleep completed, 0 – woken up

	* ID getId();
	* TName getName(); 
  	* static Thread * getThreadById(ID id);
	* static ID getIdOf(TName name);
```
Semaphores:
 ```C++
	* Semaphore(int init = 1); // makes new instance

	* virtual int wait(); // blocks running thread on this semaphore
	* virtual void signal(); // release first blocked thread on this semaphore

	* int val () const;  // returns the current value of the semaphore
```
Events:
```C
	* PREPAREENTRY(ivtNo, callOldISR) // adds new interrupt service routine that signals event and optionally calls old ISR. Routine is added inside system interrupt vector table, on specified entry number, ivtNo)

	* Event(IVTNo ivtNo); // after calling prepareentry, create event instance
	* int wait(); // blocks running thread, thread will be unblocked when any interrupt is raised on ivtNo
```
