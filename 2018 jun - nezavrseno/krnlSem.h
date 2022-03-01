#ifndef _KRNLSEM_H_
#define _KRNLSEM_H_

#include "semaphor.h"
#include "queuePCB.h"

class Semaphore;
class QueuePCB;

class KernelSem {
public:
	KernelSem (int init=1);
	virtual ~KernelSem ();

	virtual int wait (Time maxTimeToWait);
	virtual int signal();

	virtual int waitFriend(int signaledByFriend);
	virtual void signalFriend();

	int threadsInCriticalSection;

	void decrementTimeToWait();
	int value;
	QueuePCB *waitBlocked, *timerBlocked;

};

#endif
