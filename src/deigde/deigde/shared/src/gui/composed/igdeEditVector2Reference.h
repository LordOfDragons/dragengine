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

#ifndef _IGDEEDITVECTOR2REFERENCE_H_
#define _IGDEEDITVECTOR2REFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeEditVector2;


/**
 * \brief Edit vector reference.
 * 
 * Safe way to hold an edit vector reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member edit vector. Do not use
 *       it as vectorer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeEditVector2Reference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty edit vector reference holder. */
	igdeEditVector2Reference();
	
	/**
	 * \brief Create edit vector reference holder with edit vector.
	 * 
	 * Reference is added if edit vector is not NULL.
	 */
	igdeEditVector2Reference( igdeEditVector2 *editVector2 );
	
	/**
	 * \brief Create edit vector reference holder with edit vector from another holder.
	 * 
	 * Reference is added if edit vector in holder is not NULL.
	 */
	igdeEditVector2Reference( const igdeEditVector2Reference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set edit vector without adding reference.
	 * 
	 * Use this method if the edit vector to hold has been added a reference already. This is
	 * the case with created edit vectors as well as certain methods returning newly created
	 * edit vectors. In all these cases the edit vector has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em edit vector to be a NULL edit vector.
	 */
	void TakeOver( igdeEditVector2 *editVector2 );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if edit vector is not NULL. */
	operator bool() const;
	
	/** \brief Vector2er to edit vector. */
	operator igdeEditVector2*() const;
	
	/**
	 * \brief Reference to edit vector.
	 * 
	 * \throws deeNullVector2er if edit vector is NULL.
	 */
	operator igdeEditVector2&() const;
	
	/**
	 * \brief Vector2er to edit vector.
	 * 
	 * \throws deeNullVector2er if edit vector is NULL.
	 */
	igdeEditVector2* operator->() const;
	
	/**
	 * \brief Store edit vector.
	 * 
	 * If an edit vector is already held its reference is release and the new edit vector
	 * stored. If the new edit vector is not NULL a reference is added.
	 */
	igdeEditVector2Reference &operator=( igdeEditVector2 *editVector2 );
	
	/**
	 * \brief Store edit vector.
	 * 
	 * If an edit vector is already held its reference is release and the new edit vector
	 * stored. If the new edit vector is not NULL a reference is added.
	 */
	igdeEditVector2Reference &operator=( const igdeEditVector2Reference &reference );
	
	/** \brief Test if edit vector is held by this holder. */
	bool operator==( igdeEditVector2 *editVector2 ) const;
	bool operator!=( igdeEditVector2 *editVector2 ) const;
	
	/** \brief Test if edit vector is held by this holder. */
	bool operator==( const igdeEditVector2Reference &reference ) const;
	bool operator!=( const igdeEditVector2Reference &reference ) const;
	/*@}*/
};

#endif
