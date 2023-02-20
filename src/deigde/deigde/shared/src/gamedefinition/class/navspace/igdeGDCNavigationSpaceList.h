/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEGDCNAVIGATIONSPACELIST_H_
#define _IGDEGDCNAVIGATIONSPACELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDCNavigationSpace;



/**
 * \brief Game definition class navigation space list.
 */
class DE_DLL_EXPORT igdeGDCNavigationSpaceList{
private:
	decObjectOrderedSet pNavigationSpaces;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeGDCNavigationSpaceList();
	
	/** \brief Create copy of a game definition class navigation space list. */
	igdeGDCNavigationSpaceList( const igdeGDCNavigationSpaceList &list );
	
	/** \brief Clean up list. */
	~igdeGDCNavigationSpaceList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of navigation spaces. */
	int GetCount() const;
	
	/** \brief Navigation space by index. */
	igdeGDCNavigationSpace *GetAt( int index ) const;
	
	/** \brief Index of an navigation space or -1 if not found. */
	int IndexOf( igdeGDCNavigationSpace *navSpace ) const;
	
	/** \brief Determines if an navigation space exists. */
	bool Has( igdeGDCNavigationSpace *navSpace ) const;
	
	/** \brief Adds an navigation space. */
	void Add( igdeGDCNavigationSpace *navSpace );
	
	/** \brief Inserts a new navigation space. */
	void InsertAt( igdeGDCNavigationSpace *navSpace, int index );
	
	/** \brief Moves an navigation space to a new position. */
	void MoveTo( igdeGDCNavigationSpace *navSpace, int index );
	
	/** \brief Removes an navigation space. */
	void Remove( igdeGDCNavigationSpace *navSpace );
	
	/** \brief Removes all navigation spaces. */
	void RemoveAll();
	
	/** \brief Set this list to a deep copy of another list. */
	void SetToDeepCopyFrom( const igdeGDCNavigationSpaceList &list );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set list from another list. */
	igdeGDCNavigationSpaceList &operator=( const igdeGDCNavigationSpaceList &list );
	/*@}*/
};

#endif
