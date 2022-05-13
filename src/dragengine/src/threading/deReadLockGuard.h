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

#ifndef _DEREADLOCKGUARD_H_
#define _DEREADLOCKGUARD_H_

#include "../dragengine_export.h"

class deReadWriteLock;


/**
 * \brief Read-write lock guard read locking in constructor and unlocking in destructor.
 *
 * Exception safe automatic read locking and unlocking.
 */
class DE_DLL_EXPORT deReadLockGuard{
private:
	deReadWriteLock &pRWLock;
	bool pLocked;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create read-write lock guard and lock read-write lock. */
	deReadLockGuard( deReadWriteLock &rwlock );
	
	/** \brief Clean read-write lock guard unlocking read-write lock. */
	~deReadLockGuard();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Guard holds the read-lock. */
	inline bool GetLocked() const{ return pLocked; }
	
	/** \brief Read-lock guarded read-write lock. */
	void ReadLock();
	
	/** \brief Try read-locking guarded read-write lock. */
	bool TryReadLock();
	
	/** \brief Read-unlock guarded read-write lock. */
	void ReadUnlock();
	/*@}*/
};

#endif
