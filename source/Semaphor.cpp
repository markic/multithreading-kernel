// File: Semaphor.cpp

#include "Semaphor.h"
#include "KerSem.h"

/*methods - Semaphore class*/
Semaphore::Semaphore(int init) { myImpl = new KernelSem( init ); }

int Semaphore::wait() { return myImpl->wait(); } 

void Semaphore::signal() { myImpl->signal(); }

int Semaphore::val() const { return myImpl->val(); }

Semaphore::~Semaphore() { delete myImpl; }