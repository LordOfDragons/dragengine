/* 
 * Drag[en]gine IGDE Animator Editor
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

#ifndef _AECONTROLLERTARGETLIST_H_
#define _AECONTROLLERTARGETLIST_H_

#include <dragengine/common/collection/decPointerList.h>

class aeControllerTarget;



/**
 * \brief Controller target List.
 */
class aeControllerTargetList{
private:
	decPointerList pTargets;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new controller target list. */
	aeControllerTargetList();
	
	/** \brief Create copy of a controller target list. */
	aeControllerTargetList( const aeControllerTargetList &copy );
	
	/** \brief Clean up controller target list. */
	~aeControllerTargetList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Number of controller targets. */
	int GetCount() const;
	
	/**
	 * \brief Controller target at index.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	aeControllerTarget *GetAt( int index ) const;
	
	/**
	 * \brief Index of controller target or -1 if not present in the list.
	 * \throws deeInvalidParam \em target is \em NULL.
	 */
	int IndexOf( aeControllerTarget *target ) const;
	
	/**
	 * \brief Determine if a controller target is present in the list.
	 * \throws deeInvalidParam \em target is \em NULL.
	 */
	bool Has( aeControllerTarget *target ) const;
	
	/**
	 * \brief Add controller target.
	 * \throws deeInvalidParam \em target is \em NULL.
	 */
	void Add( aeControllerTarget *target );
	
	/**
	 * \brief Insert controller target.
	 * \throws deeInvalidParam \em target is \em NULL.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	void Insert( aeControllerTarget *target, int index );
	
	/**
	 * \brief Move controller target.
	 * \throws deeInvalidParam \em target is \em NULL.
	 * \throws deeInvalidParam \em from is less than 0 or greater than GetCount()-1.
	 * \throws deeInvalidParam \em to is less than 0 or greater than GetCount().
	 */
	void Move( aeControllerTarget *target, int index );
	
	/**
	 * \brief Remove controller target.
	 * \throws deeInvalidParam \em target is \em NULL.
	 * \throws deeInvalidParam \em target is not present in the list.
	 */
	void Remove( aeControllerTarget *target );
	
	/** \brief Remove all controller targets. */
	void RemoveAll();
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy controller target list to this controller target list. */
	aeControllerTargetList &operator=( const aeControllerTargetList &list );
	/*@}*/
};

#endif
