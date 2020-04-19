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

#ifndef _GDEOCFORCEFIELDLIST_H_
#define _GDEOCFORCEFIELDLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeOCForceField;



/**
 * \brief Object class force field list.
 */
class gdeOCForceFieldList{
private:
	decObjectOrderedSet pForceFields;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class force field list. */
	gdeOCForceFieldList();
	
	/** \brief Create copy of object class force field list. */
	gdeOCForceFieldList( const gdeOCForceFieldList &list );
	
	/** \brief Clean up object class force field list. */
	~gdeOCForceFieldList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of object class force fields. */
	int GetCount() const;
	
	/** \brief Object class force field at position. */
	gdeOCForceField *GetAt( int index ) const;
	
	/** \brief Index of named object class force field or -1 if absent. */
	int IndexOf( gdeOCForceField *field ) const;
	
	/** \brief Object class force field is present. */
	bool Has( gdeOCForceField *field ) const;
	
	/** \brief Add object class force field. */
	void Add( gdeOCForceField *field );
	
	/** \brief Remove object class force field. */
	void Remove( gdeOCForceField *field );
	
	/** \brief Remove all object class force fields. */
	void RemoveAll();
	
	/** \brief Set from another object class force field list. */
	gdeOCForceFieldList &operator=( const gdeOCForceFieldList &list );
	/*@}*/
};

#endif
