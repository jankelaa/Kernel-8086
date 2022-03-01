#include "pcb.h"
#include "system.h"
#include "SCHEDULE.H"
#include <dos.h>

#define MAX_STACK_SIZE 65535

PCB::PCB(Thread* myT, StackSize stackSize, Time timeSlice)
	: state(NEW), pcbStackSize(stackSize), 	myThread(myT), 	pcbStack(0), pcbTimeSlice(timeSlice), timedOut(0), timeToWait(0) {
	lock();
	parent = (PCB*)System::running;
	System::queuePCB->put(this);

	if (pcbStackSize > MAX_STACK_SIZE) pcbStackSize = MAX_STACK_SIZE;
	pcbStackSize /= sizeof(unsigned);

	myPcbId = System::systemId++;

	wtcQueue = new QueuePCB(); //wait to complete red
	unlock();
}

PCB::~PCB() {
	lock();
	delete wtcQueue;
	delete pcbStack;
	unlock();
}

void PCB::wrapper() {
	System::running->myThread->run();

	lock();
	System::running->state = PCB::FINISHED;

	PCB* temp = System::running->wtcQueue->get();			//nit se zavrsila, i sada vraca u scheduler niti koje su na nju cekale
	while (temp != 0) {
		temp->state = PCB::READY;
		Scheduler::put(temp);
		temp = System::running->wtcQueue->get();
	}

	dispatch();
}

void PCB::createStack() {
  	lock();
	pcbStack = new unsigned[pcbStackSize];

	pcbStack[pcbStackSize-1] = 0x200;				//setovan flag I
	pcbStack[pcbStackSize-2] = FP_SEG(&wrapper);	//segmentni deo wrapper
	pcbStack[pcbStackSize-3] = FP_OFF(&wrapper);	//offsetni deo wrapper

	sp = FP_OFF(pcbStack+pcbStackSize-12);
	ss = FP_SEG(pcbStack+pcbStackSize-12);
	bp = sp;
	unlock();
}
