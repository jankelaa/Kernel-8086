#ifndef _PCB_H_
#define _PCB_H_

#include "queuePCB.h"
#include "thread.h"

class QueuePCB;
class Thread;

class PCB {
public:
	PCB(Thread* myT, StackSize stackSize, Time timeSlice);
	~PCB();

	static void wrapper();
	PCB* parent;
	Thread* myThread;

	unsigned* pcbStack;
	unsigned sp, ss, bp;

	enum PCBState { NEW, READY, BLOCKED, FINISHED, WANNA};
	volatile PCBState state;

	ID myPcbId;
	StackSize pcbStackSize;
	Time pcbTimeSlice;

	QueuePCB* wtcQueue;

	QueuePCB* friendsQueue;
	int cntFriends;
	int runningFriends;

	void createStack();

	Time timeToWait;
	int timedOut;
};

#endif
