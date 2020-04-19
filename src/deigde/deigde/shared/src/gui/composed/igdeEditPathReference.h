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

#ifndef _IGDEEDITPATHREFERENCE_H_
#define _IGDEEDITPATHREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeEditPath;


/**
 * \brief EditPath reference.
 * 
 * Safe way to hold an EditPath reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member EditPath. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeEditPathReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty EditPath reference holder. */
	igdeEditPathReference();
	
	/**
	 * \brief Create EditPath reference holder with EditPath.
	 * 
	 * Reference is added if EditPath is not NULL.
	 */
	igdeEditPathReference( igdeEditPath *editPath );
	
	/**
	 * \brief Create EditPath reference holder with EditPath from another holder.
	 * 
	 * Reference is added if EditPath in holder is not NULL.
	 */
	igdeEditPathReference( const igdeEditPathReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set EditPath without adding reference.
	 * 
	 * Use this method if the EditPath to hold has been added a reference already. This is
	 * the case with created EditPaths as well as certain methods returning newly created
	 * EditPaths. In all these cases the EditPath has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em EditPath to be a NULL EditPath.
	 */
	void TakeOver( igdeEditPath *editPath );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if EditPath is not NULL. */
	operator bool() const;
	
	/** \brief Pather to EditPath. */
	operator igdeEditPath*() const;
	
	/**
	 * \brief Reference to EditPath.
	 * 
	 * \throws deeNullPather if EditPath is NULL.
	 */
	operator igdeEditPath&() const;
	
	/**
	 * \brief Pather to EditPath.
	 * 
	 * \throws deeNullPather if EditPath is NULL.
	 */
	igdeEditPath* operator->() const;
	
	/**
	 * \brief Store EditPath.
	 * 
	 * If an EditPath is already held its reference is release and the new EditPath
	 * stored. If the new EditPath is not NULL a reference is added.
	 */
	igdeEditPathReference &operator=( igdeEditPath *editPath );
	
	/**
	 * \brief Store EditPath.
	 * 
	 * If an EditPath is already held its reference is release and the new EditPath
	 * stored. If the new EditPath is not NULL a reference is added.
	 */
	igdeEditPathReference &operator=( const igdeEditPathReference &reference );
	
	/** \brief Test if EditPath is held by this holder. */
	bool operator==( igdeEditPath *editPath ) const;
	bool operator!=( igdeEditPath *editPath ) const;
	
	/** \brief Test if EditPath is held by this holder. */
	bool operator==( const igdeEditPathReference &reference ) const;
	bool operator!=( const igdeEditPathReference &reference ) const;
	/*@}*/
};

#endif
