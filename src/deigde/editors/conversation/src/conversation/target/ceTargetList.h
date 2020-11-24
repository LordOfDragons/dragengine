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

#ifndef _CETARGETLIST_H_
#define _CETARGETLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceTarget;



/**
 * \brief Target List.
 */
class ceTargetList{
private:
	decObjectOrderedSet pTargets;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new target list. */
	ceTargetList();
	/** Cleans up the target list. */
	~ceTargetList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of targets. */
	int GetCount() const;
	/** Retrieves the target at the given position. */
	ceTarget *GetAt( int index ) const;
	/** Retrieves the named target or NULL if not found. */
	ceTarget *GetNamed( const char *name ) const;
	/** Retrieves the index of a target or -1 if not found. */
	int IndexOf( ceTarget *target ) const;
	/** Retrieves the index of the named target or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** Determines if a target exists. */
	bool Has( ceTarget *target ) const;
	/** Determines if a named target exists. */
	bool HasNamed( const char *name ) const;
	/** Adds a target. */
	void Add( ceTarget *target );
	/** Removes a target. */
	void Remove( ceTarget *target );
	/** Removes all targets. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	ceTargetList &operator=( const ceTargetList &list );
	
	/** \brief Add targets to list if absent. */
	ceTargetList &operator+=( const ceTargetList &list );
	/*@}*/
};

#endif
