#include "schedule.h"
#include "system.h"
#include "queuePCB.h"
#include "queueSem.h"
#include <DOS.H> //setvect, getvect...

int userMain(int argc, char* argv[]);
void tick();

QueuePCB *System::queuePCB = new QueuePCB();
QueueSem *System::queueSem = new QueueSem();
volatile ID System::systemId = 0;
volatile Time System::counter = 0;
volatile int System::onDemand = 0;
volatile PCB* System::running = 0;
Idle* System::idle = 0;

void System::init() {
	lock();
	void interrupt (*old)(...) = getvect(0x08);
	setvect(0x60, old);
	setvect(0x08, timer);

	Thread* starting = new Thread();
	starting->myPCB->state = PCB::READY;
	running = starting->myPCB;

	idle = new Idle();
	idle->start();
	unlock();
}

void System::restore() {
	lock();
	void interrupt (*old)(...) = getvect(0x60);
	setvect(0x08, old);
	delete idle;
	delete queuePCB;
	delete queueSem;
	unlock();
}


void interrupt System::timer(...) {

	if (!onDemand) {
		tick(); 
		asm int 60h;

		static ElemSem *el;
		for (el = queueSem->head; el; el=el->next) {
			el->pointSem->decrementTimeToWait();
		}

		if (running->pcbTimeSlice == 0) return;
		counter++;
		if (counter < running->pcbTimeSlice) return;
	} 

	onDemand = 0;

	static unsigned tsp, tss, tbp;

	asm {
		mov tsp, sp
		mov tss, ss
		mov tbp, bp
	}

	running->sp = tsp;
	running->ss = tss;
	running->bp = tbp;

	if (running->state == PCB::READY && running != idle->myPCB) Scheduler::put((PCB*)running);
	running = Scheduler::get();
	if (running == 0) running = idle->myPCB;

	tsp = running->sp;
	tss = running->ss;
	tbp = running->bp;

	asm {
		mov sp, tsp
		mov ss, tss
		mov bp, tbp
	}

	counter = 0;
}

int main(int argc, char* argv[]) {
	System::init();
	int value = userMain(argc, argv);
	System::restore();
	return value;
}
