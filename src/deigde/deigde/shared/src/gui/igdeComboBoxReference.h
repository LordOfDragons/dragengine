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

#ifndef _IGDECOMBOBOXREFERENCE_H_
#define _IGDECOMBOBOXREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeComboBox;


/**
 * \brief ComboBox reference.
 * 
 * Safe way to hold an combobox reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member combobox. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeComboBoxReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty combobox reference holder. */
	igdeComboBoxReference();
	
	/**
	 * \brief Create combobox reference holder with combobox.
	 * 
	 * Reference is added if combobox is not NULL.
	 */
	igdeComboBoxReference( igdeComboBox *combobox );
	
	/**
	 * \brief Create combobox reference holder with combobox from another holder.
	 * 
	 * Reference is added if combobox in holder is not NULL.
	 */
	igdeComboBoxReference( const igdeComboBoxReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set combobox without adding reference.
	 * 
	 * Use this method if the combobox to hold has been added a reference already. This is
	 * the case with created comboboxs as well as certain methods returning newly created
	 * comboboxs. In all these cases the combobox has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em combobox to be a NULL combobox.
	 */
	void TakeOver( igdeComboBox *combobox );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if combobox is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to combobox. */
	operator igdeComboBox*() const;
	
	/**
	 * \brief Reference to combobox.
	 * 
	 * \throws deeNullPointer if combobox is NULL.
	 */
	operator igdeComboBox&() const;
	
	/**
	 * \brief Pointer to combobox.
	 * 
	 * \throws deeNullPointer if combobox is NULL.
	 */
	igdeComboBox* operator->() const;
	
	/**
	 * \brief Store combobox.
	 * 
	 * If an combobox is already held its reference is release and the new combobox
	 * stored. If the new combobox is not NULL a reference is added.
	 */
	igdeComboBoxReference &operator=( igdeComboBox *combobox );
	
	/**
	 * \brief Store combobox.
	 * 
	 * If an combobox is already held its reference is release and the new combobox
	 * stored. If the new combobox is not NULL a reference is added.
	 */
	igdeComboBoxReference &operator=( const igdeComboBoxReference &reference );
	
	/** \brief Test if combobox is held by this holder. */
	bool operator==( igdeComboBox *combobox ) const;
	bool operator!=( igdeComboBox *combobox ) const;
	
	/** \brief Test if combobox is held by this holder. */
	bool operator==( const igdeComboBoxReference &reference ) const;
	bool operator!=( const igdeComboBoxReference &reference ) const;
	/*@}*/
};

#endif
