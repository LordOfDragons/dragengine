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

#ifndef _CELOOKATLIST_H_
#define _CELOOKATLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceLookAt;



/**
 * \brief LookAt List.
 */
class ceLookAtList{
private:
	decObjectOrderedSet pLookAts;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new lookat list. */
	ceLookAtList();
	/** Cleans up the lookat list. */
	~ceLookAtList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of lookats. */
	int GetCount() const;
	/** Retrieves the lookat at the given position. */
	ceLookAt *GetAt( int index ) const;
	/** Retrieves the named lookat or NULL if not found. */
	ceLookAt *GetNamed( const char *name ) const;
	/** Retrieves the index of a lookat or -1 if not found. */
	int IndexOf( ceLookAt *lookat ) const;
	/** Retrieves the index of the named lookat or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** Determines if a lookat exists. */
	bool Has( ceLookAt *lookat ) const;
	/** Determines if a named lookat exists. */
	bool HasNamed( const char *name ) const;
	/** Adds a lookat. */
	void Add( ceLookAt *lookat );
	/** Removes a lookat. */
	void Remove( ceLookAt *lookat );
	/** Removes all lookats. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	ceLookAtList &operator=( const ceLookAtList &list );
	/*@}*/
};

#endif
