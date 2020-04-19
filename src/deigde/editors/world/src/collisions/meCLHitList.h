/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MECLHITLIST_H_
#define _MECLHITLIST_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decPointerList.h>

class meCLHitListEntry;



/**
 * \brief List of elements hit during a test.
 */
class meCLHitList{
private:
	decPointerList pEntries;
	
	
	
public:
	/** \name Constructors, Destructors */
	/*@{*/
	/** \brief Create hit list. */
	meCLHitList();
	
	/** \brief Clean up hit list. */
	~meCLHitList();
	/*@}*/
	
	
	
	/** \name Entries */
	/*@{*/
	/** \brief Number of entries. */
	int GetEntryCount() const;
	
	/** \brief Entry at index. */
	meCLHitListEntry *GetEntryAt( int index ) const;
	
	/** \brief Index of entry or -1 if absent. */
	int IndexOfEntry( meCLHitListEntry *entry ) const;
	
	/** \brief Entry is present. */
	bool HasEntry( meCLHitListEntry *entry ) const;
	
	/** \brief Add entry. */
	void AddEntry( meCLHitListEntry *entry );
	
	/** \brief Remove entry. */
	void RemoveEntry( meCLHitListEntry *entry );
	
	/** \brief Remove all entries. */
	void RemoveAllEntries();
	
	/** \brief Sort entries by distance. */
	void SortByDistance();
	/*@}*/
};

#endif
