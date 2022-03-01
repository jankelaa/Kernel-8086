#include "kernelEv.h"
#include "ivtEntry.h"
#include "system.h"
#include "krnlSem.h"

KernelEv::KernelEv(IVTNo ivtNo) {
	lock();
	if(IVTEntry::IVTable[ivtNo]) {
		owner = System::running->myThread;
		this->ivtNo=ivtNo;
		mySem = new KernelSem(0);
		IVTEntry::IVTable[ivtNo]->myKernelEv = this; 	//povezujemo event sa odgovarajucim objektom klase ivtEntry
	}													//dodeljujemo ovaj event ulazu ivtNo u IV tabeli
	unlock();
}

KernelEv::~KernelEv () {
	lock();
	IVTEntry::IVTable[ivtNo]->myKernelEv = 0;
	delete mySem;
	unlock();
}

void KernelEv::wait () {
	lock();
	if (System::running->myThread == owner) {		//po postavci samo nit koja je napravila event moze na njemu da se blokira
		mySem->wait(0);
	}
	unlock();
}

void KernelEv::signal() {
	lock();
	if (mySem->value < 1) {
		mySem->signal();
	}
	unlock();
}
