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

#ifndef _IGDEVIEWCURVEBEZIERREFERENCE_H_
#define _IGDEVIEWCURVEBEZIERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeViewCurveBezier;


/**
 * \brief ViewCurveBezier reference.
 * 
 * Safe way to hold an viewCurveBezier reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member viewCurveBezier. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeViewCurveBezierReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty viewCurveBezier reference holder. */
	igdeViewCurveBezierReference();
	
	/**
	 * \brief Create viewCurveBezier reference holder with viewCurveBezier.
	 * 
	 * Reference is added if viewCurveBezier is not NULL.
	 */
	igdeViewCurveBezierReference( igdeViewCurveBezier *viewCurveBezier );
	
	/**
	 * \brief Create viewCurveBezier reference holder with viewCurveBezier from another holder.
	 * 
	 * Reference is added if viewCurveBezier in holder is not NULL.
	 */
	igdeViewCurveBezierReference( const igdeViewCurveBezierReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set viewCurveBezier without adding reference.
	 * 
	 * Use this method if the viewCurveBezier to hold has been added a reference already. This is
	 * the case with created viewCurveBeziers as well as certain methods returning newly created
	 * viewCurveBeziers. In all these cases the viewCurveBezier has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em viewCurveBezier to be a NULL viewCurveBezier.
	 */
	void TakeOver( igdeViewCurveBezier *viewCurveBezier );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if viewCurveBezier is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to viewCurveBezier. */
	operator igdeViewCurveBezier*() const;
	
	/**
	 * \brief Reference to viewCurveBezier.
	 * 
	 * \throws deeNullPointer if viewCurveBezier is NULL.
	 */
	operator igdeViewCurveBezier&() const;
	
	/**
	 * \brief Pointer to viewCurveBezier.
	 * 
	 * \throws deeNullPointer if viewCurveBezier is NULL.
	 */
	igdeViewCurveBezier* operator->() const;
	
	/**
	 * \brief Store viewCurveBezier.
	 * 
	 * If an viewCurveBezier is already held its reference is release and the new viewCurveBezier
	 * stored. If the new viewCurveBezier is not NULL a reference is added.
	 */
	igdeViewCurveBezierReference &operator=( igdeViewCurveBezier *viewCurveBezier );
	
	/**
	 * \brief Store viewCurveBezier.
	 * 
	 * If an viewCurveBezier is already held its reference is release and the new viewCurveBezier
	 * stored. If the new viewCurveBezier is not NULL a reference is added.
	 */
	igdeViewCurveBezierReference &operator=( const igdeViewCurveBezierReference &reference );
	
	/** \brief Test if viewCurveBezier is held by this holder. */
	bool operator==( igdeViewCurveBezier *viewCurveBezier ) const;
	bool operator!=( igdeViewCurveBezier *viewCurveBezier ) const;
	
	/** \brief Test if viewCurveBezier is held by this holder. */
	bool operator==( const igdeViewCurveBezierReference &reference ) const;
	bool operator!=( const igdeViewCurveBezierReference &reference ) const;
	/*@}*/
};

#endif
