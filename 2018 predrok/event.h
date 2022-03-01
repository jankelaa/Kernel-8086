#ifndef EVENT_H_
#define EVENT_H_

#include "ivtEntry.h"
#include "thread.h"

#define PREPAREENTRY(ivtno,old) \
void interrupt routine##ivtno(...); \
IVTEntry ent##ivtno(ivtno,routine##ivtno); \
void interrupt routine##ivtno(...) { \
	if (old) ent##ivtno.callOldInterrupt(); \
	ent##ivtno.signal(); \
	dispatch(); \
}
//konstruktor ivtEntry
//nova rutina koja zove signal objekta ivtEntry, koji zove signal pridruzenog kernel eventa, koji budi njegovu nit
//preuzimanje van volje niti jer se makro izvrsava u prekidnoj rutini

class KernelEv;

typedef unsigned char IVTNo;

class Event{
public:
	Event(IVTNo ivtNo);
	~Event();

	void wait();

protected:
	friend class KernelEv;
	void signal();// can call KernelEv

private:
	KernelEv* myImpl;
};
#endif
