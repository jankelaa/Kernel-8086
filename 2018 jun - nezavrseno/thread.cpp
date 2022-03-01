#include "system.h"
#include "pcb.h"
#include "thread.h"
#include "schedule.h"

class System;
class Scheduler;

Thread::Thread(StackSize stackSize, Time timeSlice) {
	lock();
	myPCB = new PCB(this, stackSize, timeSlice);
	unlock();
}

Thread::~Thread() {
	lock();
	waitToComplete();
	delete myPCB;
	unlock();
}

void Thread::Makefriends(Thread* t1, Thread* t2){
	lock();
	t1->myPCB->makeFriend(t2->myPCB);
	t2->myPCB->makeFriend(t1->myPCB);
	unlock();
}

void Thread::start() {
	lock();
	myPCB->state = PCB::READY;
	myPCB->createStack();
	if (this != System::idle) Scheduler::put(myPCB);
	unlock();
}

void Thread::waitToComplete() {
	lock();

	PCB *p = System::running->parent;
	while (p != 0) {
		if (myPCB == p) { unlock(); return; }
		p = p->parent;
	}

	if (myPCB->state == PCB::FINISHED) { unlock(); return; }
	if (myPCB->state == PCB::NEW) { unlock(); return; }
	if (myPCB == System::running) { unlock(); return; }
	if (this == System::idle) { unlock(); return; }

	System::running->state = PCB::BLOCKED;
	myPCB->wtcQueue->put((PCB*)System::running);				//running se blokira i dodaje u niz wtc od this (nit koju pokusava da obrise)
	dispatch();													//oslobadjanje procesora
	unlock();
}

ID Thread::getId() { return myPCB->myPcbId;}

ID Thread::getRunningId() {
	ID ret;
	lock();
	ret = System::running->myPcbId;
	unlock();
	return ret;
}

Thread *Thread::getThreadById(ID id) {
	Thread* ret = 0;
	lock();
	for (ElemPCB *el = System::queuePCB->head; el; el=el->next)
		if (el->pointPCB->myPcbId == id) {
			ret = el->pointPCB->myThread;
			break;
		}
	unlock();
	return ret;
}

void dispatch() {
	lock();
	System::onDemand = 1;
	System::timer();
	unlock();
}
