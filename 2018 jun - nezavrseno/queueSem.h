#ifndef _QUEUESEM_h_
#define _QUEUESEM_h_

#include "krnlSem.h"

class KernelSem;

class ElemSem {
public:
	KernelSem* pointSem;
	ElemSem* next, *prev;
};

class QueueSem {
public:
	QueueSem();
	~QueueSem();
	void put(KernelSem* ksem);
	KernelSem* get();

	ElemSem *head, *tail;
};

#endif
