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

#ifndef _DEOBJECTDEBUG_H_
#define _DEOBJECTDEBUG_H_

#include "dragengine_export.h"


/**
 * \brief Debug version of deObject.
 *
 * Prints AddReference and FreeReference to console with first backtrace
 * entry to locate add/free calls.
 * 
 * \warning This is a pure debug class. Do not use unless you know what you are doing!
 */
class DE_DLL_EXPORT deObjectDebug{
private:
	int pRefCount;
	const char *pLogName;
	int pLogTraceIndex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object with reference count of 1. */
	deObjectDebug( const char *logname, int logTraceIndex );
	
protected:
	/**
	 * \brief Clean up object.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deObjectDebug();
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
