#ifndef _KERNELEV_H_
#define _KERNELEV_H_

#include "event.h"
#include "krnlSem.h"
#include "thread.h"

class Event;
class KernelSem;
class Thread;

typedef unsigned char IVTNo;

class KernelEv {
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();
	void wait();
	void signal();
private:
	KernelSem *mySem;
	Thread* owner;
	IVTNo ivtNo;
};

#endif
