#include "pcb.h"
#include "queuePCB.h"
#include "system.h"

QueuePCB::QueuePCB() {
	lock();
	head=0;
	tail=0;
	unlock();
}

QueuePCB::~QueuePCB() {
	lock();
	ElemPCB *del;
	while(head){
		del=head;
		head=head->next;
		delete del;
	}
	unlock();
}

void QueuePCB::put(PCB *pcb){
	lock();
	ElemPCB *el = new ElemPCB();
	el->pointPCB=pcb;
	el->prev=tail;
	el->next=0;
	if(!head) head=el;
	else tail->next=el;
	tail=el;
	unlock();
}

PCB* QueuePCB::get(){
	lock();
	if(head){
		PCB *ret = head->pointPCB;
		ElemPCB *del = head;
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
