#ifndef _QUEUEPCB_H_
#define _QUEUEPCB_H_

#include "pcb.h"

class PCB;

class ElemPCB {
public:
	PCB* pointPCB;
	ElemPCB* next, *prev;
};

class QueuePCB {
public:
	QueuePCB();
	~QueuePCB();
	void put(PCB* pcb);
	PCB* get();

	ElemPCB* head, *tail;
};


#endif
