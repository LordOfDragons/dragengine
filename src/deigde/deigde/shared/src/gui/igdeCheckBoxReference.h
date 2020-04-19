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

#ifndef _IGDECHECKBOXREFERENCE_H_
#define _IGDECHECKBOXREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeCheckBox;


/**
 * \brief CheckBox reference.
 * 
 * Safe way to hold an checkbox reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member checkbox. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeCheckBoxReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty checkbox reference holder. */
	igdeCheckBoxReference();
	
	/**
	 * \brief Create checkbox reference holder with checkbox.
	 * 
	 * Reference is added if checkbox is not NULL.
	 */
	igdeCheckBoxReference( igdeCheckBox *checkbox );
	
	/**
	 * \brief Create checkbox reference holder with checkbox from another holder.
	 * 
	 * Reference is added if checkbox in holder is not NULL.
	 */
	igdeCheckBoxReference( const igdeCheckBoxReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set checkbox without adding reference.
	 * 
	 * Use this method if the checkbox to hold has been added a reference already. This is
	 * the case with created checkboxs as well as certain methods returning newly created
	 * checkboxs. In all these cases the checkbox has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em checkbox to be a NULL checkbox.
	 */
	void TakeOver( igdeCheckBox *checkbox );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if checkbox is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to checkbox. */
	operator igdeCheckBox*() const;
	
	/**
	 * \brief Reference to checkbox.
	 * 
	 * \throws deeNullPointer if checkbox is NULL.
	 */
	operator igdeCheckBox&() const;
	
	/**
	 * \brief Pointer to checkbox.
	 * 
	 * \throws deeNullPointer if checkbox is NULL.
	 */
	igdeCheckBox* operator->() const;
	
	/**
	 * \brief Store checkbox.
	 * 
	 * If an checkbox is already held its reference is release and the new checkbox
	 * stored. If the new checkbox is not NULL a reference is added.
	 */
	igdeCheckBoxReference &operator=( igdeCheckBox *checkbox );
	
	/**
	 * \brief Store checkbox.
	 * 
	 * If an checkbox is already held its reference is release and the new checkbox
	 * stored. If the new checkbox is not NULL a reference is added.
	 */
	igdeCheckBoxReference &operator=( const igdeCheckBoxReference &reference );
	
	/** \brief Test if checkbox is held by this holder. */
	bool operator==( igdeCheckBox *checkbox ) const;
	bool operator!=( igdeCheckBox *checkbox ) const;
	
	/** \brief Test if checkbox is held by this holder. */
	bool operator==( const igdeCheckBoxReference &reference ) const;
	bool operator!=( const igdeCheckBoxReference &reference ) const;
	/*@}*/
};

#endif
