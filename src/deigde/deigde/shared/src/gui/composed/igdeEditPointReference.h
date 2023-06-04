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

#ifndef _IGDEEDITPOINTREFERENCE_H_
#define _IGDEEDITPOINTREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeEditPoint;


/**
 * \brief Edit point reference.
 * 
 * Safe way to hold an edit point reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member edit point. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeEditPointReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty edit point reference holder. */
	igdeEditPointReference();
	
	/**
	 * \brief Create edit point reference holder with edit point.
	 * 
	 * Reference is added if edit point is not NULL.
	 */
	igdeEditPointReference( igdeEditPoint *editPoint );
	
	/**
	 * \brief Create edit point reference holder with edit point from another holder.
	 * 
	 * Reference is added if edit point in holder is not NULL.
	 */
	igdeEditPointReference( const igdeEditPointReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set edit point without adding reference.
	 * 
	 * Use this method if the edit point to hold has been added a reference already. This is
	 * the case with created edit points as well as certain methods returning newly created
	 * edit points. In all these cases the edit point has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em edit point to be a NULL edit point.
	 */
	void TakeOver( igdeEditPoint *editPoint );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if edit point is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to edit point. */
	operator igdeEditPoint*() const;
	
	/**
	 * \brief Reference to edit point.
	 * 
	 * \throws deeNullPointer if edit point is NULL.
	 */
	operator igdeEditPoint&() const;
	
	/**
	 * \brief Pointer to edit point.
	 * 
	 * \throws deeNullPointer if edit point is NULL.
	 */
	igdeEditPoint* operator->() const;
	
	/**
	 * \brief Store edit point.
	 * 
	 * If an edit point is already held its reference is release and the new edit point
	 * stored. If the new edit point is not NULL a reference is added.
	 */
	igdeEditPointReference &operator=( igdeEditPoint *editPoint );
	
	/**
	 * \brief Store edit point.
	 * 
	 * If an edit point is already held its reference is release and the new edit point
	 * stored. If the new edit point is not NULL a reference is added.
	 */
	igdeEditPointReference &operator=( const igdeEditPointReference &reference );
	
	/** \brief Test if edit point is held by this holder. */
	bool operator==( igdeEditPoint *editPoint ) const;
	bool operator!=( igdeEditPoint *editPoint ) const;
	
	/** \brief Test if edit point is held by this holder. */
	bool operator==( const igdeEditPointReference &reference ) const;
	bool operator!=( const igdeEditPointReference &reference ) const;
	/*@}*/
};

#endif
