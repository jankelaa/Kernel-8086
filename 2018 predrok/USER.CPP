#include <iostream.h>
#include <stdlib.h>
#include <assert.h>

#include "bounded.h"
#include "intLock.h"
#include "keyevent.h"
#include "user.h"
#include "Event.h"
#include "semaphor.h"

extern int syncPrintf(const char *format, ...);

Time TIME_SLICE = 2;   // 0 ili defaultTimeSlice

int N = 3;            // 1 <= N <= 19


volatile int theEnd=0;

void tick(){
}

Semaphore sleep(0);

class Test: public Thread
{
public:
	Test():Thread(4096,1){}
protected:
	void run(){
		syncPrintf("IZVRSAVANJE: %d\n",(int)this->getId());
		sleep.wait(rand()%200+50);
//		notify_friends();
	}
};

int userMain (int argc, char* argv[])
{
	intLock
	cout<<"userMain start!"<<endl;
	intUnlock
	int l =10;
	int i =0;
	Test* t[10];

	for(i=0;i<l;i++)
	{
		t[i] = new Test();
		syncPrintf("NAPRAVLJENA NIT: %d\n", (int)t[i]->getId());
	}

	Thread::makeFriends(t[0],t[1]);
	Thread::makeFriends(t[0],t[2]);
	Thread::makeFriends(t[0],t[3]);
	Thread::makeFriends(t[0],t[4]);


	Thread::makeFriends(t[1],t[2]);
	Thread::makeFriends(t[1],t[3]);
	Thread::makeFriends(t[1],t[4]);

	Thread::makeFriends(t[2],t[3]);
	Thread::makeFriends(t[2],t[4]);

	Thread::makeFriends(t[3],t[4]);

	for(i=0;i<l;i++)
	{
			t[i]->start();
	}

	for(i=0;i<l;i++)
	{
		delete t[i];
	}

	intLock
	cout<<"userMain finished!"<<endl;
	intUnlock
	return 0;
}
