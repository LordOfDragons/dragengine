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

#ifndef _CEPCBOPTIONLIST_H_
#define _CEPCBOPTIONLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class cePCBOption;



/**
 * \brief Player Choice Box Option List.
 */
class cePCBOptionList{
private:
	decObjectOrderedSet pOptions;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new option list. */
	cePCBOptionList();
	/** Cleans up the option list. */
	~cePCBOptionList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of options. */
	int GetCount() const;
	/** Retrieves the option at the given position. */
	cePCBOption *GetAt( int index ) const;
	/** Retrieves the index of the given option or -1 if not found. */
	int IndexOf( cePCBOption *option ) const;
	/** Determines if a option exists. */
	bool Has( cePCBOption *option ) const;
	/** Adds a option. */
	void Add( cePCBOption *option );
	/** Inserts a new option. */
	void InsertAt( cePCBOption *option, int index );
	/** Moves a option to a new position. */
	void MoveTo( cePCBOption *option, int index );
	/** Removes a option. */
	void Remove( cePCBOption *option );
	/** Removes all options. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	cePCBOptionList &operator=( const cePCBOptionList &list );
	/*@}*/
};

#endif
