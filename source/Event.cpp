// File: Event.cpp

#include "Event.h"
#include "KernelEv.h"

/*methods - Event class*/

Event::Event(IVTNo ivtNo) { myImpl = new KernelEv( ivtNo ); }

int Event::wait() { return myImpl->wait(); }
	
void Event::signal() { myImpl->signal(); }

Event::~Event() { delete myImpl; }