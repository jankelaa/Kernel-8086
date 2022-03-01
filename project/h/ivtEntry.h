#ifndef _IVTENTRY_H_
#define _IVTENTRY_H_

class KernelEv;

class IVTEntry {
public:
	static IVTEntry* IVTable[256];

	IVTEntry(int, void interrupt (*)(...));
   ~IVTEntry();
	void signal();
	void callOldInterrupt();

	void interrupt (*old)(...);
	KernelEv* myKernelEv;
	int ivtNo;
};

#endif
