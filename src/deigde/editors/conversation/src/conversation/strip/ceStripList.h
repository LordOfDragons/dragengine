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

#ifndef _CESTRIPLIST_H_
#define _CESTRIPLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceStrip;



/**
 * \brief Conversation action strip List.
 */
class ceStripList{
private:
	decObjectOrderedSet pList;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new string duration list. */
	ceStripList();
	/** Cleans up the string duration list. */
	~ceStripList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of entries. */
	int GetCount() const;
	/** Retrieves the entry at the given duration. */
	ceStrip *GetAt( int index ) const;
	/** Retrieves the index of the entry or -1 if not found. */
	int IndexOf( ceStrip *entry ) const;
	/** Determines if an entry exists. */
	bool Has( ceStrip *entry ) const;
	/** Adds an entry. */
	void Add( ceStrip *entry );
	/** Inserts an entry. */
	void InsertAt( ceStrip *entry, int index );
	/** Moves an entry to a new position. */
	void MoveTo( ceStrip *entry, int index );
	/** Removes an entry. */
	void Remove( ceStrip *entry );
	/** Removes all entries. */
	void RemoveAll();
	
	/** Adds copies of entries of another list to this list. */
	void AddCopyFrom( const ceStripList &list );
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** Set this list to the content of another list. */
	ceStripList &operator=( const ceStripList &list );
	/*@}*/
};

#endif
