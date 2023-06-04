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

#ifndef _IGDESPINTEXTFIELDREFERENCE_H_
#define _IGDESPINTEXTFIELDREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeSpinTextField;


/**
 * \brief SpinTextField reference.
 * 
 * Safe way to hold an textfield reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member textfield. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeSpinTextFieldReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty textfield reference holder. */
	igdeSpinTextFieldReference();
	
	/**
	 * \brief Create textfield reference holder with textfield.
	 * 
	 * Reference is added if textfield is not NULL.
	 */
	igdeSpinTextFieldReference( igdeSpinTextField *textfield );
	
	/**
	 * \brief Create textfield reference holder with textfield from another holder.
	 * 
	 * Reference is added if textfield in holder is not NULL.
	 */
	igdeSpinTextFieldReference( const igdeSpinTextFieldReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set textfield without adding reference.
	 * 
	 * Use this method if the textfield to hold has been added a reference already. This is
	 * the case with created textfields as well as certain methods returning newly created
	 * textfields. In all these cases the textfield has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em textfield to be a NULL textfield.
	 */
	void TakeOver( igdeSpinTextField *textfield );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if textfield is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to textfield. */
	operator igdeSpinTextField*() const;
	
	/**
	 * \brief Reference to textfield.
	 * 
	 * \throws deeNullPointer if textfield is NULL.
	 */
	operator igdeSpinTextField&() const;
	
	/**
	 * \brief Pointer to textfield.
	 * 
	 * \throws deeNullPointer if textfield is NULL.
	 */
	igdeSpinTextField* operator->() const;
	
	/**
	 * \brief Store textfield.
	 * 
	 * If an textfield is already held its reference is release and the new textfield
	 * stored. If the new textfield is not NULL a reference is added.
	 */
	igdeSpinTextFieldReference &operator=( igdeSpinTextField *textfield );
	
	/**
	 * \brief Store textfield.
	 * 
	 * If an textfield is already held its reference is release and the new textfield
	 * stored. If the new textfield is not NULL a reference is added.
	 */
	igdeSpinTextFieldReference &operator=( const igdeSpinTextFieldReference &reference );
	
	/** \brief Test if textfield is held by this holder. */
	bool operator==( igdeSpinTextField *textfield ) const;
	bool operator!=( igdeSpinTextField *textfield ) const;
	
	/** \brief Test if textfield is held by this holder. */
	bool operator==( const igdeSpinTextFieldReference &reference ) const;
	bool operator!=( const igdeSpinTextFieldReference &reference ) const;
	/*@}*/
};

#endif
