// File: Queue.h

#ifndef _QUEUE_H_
#define _QUEUE_H_

/*get does NOT remove PCB from the Queue, use remove instead*/

class PCB;

/*class Queue*/
class Queue {
public:
	Queue():head( 0 ),tail( 0 ) {}

	int isEmpty() { return (tail == 0) ? 1 : 0; }
	int size();
	int getIdByName(char *name);
	
	void push(PCB* _pcb);
	
	PCB* getPCBById(int id);
	PCB* removePCBById(int id);
	PCB* removeFirst();
	PCB* removeLast();

	~Queue();

	//iterator
	void set();
	void next();
	PCB* get();

private:
	struct NODE
	{
		PCB *myPCB;
		NODE *next;
		NODE *prev;
		NODE(PCB *_pcb): myPCB( _pcb ), next( 0 ), prev( 0 ) {}
	};

	NODE *head;
	NODE *tail;

	NODE *iterator;

};

#endif