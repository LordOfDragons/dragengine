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

#ifndef _GDEOCSNAPPOINTLIST_H_
#define _GDEOCSNAPPOINTLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeOCSnapPoint;



/**
 * \brief Object class snap point list.
 */
class gdeOCSnapPointList{
private:
	decObjectOrderedSet pSnapPoints;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class snap point list. */
	gdeOCSnapPointList();
	
	/** \brief Create copy of object class snap point list. */
	gdeOCSnapPointList( const gdeOCSnapPointList &list );
	
	/** \brief Clean up object class snap point list. */
	~gdeOCSnapPointList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of object class snap points. */
	int GetCount() const;
	
	/** \brief Object class snapPoint at position. */
	gdeOCSnapPoint *GetAt( int index ) const;
	
	/** \brief Index of named object class snap point or -1 if absent. */
	int IndexOf( gdeOCSnapPoint *snapPoint ) const;
	
	/** \brief Object class snap point is present. */
	bool Has( gdeOCSnapPoint *snapPoint ) const;
	
	/** \brief Add object class snap point. */
	void Add( gdeOCSnapPoint *snapPoint );
	
	/** \brief Remove object class snap point. */
	void Remove( gdeOCSnapPoint *snapPoint );
	
	/** \brief Remove all object class snap points. */
	void RemoveAll();
	
	/** \brief Set from another object class snap point list. */
	gdeOCSnapPointList &operator=( const gdeOCSnapPointList &list );
	/*@}*/
};

#endif
