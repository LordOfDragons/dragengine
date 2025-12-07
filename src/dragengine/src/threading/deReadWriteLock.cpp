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
#include <errno.h>
#endif

#include "deReadWriteLock.h"
#include "../common/exceptions.h"



// Class deReadWriteLock
//////////////////////////

// Constructor, destructor
////////////////////////////

deReadWriteLock::deReadWriteLock(){
	#if defined OS_UNIX || defined OS_BEOS
	if(pthread_rwlock_init(&pRWLock, NULL) != 0){
		DETHROW(deeOutOfMemory);
	}
	#endif
	
	#ifdef OS_W32
	InitializeSRWLock(&pRWLock);
	#endif
}

deReadWriteLock::~deReadWriteLock(){
	#if defined OS_UNIX || defined OS_BEOS
	const int result = pthread_rwlock_destroy(&pRWLock);
	if(result != 0){
		switch(result){
		case EBUSY:
			printf("[RWLOCK] %p: Still locked!\n", this);
			break;
			
		case EINVAL:
			printf("[RWLOCK] %p: Invalid lock!\n", this);
			break;
		}
		printf("[RWLOCK] %p: Cleanup failed (%i)\n", this, result);
	}
	#endif
	
	#ifdef OS_W32
	// SRWLock does not require explicit deletion
	#endif
}



// Management
///////////////

void deReadWriteLock::ReadLock(){
	#if defined OS_UNIX || defined OS_BEOS
	if(pthread_rwlock_rdlock(&pRWLock) != 0){
		DETHROW(deeInvalidAction);
	}
	#endif
	
	#ifdef OS_W32
	AcquireSRWLockShared(&pRWLock);
	#endif
}

bool deReadWriteLock::TryReadLock(){
	#if defined OS_UNIX || defined OS_BEOS
	switch(pthread_rwlock_tryrdlock(&pRWLock)){
	case 0:
		return true;
		
	case EBUSY:
		return false;
		
	default:
		DETHROW(deeInvalidAction);
	}
	#endif
	
	#ifdef OS_W32
	return TryAcquireSRWLockShared(&pRWLock) != 0;
	#endif
}

void deReadWriteLock::ReadUnlock(){
	#if defined OS_UNIX || defined OS_BEOS
	if(pthread_rwlock_unlock(&pRWLock) != 0){
		DETHROW(deeInvalidAction);
	}
	#endif
	
	#ifdef OS_W32
	ReleaseSRWLockShared(&pRWLock);
	#endif
}



void deReadWriteLock::WriteLock(){
	#if defined OS_UNIX || defined OS_BEOS
	if(pthread_rwlock_wrlock(&pRWLock) != 0){
		DETHROW(deeInvalidAction);
	}
	#endif
	
	#ifdef OS_W32
	AcquireSRWLockExclusive(&pRWLock);
	#endif
}

bool deReadWriteLock::TryWriteLock(){
	#if defined OS_UNIX || defined OS_BEOS
	switch(pthread_rwlock_trywrlock(&pRWLock)){
	case 0:
		return true;
		
	case EBUSY:
		return false;
		
	default:
		DETHROW(deeInvalidAction);
	}
	#endif
	
	#ifdef OS_W32
	return TryAcquireSRWLockExclusive(&pRWLock) != 0;
	#endif
}

void deReadWriteLock::WriteUnlock(){
	#if defined OS_UNIX || defined OS_BEOS
	if(pthread_rwlock_unlock(&pRWLock) != 0){
		DETHROW(deeInvalidAction);
	}
	#endif
	
	#ifdef OS_W32
	ReleaseSRWLockExclusive(&pRWLock);
	#endif
}
