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

#ifndef _IGDEGDCLIGHTLIST_H_
#define _IGDEGDCLIGHTLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDCLight;



/**
 * \brief Game Definition Class Light List.
 */
class DE_DLL_EXPORT igdeGDCLightList{
private:
	decObjectOrderedSet pLights;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeGDCLightList();
	
	/** \brief Create copy of a game definition class light list. */
	igdeGDCLightList( const igdeGDCLightList &list );
	
	/** \brief Clean up list. */
	~igdeGDCLightList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of lights. */
	int GetCount() const;
	
	/** \brief Light by index. */
	igdeGDCLight *GetAt( int index ) const;
	
	/** \brief Index of an light or -1 if not found. */
	int IndexOf( igdeGDCLight *light ) const;
	
	/** \brief Determines if an light exists. */
	bool Has( igdeGDCLight *light ) const;
	
	/** \brief Adds an light. */
	void Add( igdeGDCLight *light );
	
	/** \brief Inserts a new light. */
	void InsertAt( igdeGDCLight *light, int index );
	
	/** \brief Moves an light to a new position. */
	void MoveTo( igdeGDCLight *light, int index );
	
	/** \brief Removes an light. */
	void Remove( igdeGDCLight *light );
	
	/** \brief Removes all lights. */
	void RemoveAll();
	
	/** \brief Set this list to a deep copy of another list. */
	void SetToDeepCopyFrom( const igdeGDCLightList &list );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set list from another list. */
	igdeGDCLightList &operator=( const igdeGDCLightList &list );
	/*@}*/
};

#endif
