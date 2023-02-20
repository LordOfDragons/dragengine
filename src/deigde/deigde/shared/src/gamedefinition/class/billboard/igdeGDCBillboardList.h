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

#ifndef _IGDEGDCBILLBOARDLIST_H_
#define _IGDEGDCBILLBOARDLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDCBillboard;



/**
 * \brief Game Definition Class Billboard List.
 */
class DE_DLL_EXPORT igdeGDCBillboardList{
private:
	decObjectOrderedSet pBillboards;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeGDCBillboardList();
	
	/** \brief Create copy of a game definition class billboard list. */
	igdeGDCBillboardList( const igdeGDCBillboardList &list );
	
	/** \brief Clean up list. */
	~igdeGDCBillboardList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of billboards. */
	int GetCount() const;
	
	/** \brief Billboard by index. */
	igdeGDCBillboard *GetAt( int index ) const;
	
	/** \brief Index of an billboard or -1 if not found. */
	int IndexOf( igdeGDCBillboard *billboard ) const;
	
	/** \brief Determines if an billboard exists. */
	bool Has( igdeGDCBillboard *billboard ) const;
	
	/** \brief Adds an billboard. */
	void Add( igdeGDCBillboard *billboard );
	
	/** \brief Inserts a new billboard. */
	void InsertAt( igdeGDCBillboard *billboard, int index );
	
	/** \brief Moves an billboard to a new position. */
	void MoveTo( igdeGDCBillboard *billboard, int index );
	
	/** \brief Removes an billboard. */
	void Remove( igdeGDCBillboard *billboard );
	
	/** \brief Removes all billboards. */
	void RemoveAll();
	
	/** \brief Set this list to a deep copy of another list. */
	void SetToDeepCopyFrom( const igdeGDCBillboardList &list );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set list from another list. */
	igdeGDCBillboardList &operator=( const igdeGDCBillboardList &list );
	/*@}*/
};

#endif
