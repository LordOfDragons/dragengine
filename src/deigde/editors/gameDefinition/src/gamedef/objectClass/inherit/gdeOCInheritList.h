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

#ifndef _GDEOCINHERITLIST_H_
#define _GDEOCINHERITLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeOCInherit;



/**
 * \brief Object class inherit list.
 */
class gdeOCInheritList{
private:
	decObjectOrderedSet pInheritObjectClasses;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create inherit object class list. */
	gdeOCInheritList();
	
	/** \brief Create copy of inherit object class list. */
	gdeOCInheritList( const gdeOCInheritList &list );
	
	/** \brief Clean up inherit object class list. */
	~gdeOCInheritList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of inherit object classes. */
	int GetCount() const;
	
	/** \brief Object class at position. */
	gdeOCInherit *GetAt( int index ) const;
	
	/** \brief Index of named inherit object class or -1 if absent. */
	int IndexOf( gdeOCInherit *inherit ) const;
	
	/** \brief Object class is present. */
	bool Has( gdeOCInherit *inherit ) const;
	
	/** \brief Add inherit object class. */
	void Add( gdeOCInherit *inherit );
	
	/** \brief Remove inherit object class. */
	void Remove( gdeOCInherit *inherit );
	
	/** \brief Remove all inherit object classes. */
	void RemoveAll();
	
	/** \brief Set from another inherit object class list. */
	gdeOCInheritList &operator=( const gdeOCInheritList &list );
	/*@}*/
};

#endif
