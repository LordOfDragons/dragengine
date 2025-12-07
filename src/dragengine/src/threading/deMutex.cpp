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

#if defined OS_UNIX || defined OS_BEOS
#	include <errno.h>
#endif

#include "deMutex.h"
#include "../common/exceptions.h"



// Class deMutex
//////////////////

// Constructor, destructor
////////////////////////////

deMutex::deMutex()
#ifdef OS_W32
: pMutex(NULL)
#endif
{
#if defined OS_UNIX || defined OS_BEOS
	/*
	const int result = pthread_mutex_init(&pMutex, NULL);
	if(result != 0){
		if(result == ENOMEM) DETHROW(deeOutOfMemory);
		if(result == EAGAIN) DETHROW_INFO(deeOutOfMemory, "lack of resources");
		if(result == EPERM) DETHROW_INFO(deeInvalidAction, "permission denied");
		if(result == EBUSY) DETHROW_INFO(deeInvalidAction, "double initialization");
		DETHROW_INFO(deeInvalidAction, "unknown error");
	}
	*/
	if(pthread_mutex_init(&pMutex, NULL) != 0){
		DETHROW(deeOutOfMemory);
	}
#endif

#ifdef OS_W32
	pMutex = CreateMutex(NULL, FALSE, NULL);
	if(pMutex == NULL){
		DETHROW(deeOutOfMemory);
	}
#endif
}

deMutex::~deMutex(){
#if defined OS_UNIX || defined OS_BEOS
	const int result = pthread_mutex_destroy(&pMutex);
	if(result != 0){
		if(result == EBUSY) printf("[MUTEX] %p: Still locked!\n", this);
		if(result == EINVAL) printf("[MUTEX] %p: Invalid mutex!\n", this);
		printf("[MUTEX] %p: Cleanup failed (%i)\n", this, result);
	}
#endif

#ifdef OS_W32
	if(pMutex != NULL){
		CloseHandle(pMutex);
	}
#endif
}



// Management
///////////////

void deMutex::Lock(){
#if defined OS_UNIX || defined OS_BEOS
	if(pthread_mutex_lock(&pMutex) != 0){
		DETHROW(deeInvalidAction);
	}
#endif
	
#ifdef OS_W32
	if(WaitForSingleObject(pMutex, INFINITE) != WAIT_OBJECT_0){
		DETHROW(deeInvalidAction);
	}
#endif
}

bool deMutex::TryLock(){
#if defined OS_UNIX || defined OS_BEOS
	switch(pthread_mutex_trylock(&pMutex)){
	case 0:
		return true;
		
	case EBUSY:
		return false;
		
	default:
		DETHROW(deeInvalidAction);
	}
#endif
	
#ifdef OS_W32
	switch(WaitForSingleObject(pMutex, 0)){
	case WAIT_OBJECT_0:
		return true;
		
	case WAIT_TIMEOUT:
		return false;
		
	default:
		DETHROW(deeInvalidAction);
	}
#endif
}

bool deMutex::TryLock(int timeout){
#if defined OS_UNIX || defined OS_BEOS
	timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += timeout / 1000;
	ts.tv_nsec += (long)(timeout % 1000) * 1000000L;
	if(ts.tv_nsec >= 1000000000L){
		ts.tv_sec++;
		ts.tv_nsec %= 1000000000L;
	}
	
	switch(pthread_mutex_timedlock(&pMutex, &ts)){
	case 0:
		return true;
		
	case ETIMEDOUT:
		return false;
		
	default:
		DETHROW(deeInvalidAction);
	}
#endif
	
#ifdef OS_W32
	switch(WaitForSingleObject(pMutex, timeout)){
	case WAIT_OBJECT_0:
		return true;
		
	case WAIT_TIMEOUT:
		return false;
		
	default:
		DETHROW(deeInvalidAction);
	}
#endif
}

void deMutex::Unlock(){
#if defined OS_UNIX || defined OS_BEOS
	if(pthread_mutex_unlock(&pMutex) != 0){
		DETHROW(deeInvalidAction);
	}
#endif
	
#ifdef OS_W32
	if(! ReleaseMutex(pMutex)){
		DETHROW(deeInvalidAction);
	}
#endif
}
