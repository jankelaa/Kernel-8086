#include "semaphor.h"
#include "queueSem.h"
#include "system.h"
#include "krnlSem.h"

Semaphore::Semaphore (int init){
	lock();
	myImpl = new KernelSem(init);
	System::queueSem->put(myImpl);
	unlock();
}
Semaphore::~Semaphore (){
	lock();
	delete myImpl;
	unlock();
}

void Semaphore::waitFriend(){
	lock();
	myImpl->waitFriend(0);
	unlock();
}

void Semaphore::signalFriend(){
	lock();
	myImpl->signalFriend();
	unlock();
}

int Semaphore::wait (Time maxTimeToWait){
	int ret;
	lock();
	ret = myImpl->wait(maxTimeToWait);
	unlock();
	return ret;
}
int Semaphore::signal(){
	int ret;
	lock();
	ret = myImpl->signal();
	unlock();
	return ret;
}
int Semaphore::val () const {
	int ret;
	lock();
	ret = myImpl->value;
	unlock();
	return ret;
}
