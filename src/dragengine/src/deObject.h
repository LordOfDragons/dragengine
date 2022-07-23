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

#ifndef _DEOBJECT_H_
#define _DEOBJECT_H_

#include "deTObjectReference.h"


/**
 * \brief Basic object interface.
 *
 * Provides the basic class for all engine objects. Each object has a
 * reference count. Objects stay alive as long as the reference count
 * is larger than 0. If you want to add a reference use the AddReference
 * function. If you want to free the reference you have added before use
 * the FreeReference function and do not call 'delete'. Upon creation
 * each object is assigned a reference count of 1 so you do not have to
 * add a refernece after creating an object.
 */
class DE_DLL_EXPORT deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deObject> Ref;
	
	
	
private:
	int pRefCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object with reference count of 1. */
	deObject();
	
protected:
	/**
	 * \brief Clean up object.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deObject();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Reference count. */
	inline int GetRefCount() const{ return pRefCount; }
	
	/** \brief Add reference increasing reference count by 1. */
	void AddReference();
	
	/** \brief Decrease reference count by one and delete object if count reaches 0. */
	void FreeReference();
	/*@}*/
};

#endif
