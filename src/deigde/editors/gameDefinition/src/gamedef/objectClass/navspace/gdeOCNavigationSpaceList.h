/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEOCNAVIGATIONSPACELIST_H_
#define _GDEOCNAVIGATIONSPACELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeOCNavigationSpace;



/**
 * \brief Object class navigation space list.
 */
class gdeOCNavigationSpaceList{
private:
	decObjectOrderedSet pNavigationSpaces;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class navigation space list. */
	gdeOCNavigationSpaceList();
	
	/** \brief Create copy of object class navigation space list. */
	gdeOCNavigationSpaceList( const gdeOCNavigationSpaceList &list );
	
	/** \brief Clean up object class navigation space list. */
	~gdeOCNavigationSpaceList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of object class navigation spaces. */
	int GetCount() const;
	
	/** \brief Object class navigation space at position. */
	gdeOCNavigationSpace *GetAt( int index ) const;
	
	/** \brief Index of named object class navigation space or -1 if absent. */
	int IndexOf( gdeOCNavigationSpace *space ) const;
	
	/** \brief Object class navigation space is present. */
	bool Has( gdeOCNavigationSpace *space ) const;
	
	/** \brief Add object class navigation space. */
	void Add( gdeOCNavigationSpace *space );
	
	/** \brief Remove object class navigation space. */
	void Remove( gdeOCNavigationSpace *space );
	
	/** \brief Remove all object class navigation spaces. */
	void RemoveAll();
	
	/** \brief Set from another object class navigation space list. */
	gdeOCNavigationSpaceList &operator=( const gdeOCNavigationSpaceList &list );
	/*@}*/
};

#endif
