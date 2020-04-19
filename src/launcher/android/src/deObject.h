/* 
 * Drag[en]gine Android Launcher
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

// include only once
#ifndef _DEOBJECT_H_
#define _DEOBJECT_H_


/**
 * @brief Basic Object Interface.
 *
 * Provides the basic class for all engine objects. Each object has a
 * reference count. Objects stay alive as long as the reference count
 * is larger than 0. If you want to add a reference use the AddReference
 * function. If you want to free the reference you have added before use
 * the FreeReference function and do not call 'delete'. Upon creation
 * each object is assigned a reference count of 1 so you do not have to
 * add a refernece after creating an object.
 */
class deObject{
private:
	int pRefCount;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new object with a reference count of 1. */
	deObject();
	/** Cleans up the resource. */
	virtual ~deObject();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the reference count. */
	inline int GetRefCount() const{ return pRefCount; }
	/** Adds a reference and increases the reference count by 1. */
	void AddReference();
	/** Decreases the reference count by one and deletes the object if the count reaches 0. */
	void FreeReference();
	/*@}*/
};

// end of include only once
#endif
