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

#ifndef _GDEOCCOMPONENTLIST_H_
#define _GDEOCCOMPONENTLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeOCComponent;



/**
 * \brief Object class component list.
 */
class gdeOCComponentList{
private:
	decObjectOrderedSet pComponents;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class component list. */
	gdeOCComponentList();
	
	/** \brief Create copy of object class component list. */
	gdeOCComponentList( const gdeOCComponentList &list );
	
	/** \brief Clean up object class component list. */
	~gdeOCComponentList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of object class components. */
	int GetCount() const;
	
	/** \brief Object class at position. */
	gdeOCComponent *GetAt( int index ) const;
	
	/** \brief Index of named object class component or -1 if absent. */
	int IndexOf( gdeOCComponent *component ) const;
	
	/** \brief Object class is present. */
	bool Has( gdeOCComponent *component ) const;
	
	/** \brief Add object class component. */
	void Add( gdeOCComponent *component );
	
	/** \brief Remove object class component. */
	void Remove( gdeOCComponent *component );
	
	/** \brief Remove all object class components. */
	void RemoveAll();
	
	/** \brief Set from another object class component list. */
	gdeOCComponentList &operator=( const gdeOCComponentList &list );
	/*@}*/
};

#endif
