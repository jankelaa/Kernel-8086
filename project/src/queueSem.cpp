#include "semaphor.h"
#include "queueSem.h"
#include "system.h"

QueueSem::QueueSem(){
	lock();
	head=0;
	tail=0;
	unlock();
}

QueueSem::~QueueSem() {
	lock();
	ElemSem *del;
	while(head){
		del=head;
		head=head->next;
		delete del;
	}
	unlock();
}

void QueueSem::put(KernelSem *ksem){
	lock();
	ElemSem *el = new ElemSem();
	el->pointSem=ksem;
	el->prev=tail;
	el->next=0;
	if(!head) head=el;
	else tail->next=el;
	tail=el;
	unlock();
}

KernelSem* QueueSem::get(){
	lock();
	if(head){
		KernelSem *ret = head->pointSem;
		ElemSem *del = head;
		head=head->next;
		if(head)head->prev=0;
		else tail =0;
		delete del;
		unlock();
		return ret;
	}
	unlock();
	return 0;
}
