#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "idle.h"
#include "pcb.h"
#include "queuePCB.h"
#include "queueSem.h"

class Idle;
class PCB;
class QueuePCB;
class QueueSem;

#define lock() {asm{pushf; cli;}}
#define unlock() {asm{popf}}

class System {
public:
	static void init();
	static void restore();

	static Idle* idle;
	static volatile PCB* running;
	static volatile int onDemand;
	static volatile Time counter;
	static volatile ID systemId;
	static QueuePCB *queuePCB; //dohvatanje pcb po id
	static QueueSem *queueSem; //dekrementiranje timera na svim semaforima

	static void interrupt timer(...);
};

#endif
