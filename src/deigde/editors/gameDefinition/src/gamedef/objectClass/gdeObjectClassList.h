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

#ifndef _GDEOBJECTCLASSLIST_H_
#define _GDEOBJECTCLASSLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeObjectClass;



/**
 * \brief Object class list.
 */
class gdeObjectClassList{
private:
	decObjectOrderedSet pObjectClasses;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class list. */
	gdeObjectClassList();
	
	/** \brief Create copy of object class list. */
	gdeObjectClassList( const gdeObjectClassList &list );
	
	/** \brief Clean up object class list. */
	~gdeObjectClassList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of object classes. */
	int GetCount() const;
	
	/** \brief Object class at position. */
	gdeObjectClass *GetAt( int index ) const;
	
	/** \brief Named object class or \em NULL if absent. */
	gdeObjectClass *GetNamed( const char *name ) const;
	
	/** \brief Index of named object class or -1 if absent. */
	int IndexOf( gdeObjectClass *objectClass ) const;
	
	/** \brief Index of named object class or -1 if absent. */
	int IndexOfNamed( const char *name ) const;
	
	/** \brief Object class is present. */
	bool Has( gdeObjectClass *objectClass ) const;
	
	/** \brief Named object class is present. */
	bool HasNamed( const char *name ) const;
	
	/** \brief Add object class. */
	void Add( gdeObjectClass *objectClass );
	
	/** \brief Remove object class. */
	void Remove( gdeObjectClass *objectClass );
	
	/** \brief Remove all object classes. */
	void RemoveAll();
	
	/** \brief Set from another object class list. */
	gdeObjectClassList &operator=( const gdeObjectClassList &list );
	/*@}*/
};

#endif
