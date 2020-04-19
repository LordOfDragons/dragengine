/* 
 * Drag[en]gine IGDE Language Pack Editor
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

#ifndef _LPELANGPACKENTRYLIST_H_
#define _LPELANGPACKENTRYLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class lpeLangPackEntry;



/**
 * \brief Language pack entry List.
 */
class lpeLangPackEntryList{
private:
	decObjectOrderedSet pEntries;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	lpeLangPackEntryList();
	
	/** \brief Create copy of list. */
	lpeLangPackEntryList( const lpeLangPackEntryList &list );
	
	/** \brief Clean up entry list. */
	~lpeLangPackEntryList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of entries. */
	int GetCount() const;
	
	/** \brief Entry at index. */
	lpeLangPackEntry *GetAt( int index ) const;
	
	/** \brief Named entry or NULL if absent. */
	lpeLangPackEntry *GetNamed( const char *name ) const;
	
	/** \brief Index of entry or -1 if absent. */
	int IndexOf( lpeLangPackEntry *entry ) const;
	
	/** \brief Index of named entry or -1 if absent. */
	int IndexOfNamed( const char *name ) const;
	
	/** \brief Entry is present. */
	bool Has( lpeLangPackEntry *entry ) const;
	
	/** \brief Named entry is present. */
	bool HasNamed( const char *name ) const;
	
	/** \brief Add entry. */
	void Add( lpeLangPackEntry *entry );
	
	/** \brief Insert entry. */
	void InsertAt( lpeLangPackEntry *entry, int index );
	
	/** \brief Move entry. */
	void MoveTo( lpeLangPackEntry *entry, int index );
	
	/** \brief Remove entry. */
	void Remove( lpeLangPackEntry *entry );
	
	/** \brief Remove all entries. */
	void RemoveAll();
	
	
	
	/** \brief Set from another list. */
	lpeLangPackEntryList &operator=( const lpeLangPackEntryList &list );
	/*@}*/
};

#endif
