#include "ivtEntry.h"
#include <dos.h>
#include "kernelEv.h"
#include "system.h"

IVTEntry* IVTEntry::IVTable[256] = {0};

IVTEntry::IVTEntry(int ivtNo, void interrupt (*newRoutine)(...)) {
	lock();
	old = getvect(ivtNo);
	setvect(ivtNo, newRoutine);
	myKernelEv = 0;
	this->ivtNo = ivtNo;
	IVTable[ivtNo] = this;
	unlock();
}

IVTEntry::~IVTEntry() {
	lock();
	old();
	setvect(ivtNo, old);
	unlock();
}

void IVTEntry::signal() {
	if (myKernelEv) myKernelEv->signal();
}

void IVTEntry::callOldInterrupt() {
	old();
}

