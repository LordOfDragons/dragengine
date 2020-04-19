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

#ifndef _IGDEGDCFORCEFIELDLIST_H_
#define _IGDEGDCFORCEFIELDLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDCForceField;



/**
 * \brief Game Definition Class Force Field List.
 */
class igdeGDCForceFieldList{
private:
	decObjectOrderedSet pFields;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeGDCForceFieldList();
	
	/** \brief Create copy of a game definition class particle field list. */
	igdeGDCForceFieldList( const igdeGDCForceFieldList &list );
	
	/** \brief Clean up list. */
	~igdeGDCForceFieldList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of fields. */
	int GetCount() const;
	
	/** \brief Emitter by index. */
	igdeGDCForceField *GetAt( int index ) const;
	
	/** \brief Index of an field or -1 if not found. */
	int IndexOf( igdeGDCForceField *field ) const;
	
	/** \brief Determines if an field exists. */
	bool Has( igdeGDCForceField *field ) const;
	
	/** \brief Adds an field. */
	void Add( igdeGDCForceField *field );
	
	/** \brief Inserts a new field. */
	void InsertAt( igdeGDCForceField *field, int index );
	
	/** \brief Moves an field to a new position. */
	void MoveTo( igdeGDCForceField *field, int index );
	
	/** \brief Removes an field. */
	void Remove( igdeGDCForceField *field );
	
	/** \brief Removes all fields. */
	void RemoveAll();
	
	/** \brief Set this list to a deep copy of another list. */
	void SetToDeepCopyFrom( const igdeGDCForceFieldList &list );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set list from another list. */
	igdeGDCForceFieldList &operator=( const igdeGDCForceFieldList &list );
	/*@}*/
};

#endif
