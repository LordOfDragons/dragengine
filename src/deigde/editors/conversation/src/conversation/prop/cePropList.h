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

#ifndef _CEPROPLIST_H_
#define _CEPROPLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceProp;



/**
 * \brief Conversation prop list.
 */
class cePropList{
private:
	decObjectOrderedSet pProps;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new prop list. */
	cePropList();
	/** \brief Cleans up the prop list. */
	~cePropList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the number of props. */
	int GetCount() const;
	/** \brief Retrieves the prop at the given position. */
	ceProp *GetAt( int index ) const;
	/** \brief Retrieves the index of the given prop or -1 if not found. */
	int IndexOf( ceProp *prop ) const;
	/** \brief Determines if a prop exists. */
	bool Has( ceProp *prop ) const;
	/** \brief Adds a prop. */
	void Add( ceProp *prop );
	/** \brief Removes a prop. */
	void Remove( ceProp *prop );
	/** \brief Removes all props. */
	void RemoveAll();
	
	/** \brief Sets the list from another list. */
	cePropList &operator=( const cePropList &list );
	/*@}*/
};

#endif
