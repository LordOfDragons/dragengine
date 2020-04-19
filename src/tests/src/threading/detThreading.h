// include only once
#ifndef _DETTHREADING_H_
#define _DETTHREADING_H_

// includes
#include "../detCase.h"

// predefinitions
class deMutex;
class deSemaphore;
class deThread;



// definitions
#define DETT_THREAD_COUNT	5

// class detThreading
class detThreading : public detCase{
public:
	int testValue;
	bool testCondition;
	deMutex *mutex1;
	deSemaphore *semaphore1;
	deThread *threads[ DETT_THREAD_COUNT ];
	
public:
	detThreading();
	~detThreading();
	void Prepare();
	void Run();
	void CleanUp();
	const char *GetTestName();
	
private:
	void TestThread();
	void Sleep( float seconds );
};

// end of include only once
#endif

