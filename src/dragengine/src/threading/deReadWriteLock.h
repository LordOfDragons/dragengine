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

#ifndef _DEREADWRITELOCK_H_
#define _DEREADWRITELOCK_H_

#include "../dragengine_export.h"
#include "../dragengine_configuration.h"

#if defined OS_UNIX || defined OS_BEOS
#include <pthread.h>
#endif

#ifdef OS_W32
#include "../app/include_windows.h"
#endif


/**
 * \brief Read-Write Lock.
 *
 * Provides mutual shared read access and exclusive write access support using a thin
 * wrapper around the operating system specific threading mutex.
 * 
 * This object ensure that either one or more readers access an object or exactly one
 * writer but no other combination.
 */
class DE_DLL_EXPORT deReadWriteLock{
private:
#if defined OS_UNIX || defined OS_BEOS
	pthread_rwlock_t pRWLock;
#endif
#ifdef OS_W32
	SRWLOCK pRWLock;
#endif
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create read-write lock. */
	deReadWriteLock();
	
	/** \brief Clean up read-write lock. */
	~deReadWriteLock();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Lock for reading.
	 * 
	 * If the lock is already write-locked this call blocks until the write-lock is released.
	 */
	void ReadLock();
	
	/**
	 * \brief Try locking for reading.
	 * 
	 * If the lock is already write-locked this call returns false and no read-lock is
	 * taken. If the lock is not write-locked this call performs the same as ReadLock()
	 * and returns true.
	 */
	bool TryReadLock();
	
	/**
	 * \brief Unlock for reading.
	 * 
	 * An exception is thrown if this thread is not holding a read-lock.
	 */
	void ReadUnlock();
	
	
	
	/**
	 * \brief Lock for writing.
	 * 
	 * If the lock is already read-locked or write-locked this call blocks until all
	 * read-locks and write-lock are released.
	 */
	void WriteLock();
	
	/**
	 * \brief Try locking for writing.
	 * 
	 * If the lock is already read-locked or write-locked this call returns false and
	 * no write-lock is taken. If the lock is not read-locked nor write-locked this call
	 * performs the same as WriteLock() and returns true.
	 */
	bool TryWriteLock();
	
	/**
	 * \brief Unlock for writing.
	 * 
	 * An exception is thrown if this thread is not holding a write-lock.
	 */
	void WriteUnlock();
	/*@}*/
};

#endif
