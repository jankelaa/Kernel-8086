#include "krnlSem.h"
#include "system.h"
#include "SCHEDULE.H"
#include "queuePCB.h"

KernelSem::KernelSem (int init) : value(init), waitBlocked(new QueuePCB()), timerBlocked(new QueuePCB()) {}

KernelSem::~KernelSem () {
	lock();
	delete waitBlocked;
	delete timerBlocked;
	unlock();
}

int KernelSem::wait(Time maxTimeToWait) {
	lock();
	value--;
	if (value<0){
		System::running->state=PCB::BLOCKED;
		System::running->timeToWait=maxTimeToWait;
		if(maxTimeToWait==0){
			waitBlocked->put((PCB*)System::running);
		}
		else{
			timerBlocked->put((PCB*)System::running);
		}
		dispatch();
	}
	if(System::running->timedOut){
		unlock();
		return 0;
	}
	else{
		unlock();
		return 1;
	}
}

int KernelSem::signal(){
	lock();
	value++;
	if(value<=0){
		PCB* unblockedPCB = waitBlocked->get();
		if (unblockedPCB==0){
			unblockedPCB = timerBlocked->get();
			unblockedPCB->timedOut = 0;
		}
		unblockedPCB->state=PCB::READY;
		Scheduler::put(unblockedPCB);
	}
	unlock();
	return 0;
}

void KernelSem::decrementTimeToWait() {
	ElemPCB *temp = timerBlocked->head;
	while (temp) {
		temp->pointPCB->timeToWait--;
		if (temp->pointPCB->timeToWait == 0) {
			value++;

			temp->pointPCB->state = PCB::READY;
			temp->pointPCB->timedOut = 1;
			Scheduler::put(temp->pointPCB);

			if (temp->prev){
				temp->prev->next = temp->next;
				temp->next->prev = temp->prev;			}
			else{
				timerBlocked->head = temp->next;//*temp->next->prev=0;
				temp->next->prev=0;
			}
			ElemPCB *del = temp;
			if(temp==timerBlocked->tail) timerBlocked->tail=temp->prev;
			temp=temp->next;

			delete del;

		} else temp = temp->next;
	}
}


/*
 int KernelSem::signal(int n){
	lock();
	if(n<0){
		unlock();
		return n;
	}
	if(n==0){
		value++;
		if(value<=0){
			PCB* unblockedPCB = waitBlocked->get();
			if (unblockedPCB==0){
				unblockedPCB = timerBlocked->get();
				unblockedPCB->timedOut = 0;
			}
			unblockedPCB->state=PCB::READY;
			Scheduler::put(unblockedPCB);
		}
		unlock();
		return 0;
	}
	if(n>0){
		int unblocked = 0;
		while(n>0){
			n--;
			value++;
			if(value<=0){
				unblocked++;
				PCB* unblockedPCB = waitBlocked->get();
				if (unblockedPCB==0){
					unblockedPCB = timerBlocked->get();
					unblockedPCB->timedOut = 0;
				}
				unblockedPCB->state=PCB::READY;
				Scheduler::put(unblockedPCB);
			}
		}
		unlock();
		return unblocked;
	}
}
*/
