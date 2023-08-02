/* 
 * Drag[en]gine IGDE Skin Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _SEMAPPEDLIST_H_
#define _SEMAPPEDLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class seMapped;



/**
 * Mapped list.
 */
class seMappedList{
private:
	decObjectOrderedSet pMapped;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create mapped list. */
	seMappedList();
	
	/** Cleans up the mapped list. */
	~seMappedList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Count of mapped. */
	int GetCount() const;
	
	/** Mapped at index. */
	seMapped *GetAt( int index ) const;
	
	/** Named mapped or nullptr if absent. */
	seMapped *GetNamed( const char *name ) const;
	
	/** Index of mapped or -1 if absent. */
	int IndexOf( seMapped *mapped ) const;
	
	/** Index of named mapped or -1 if absent. */
	int IndexOfNamed( const char *name ) const;
	
	/** Mapped is present. */
	bool Has( seMapped *mapped ) const;
	
	/** Named mapped is present. */
	bool HasNamed( const char *name ) const;
	
	/** Add mapped. */
	void Add( seMapped *mapped );
	
	/** Remove mapped. */
	void Remove( seMapped *mapped );
	
	/** Remove all mapped. */
	void RemoveAll();
	
	/** Set list from another list. */
	seMappedList &operator=( const seMappedList &list );
	/*@}*/
};

#endif
