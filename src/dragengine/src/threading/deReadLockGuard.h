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
	deReadLockGuard(deReadWriteLock &rwlock);
	
	/** \brief Clean read-write lock guard unlocking read-write lock. */
	~deReadLockGuard();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Guard holds the read-lock. */
	inline bool GetLocked() const{return pLocked;}
	
	/** \brief Read-lock guarded read-write lock. */
	void ReadLock();
	
	/** \brief Try read-locking guarded read-write lock. */
	bool TryReadLock();
	
	/** \brief Read-unlock guarded read-write lock. */
	void ReadUnlock();
	/*@}*/
};

#endif
