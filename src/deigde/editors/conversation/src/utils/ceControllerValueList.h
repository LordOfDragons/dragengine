/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CECONTROLLERVALUELIST_H_
#define _CECONTROLLERVALUELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceControllerValue;



/**
 * \brief Controller Value List.
 */
class ceControllerValueList{
private:
	decObjectOrderedSet pList;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new controller value list. */
	ceControllerValueList();
	/** Cleans up the controller value list. */
	~ceControllerValueList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of entries. */
	int GetCount() const;
	/** Retrieves the entry at the given position. */
	ceControllerValue *GetAt( int index ) const;
	/** Retrieves the entry with the given controller or -1 if not found. */
	ceControllerValue *GetWith( int controller ) const;
	/** Retrieves the index of the entry or -1 if not found. */
	int IndexOf( ceControllerValue *entry ) const;
	/** Retrieves the index of the entry with the given controller or -1 if not found. */
	int IndexOfWith( int controller ) const;
	/** Determines if an entry exists. */
	bool Has( ceControllerValue *entry ) const;
	/** Determines if an entry with the given controller exists. */
	bool HasWith( int controller ) const;
	/** Adds an entry. */
	void Add( ceControllerValue *entry );
	/** Removes an entry. */
	void Remove( ceControllerValue *entry );
	/** Removes all entries. */
	void RemoveAll();
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** Set this list to the content of another list. */
	ceControllerValueList &operator=( const ceControllerValueList &list );
	/*@}*/
};

#endif
