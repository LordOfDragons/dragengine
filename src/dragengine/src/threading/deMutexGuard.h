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

#ifndef _DEMUTEXGUARD_H_
#define _DEMUTEXGUARD_H_

class deMutex;


/**
 * \brief Mutex guard locking in constructor and unlocking in destructor.
 *
 * Exception safe automatic locking and unlocking.
 */
class deMutexGuard{
private:
	deMutex &pMutex;
	bool pLocked;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create mutex guard and lock mutex. */
	deMutexGuard( deMutex &mutex );
	
	/** \brief Clean mutex guard unlocking mutex. */
	~deMutexGuard();
	/*@}*/
	
	
	
private:
	// Protection against coding mistakes
	deMutexGuard( const deMutexGuard &guard );
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Guard holds the lock. */
	inline bool GetLocked() const{ return pLocked; }
	
	/** \brief Lock guarded mutex. */
	void Lock();
	
	/** \brief Try locking guarded mutex. */
	bool TryLock();
	
	/** \brief Unlock guarded mutex. */
	void Unlock();
	/*@}*/
};

#endif
