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

#ifndef _DETHREADSAFEOBJECT_H_
#define _DETHREADSAFEOBJECT_H_

#include "deMutex.h"


/**
 * \brief Thread safe version of deObject.
 *
 * In contrary to deObject a deMutex is used to protect reference manipulations against multi
 * threaded use. This does not imply all methods of the object are thread safe. Subclasses
 * are encouraged to use GetMutex() to provide thread safe access to methods.
 */
class deThreadSafeObject{
private:
	int pRefCount;
	deMutex pMutex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object with reference count of 1. */
	deThreadSafeObject();
	
protected:
	/**
	 * \brief Clean up object.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deThreadSafeObject();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Reference count. */
	int GetRefCount();
	
	/** \brief Add reference increasing reference count by 1. */
	void AddReference();
	
	/** \brief Decrease reference count by one and delete object if count reaches 0. */
	void FreeReference();
	/*@}*/
};

#endif
