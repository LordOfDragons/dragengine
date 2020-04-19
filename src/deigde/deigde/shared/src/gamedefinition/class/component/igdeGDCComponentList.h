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

#ifndef _IGDEGDCCOMPONENTLIST_H_
#define _IGDEGDCCOMPONENTLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDCComponent;



/**
 * \brief Game Definition Class Component List.
 */
class igdeGDCComponentList{
private:
	decObjectOrderedSet pComponents;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeGDCComponentList();
	
	/** \brief Create copy of a component list. */
	igdeGDCComponentList( const igdeGDCComponentList &list );
	
	/** \brief Clean up list. */
	~igdeGDCComponentList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of components. */
	int GetCount() const;
	
	/** \brief Component by index. */
	igdeGDCComponent *GetAt( int index ) const;
	
	/** \brief Index of an component or -1 if not found. */
	int IndexOf( igdeGDCComponent *component ) const;
	
	/** \brief Determines if an component exists. */
	bool Has( igdeGDCComponent *component ) const;
	
	/** \brief Adds an component. */
	void Add( igdeGDCComponent *component );
	
	/** \brief Inserts a new component. */
	void InsertAt( igdeGDCComponent *component, int index );
	
	/** \brief Moves an component to a new position. */
	void MoveTo( igdeGDCComponent *component, int index );
	
	/** \brief Removes an component. */
	void Remove( igdeGDCComponent *component );
	
	/** \brief Removes all components. */
	void RemoveAll();
	
	/** \brief Set this list to a deep copy of another list. */
	void SetToDeepCopyFrom( const igdeGDCComponentList &list );
	
	/** \brief Set list from another list. */
	igdeGDCComponentList &operator=( const igdeGDCComponentList &list );
	/*@}*/
};

#endif
