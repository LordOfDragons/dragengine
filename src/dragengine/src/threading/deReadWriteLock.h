/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
