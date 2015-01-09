// File: Queue.cpp

#include <string.h>

#include "Queue.h"
#include "PCB.h"
#include "Kernel.h"

/*methods - Queue class*/

void Queue::push(PCB* _pcb) {
	lock

	NODE *newNode = new NODE( _pcb );
	
	if(!head) head = newNode;
	
	if(!tail) tail = newNode;
	else {
		tail->next = newNode;
		newNode->prev = tail;
		tail = newNode;
	}

	unlock
}

int Queue::size() {
	lock

	int counter = 0;

	for(NODE *it = head; it != 0; it = it->next)
		counter++;

	unlock
	return counter;
}

PCB* Queue::getPCBById(int id) {
	lock
	
	for(NODE *it = head; it != 0; it = it->next)
		if(it->myPCB->threadID == id) {
			unlock
			return it->myPCB;
		}			
	
	unlock
	
	return 0;
}

PCB* Queue::removePCBById(int id) {
	if(!head) return 0;
	lock
	
	PCB* result;
	
	for(NODE *it = head; it != 0; it = it->next)
		if(it->myPCB->threadID == id) {
			result = it->myPCB;
			
			if(head == it) head = head->next;
			if(tail == it) tail = tail->prev;
			
			it->prev->next = it->next;
			it->next->prev = it->prev;

			delete it;
			
			unlock
			
			return result;
		}	
	
	unlock
	
	return 0;
}

PCB* Queue::removeFirst() {
	if(!head) return 0;
	lock

	PCB* result = head->myPCB;
	NODE *old = head;

	if(head == tail) { head = 0; tail = 0; }
	else {head = head->next; head->prev = 0; }

	delete old;

	unlock

	return result;
}


PCB* Queue::removeLast() {	
	if(!head) return 0;
	lock

	PCB* result = tail->myPCB;
	NODE *old = tail;

	if(head == tail){ head = 0; tail = 0; }
	else { tail = tail->prev; tail->next = 0; }

	delete old;

	unlock

	return result;
}

int Queue::getIdByName(char *name) {
	lock
	
	for(NODE *it = head; it != 0; it = it->next)
		if(strcmp(it->myPCB->threadName, name) == 0) {
			unlock
			return it->myPCB->threadID;
		}
			
	unlock
	
	return 0;
}

Queue::~Queue() {
	lock

	NODE *it = head;

	while(it != 0) {
		it = head->next;
		delete head;
		head = it;
	}
	
	tail = 0;

	unlock
}

//iterator
void Queue::set() { iterator = head; }

void Queue::next() { iterator = iterator->next; }

PCB* Queue::get() {
	if(iterator == 0) return 0;
	return iterator->myPCB;
}