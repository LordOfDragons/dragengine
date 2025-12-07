// includes
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "detThreading.h"
#include "dragengine/threading/deMutex.h"
#include "dragengine/threading/deSemaphore.h"
#include "dragengine/threading/deThread.h"
#include "dragengine/common/utils/decTimer.h"
#include "dragengine/common/exceptions.h"



// Threads
////////////

class cThreadSimple : public deThread{
private:
	detThreading *pThreading;
	
public:
	cThreadSimple(detThreading *threading){
		pThreading = threading;
	}
	virtual ~cThreadSimple(){}
	virtual void Run(){
		pThreading->mutex1->Lock();
		pThreading->testValue = 1;
		pThreading->mutex1->Unlock();
		
		pThreading->semaphore1->Wait();
		
		pThreading->mutex1->Lock();
		pThreading->testValue = 2;
		pThreading->mutex1->Unlock();
	}
};



// Class detThreading
///////////////////////

// Constructors, destructor
/////////////////////////////

detThreading::detThreading(){
	Prepare();
}

detThreading::~detThreading(){
	CleanUp();
}



// Testing
////////////

void detThreading::Prepare(){
	int i;
	
	testValue = 0;
	testCondition = false;
	mutex1 = NULL;
	semaphore1 = NULL;
	
	for(i=0; i<DETT_THREAD_COUNT; i++) threads[i] = NULL;
}

void detThreading::Run(){
	TestThread();
}

void detThreading::CleanUp(){
	int i;
	
	for(i=0; i<DETT_THREAD_COUNT; i++){
		if(threads[i]){
			threads[i]->Stop();
			delete threads[i];
		}
	}
	
	if(semaphore1) delete semaphore1;
	if(mutex1) delete mutex1;
}

const char *detThreading::GetTestName(){return "Threading";}



// Tests
//////////

void detThreading::TestThread(){
	SetSubTestNum(0);
	
	mutex1 = new deMutex;
	if(! mutex1) DETHROW(deeOutOfMemory);
	
	semaphore1 = new deSemaphore;
	if(! semaphore1) DETHROW(deeOutOfMemory);
	
	testValue = 0;
	
	threads[0] = new cThreadSimple(this);
	if(! threads[0]) DETHROW(deeOutOfMemory);
	ASSERT_FALSE(threads[0]->IsRunning());
	
	Sleep(0.2);
	ASSERT_FALSE(threads[0]->IsRunning());
	
	mutex1->Lock();
	try{
		ASSERT_TRUE(testValue == 0);
		
	}catch(const deException &){
		mutex1->Unlock();
		throw;
	}
	mutex1->Unlock();
	ASSERT_FALSE(threads[0]->IsRunning());
	
	mutex1->Lock();
	try{
		threads[0]->Start();
		ASSERT_TRUE(threads[0]->IsRunning());
		
		Sleep(0.2);
		
		ASSERT_TRUE(testValue == 0);
		
	}catch(const deException &){
		mutex1->Unlock();
		throw;
	}
	mutex1->Unlock();
	
	Sleep(0.2);
	ASSERT_TRUE(threads[0]->IsRunning());
	
	mutex1->Lock();
	try{
		ASSERT_TRUE(testValue == 1);
		
	}catch(const deException &){
		mutex1->Unlock();
		throw;
	}
	mutex1->Unlock();
	ASSERT_TRUE(threads[0]->IsRunning());
	
	semaphore1->SignalAll();
	
	Sleep(0.2);
	ASSERT_FALSE(threads[0]->IsRunning());
	
	mutex1->Lock();
	try{
		ASSERT_TRUE(testValue == 2);
		
	}catch(const deException &){
		mutex1->Unlock();
		throw;
	}
	mutex1->Unlock();
	
	delete threads[0];
	threads[0] = NULL;
	
	delete semaphore1;
	semaphore1 = NULL;
	
	delete mutex1;
	mutex1 = NULL;
}

void detThreading::Sleep(float seconds){
	decTimer timer;
	
	while(seconds > 0.0f){
		seconds -= timer.GetElapsedTime();
	}
}
