/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>

#ifdef OS_UNIX
#include <errno.h>
#endif

#include "deSemaphore.h"
#include "../common/exceptions.h"



// #define DEBUG_SEMAPHORE 1

#ifdef DEBUG_SEMAPHORE
	#define DBGSEMAPHORE(f) printf("Debug-Semaphore %p %s: counter=%i signalCounter=%i\n", this, f, pCounter, pSignalCounter)
#else
	#define DBGSEMAPHORE(f)
#endif



// Class deSemaphore
//////////////////////

// Constructor, destructor
////////////////////////////

deSemaphore::deSemaphore() :
pCounter(0),
pSignalCounter(0)

#ifdef OS_W32
,pEvent(NULL)
#endif
{
	// unix, beos
	#if defined OS_UNIX || defined OS_BEOS
	if(pthread_mutex_init(&pMutex, NULL) != 0){
		DETHROW(deeOutOfMemory);
	}
	if(pthread_cond_init(&pCondition, NULL) != 0){
		DETHROW(deeOutOfMemory);
	}
	#endif
	
	// windows
	#ifdef OS_W32
	pEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(! pEvent){
		DETHROW(deeOutOfMemory);
	}
	InitializeCriticalSection(&pCSWaitCounter);
	#endif
}

deSemaphore::deSemaphore(int count) :
pCounter(count),
pSignalCounter(0)

#ifdef OS_W32
,pEvent(NULL)
#endif
{
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	// unix, beos
	#if defined OS_UNIX || defined OS_BEOS
	if(pthread_mutex_init(&pMutex, NULL) != 0){
		DETHROW(deeOutOfMemory);
	}
	if(pthread_cond_init(&pCondition, NULL) != 0){
		DETHROW(deeOutOfMemory);
	}
	#endif
	
	// windows
	#ifdef OS_W32
	pEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(! pEvent){
		DETHROW(deeOutOfMemory);
	}
	InitializeCriticalSection(&pCSWaitCounter);
	#endif
}

deSemaphore::~deSemaphore(){
	// unix, beos
	#if defined OS_UNIX || defined OS_BEOS
	int result = pthread_cond_destroy(&pCondition);
	if(result != 0){
		if(result == EBUSY){
			printf("Semaphore %p: Condition still in use!\n", this);
			
		}else if(result == EINVAL){
			printf("Semaphore %p: Invalid condition!\n", this);
		}
		printf("Semaphore %p: Condition cleanup failed (%i)\n", this, result);
	}
	
	result = pthread_mutex_destroy(&pMutex);
	if(result != 0){
		if(result == EBUSY){
			printf("Semaphore %p: Mutex still locked!\n", this);
		}
		if(result == EINVAL){
			printf("Semaphore %p: Invalid mutex!\n", this);
		}
		printf("Semaphore %p: Mutex cleanup failed (%i)\n", this, result);
	}
	#endif
	
	// windows
	#ifdef OS_W32
	if(pEvent){
		CloseHandle(pEvent);
	}
	#endif
}



// Management
///////////////

void deSemaphore::Wait(){
	// unix, beos
	#if defined OS_UNIX || defined OS_BEOS
	if(pthread_mutex_lock(&pMutex) != 0){
		DETHROW(deeInvalidAction);
	}
	
	DBGSEMAPHORE("Wait() in");
	pCounter--;
	
	if(pCounter < 0){
		DBGSEMAPHORE("Wait() wait");
		while(pSignalCounter == 0){
			if(pthread_cond_wait(&pCondition, &pMutex) != 0){
				pSignalCounter--;
				pCounter++;
				pthread_mutex_unlock(&pMutex);
				DETHROW(deeInvalidAction);
			}
		}
	}
	
	if(pSignalCounter > 0){
		pSignalCounter--;
	}
	
	DBGSEMAPHORE("Wait() out");
	
	if(pthread_mutex_unlock(&pMutex) != 0){
		DETHROW(deeInvalidAction);
	}
	#endif
	
	// windows
	#ifdef OS_W32
	EnterCriticalSection(&pCSWaitCounter);
	DBGSEMAPHORE("Wait() in");
	pCounter--;
	
	if(pCounter < 0){
		DBGSEMAPHORE("Wait() wait");
		while(pSignalCounter == 0){
			LeaveCriticalSection(&pCSWaitCounter);
			if(WaitForSingleObject(pEvent, INFINITE) != WAIT_OBJECT_0){
				EnterCriticalSection(&pCSWaitCounter);
				pSignalCounter--;
				pCounter++;
				LeaveCriticalSection(&pCSWaitCounter);
				DETHROW(deeInvalidAction);
			}
			EnterCriticalSection(&pCSWaitCounter);
		}
	}
	
	if(pSignalCounter > 0){
		pSignalCounter--;
	}
	
	DBGSEMAPHORE("Wait() out");
	LeaveCriticalSection(&pCSWaitCounter);
	#endif
}

bool deSemaphore::TryWait(){
	// unix, beos
	#if defined OS_UNIX || defined OS_BEOS
	if(pthread_mutex_lock(&pMutex) != 0){
		DETHROW(deeInvalidAction);
	}
	
	DBGSEMAPHORE("TryWait() in");
	if(pCounter < 1){
		DBGSEMAPHORE("TryWait() skip");
		if(pthread_mutex_unlock(&pMutex) != 0){
			DETHROW(deeInvalidAction);
		}
		return false;
	}
	
	if(pSignalCounter > 0){
		pSignalCounter--;
	}
	
	pCounter--;
	DBGSEMAPHORE("TryWait() out");
	
	if(pthread_mutex_unlock(&pMutex) != 0){
		DETHROW(deeInvalidAction);
	}
	return true;
	#endif
	
	// windows
	#ifdef OS_W32
	EnterCriticalSection(&pCSWaitCounter);
	DBGSEMAPHORE("TryWait() in");
	if(pCounter == 0){
		DBGSEMAPHORE("TryWait() skip");
		LeaveCriticalSection(&pCSWaitCounter);
		return false;
	}
	
	pCounter--;
	DBGSEMAPHORE("TryWait() out");
	LeaveCriticalSection(&pCSWaitCounter);
	return true;
	#endif
}

void deSemaphore::Signal(){
	// unix, beos
	#if defined OS_UNIX || defined OS_BEOS
	if(pthread_mutex_lock(&pMutex) != 0){
		DETHROW(deeInvalidAction);
	}
	
	DBGSEMAPHORE("Signal() in");
	pCounter++;
	pSignalCounter++;
	
	if(pthread_cond_signal(&pCondition) != 0){
		pSignalCounter--;
		pCounter--;
		pthread_mutex_unlock(&pMutex);
		DETHROW(deeInvalidAction);
	}
	
	DBGSEMAPHORE("Signal() out");
	
	if(pthread_mutex_unlock(&pMutex) != 0){
		DETHROW(deeInvalidAction);
	}
	#endif
	
	// windows
	#ifdef OS_W32
	EnterCriticalSection(&pCSWaitCounter);
	DBGSEMAPHORE("Signal() in");
	pCounter++;
	pSignalCounter++;
	LeaveCriticalSection(&pCSWaitCounter);
	
	if(! SetEvent(pEvent)){
		EnterCriticalSection(&pCSWaitCounter);
		pSignalCounter--;
		pCounter--;
		LeaveCriticalSection(&pCSWaitCounter);
		DETHROW(deeInvalidAction);
	}
	
	DBGSEMAPHORE("Signal() out");
	#endif
}

void deSemaphore::SignalAll(){
	// unix, beos
	#if defined OS_UNIX || defined OS_BEOS
	if(pthread_mutex_lock(&pMutex) != 0){
		DETHROW(deeInvalidAction);
	}
	
	DBGSEMAPHORE("SignalAll() in");
	if(pCounter < 0){
		DBGSEMAPHORE("SignalAll() broadcast");
		pSignalCounter += -pCounter;
		pCounter = 0;
		
		if(pthread_cond_broadcast(&pCondition) != 0){
			pthread_mutex_unlock(&pMutex);
			DETHROW(deeInvalidAction);
		}
	}
	
	DBGSEMAPHORE("SignalAll() out");
	
	if(pthread_mutex_unlock(&pMutex) != 0){
		DETHROW(deeInvalidAction);
	}
	#endif
	
	#ifdef OS_W32
	// windows releases only one single thread if any are waiting. to simulate a signal all we have
	// to call SetEvent multiple times. every time a thread is released the event is automatically
	// set to not signaled in which case another thread can be potentially released. if there are
	// no more threads waiting the state remains signaled. this is the break condition.
	EnterCriticalSection(&pCSWaitCounter);
	DBGSEMAPHORE("SignalAll() in");
	if(pCounter < 0){
		DBGSEMAPHORE("SignalAll() broadcast");
		pSignalCounter = -pCounter;
		pCounter = 0;
		
		while(pSignalCounter > 0){
			LeaveCriticalSection(&pCSWaitCounter);
			if(! SetEvent(pEvent)){
				DETHROW(deeInvalidAction);
			}
			EnterCriticalSection(&pCSWaitCounter);
		}
	}
	
	DBGSEMAPHORE("SignalAll() out");
	LeaveCriticalSection(&pCSWaitCounter);
	#endif
}
