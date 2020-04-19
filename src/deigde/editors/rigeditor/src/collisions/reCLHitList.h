/* 
 * Drag[en]gine IGDE Rig Editor
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
#ifndef _RECLHITLIST_H_
#define _RECLHITLIST_H_

// includes
#include "dragengine/common/math/decMath.h"

// predefinitions
class reCLHitListEntry;



/**
 * List of elements hit during a test.
 */
class reCLHitList{
private:
	reCLHitListEntry **pEntries;
	int pEntryCount;
	int pEntrySize;
	
public:
	// constructor, destructor
	reCLHitList();
	~reCLHitList();
	
	/** @name Entries */
	/*@{*/
	/** Retrieves the number of entries. */
	inline int GetEntryCount() const{ return pEntryCount; }
	/** Retrieves the entry at the given index. */
	reCLHitListEntry *GetEntryAt( int index ) const;
	/** Retrieves the index of the entry of -1 if not found. */
	int IndexOfEntry( reCLHitListEntry *entry ) const;
	/** Determines if the entry exists. */
	bool HasEntry( reCLHitListEntry *entry ) const;
	/** Adds a entry. */
	void AddEntry( reCLHitListEntry *entry );
	/** Removes a entry. */
	void RemoveEntry( reCLHitListEntry *entry );
	/** Removes all entries. */
	void RemoveAllEntries();
	
	/** Sorts entries by distance. */
	void SortByDistance();
	/*@}*/
};

// end of include only once
#endif
