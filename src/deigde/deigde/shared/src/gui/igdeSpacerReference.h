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

#ifndef _IGDESPACERREFERENCE_H_
#define _IGDESPACERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeSpacer;


/**
 * \brief Spacer reference.
 * 
 * Safe way to hold an spacer reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member spacer. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeSpacerReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty spacer reference holder. */
	igdeSpacerReference();
	
	/**
	 * \brief Create spacer reference holder with spacer.
	 * 
	 * Reference is added if spacer is not NULL.
	 */
	igdeSpacerReference( igdeSpacer *spacer );
	
	/**
	 * \brief Create spacer reference holder with spacer from another holder.
	 * 
	 * Reference is added if spacer in holder is not NULL.
	 */
	igdeSpacerReference( const igdeSpacerReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set spacer without adding reference.
	 * 
	 * Use this method if the spacer to hold has been added a reference already. This is
	 * the case with created spacers as well as certain methods returning newly created
	 * spacers. In all these cases the spacer has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em spacer to be a NULL spacer.
	 */
	void TakeOver( igdeSpacer *spacer );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if spacer is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to spacer. */
	operator igdeSpacer*() const;
	
	/**
	 * \brief Reference to spacer.
	 * 
	 * \throws deeNullPointer if spacer is NULL.
	 */
	operator igdeSpacer&() const;
	
	/**
	 * \brief Pointer to spacer.
	 * 
	 * \throws deeNullPointer if spacer is NULL.
	 */
	igdeSpacer* operator->() const;
	
	/**
	 * \brief Store spacer.
	 * 
	 * If an spacer is already held its reference is release and the new spacer
	 * stored. If the new spacer is not NULL a reference is added.
	 */
	igdeSpacerReference &operator=( igdeSpacer *spacer );
	
	/**
	 * \brief Store spacer.
	 * 
	 * If an spacer is already held its reference is release and the new spacer
	 * stored. If the new spacer is not NULL a reference is added.
	 */
	igdeSpacerReference &operator=( const igdeSpacerReference &reference );
	
	/** \brief Test if spacer is held by this holder. */
	bool operator==( igdeSpacer *spacer ) const;
	bool operator!=( igdeSpacer *spacer ) const;
	
	/** \brief Test if spacer is held by this holder. */
	bool operator==( const igdeSpacerReference &reference ) const;
	bool operator!=( const igdeSpacerReference &reference ) const;
	/*@}*/
};

#endif
