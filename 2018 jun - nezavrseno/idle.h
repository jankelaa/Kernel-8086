#ifndef _IDLE_H_
#define _IDLE_H_

#include "thread.h"

class Thread;

class Idle : public Thread {
public:
	Idle();
	virtual void run();
};

#endif
