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

#ifndef _IGDEGDCNAVIGATIONBLOCKERLIST_H_
#define _IGDEGDCNAVIGATIONBLOCKERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDCNavigationBlocker;



/**
 * \brief Game definition class navigation blocker list.
 */
class igdeGDCNavigationBlockerList{
private:
	decObjectOrderedSet pNavigationBlockers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeGDCNavigationBlockerList();
	
	/** \brief Create copy of a game definition class navigation blocker list. */
	igdeGDCNavigationBlockerList( const igdeGDCNavigationBlockerList &list );
	
	/** \brief Clean up list. */
	~igdeGDCNavigationBlockerList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of navigation blockers. */
	int GetCount() const;
	
	/** \brief Navigation blocker by index. */
	igdeGDCNavigationBlocker *GetAt( int index ) const;
	
	/** \brief Index of an navigation blocker or -1 if not found. */
	int IndexOf( igdeGDCNavigationBlocker *blocker ) const;
	
	/** \brief Determines if an navigation blocker exists. */
	bool Has( igdeGDCNavigationBlocker *blocker ) const;
	
	/** \brief Adds an navigation blocker. */
	void Add( igdeGDCNavigationBlocker *blocker );
	
	/** \brief Inserts a new navigation blocker. */
	void InsertAt( igdeGDCNavigationBlocker *blocker, int index );
	
	/** \brief Moves an navigation blocker to a new position. */
	void MoveTo( igdeGDCNavigationBlocker *blocker, int index );
	
	/** \brief Removes an navigation blocker. */
	void Remove( igdeGDCNavigationBlocker *blocker );
	
	/** \brief Removes all navigation blockers. */
	void RemoveAll();
	
	/** \brief Set this list to a deep copy of another list. */
	void SetToDeepCopyFrom( const igdeGDCNavigationBlockerList &list );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set list from another list. */
	igdeGDCNavigationBlockerList &operator=( const igdeGDCNavigationBlockerList &list );
	/*@}*/
};

#endif
