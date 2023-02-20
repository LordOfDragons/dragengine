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

#ifndef _IGDECOMBOBOXFILTERREFERENCE_H_
#define _IGDECOMBOBOXFILTERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeComboBoxFilter;


/**
 * \brief ComboBoxFilter reference.
 * 
 * Safe way to hold an combobox reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member combobox. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeComboBoxFilterReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty combobox reference holder. */
	igdeComboBoxFilterReference();
	
	/**
	 * \brief Create combobox reference holder with combobox.
	 * 
	 * Reference is added if combobox is not NULL.
	 */
	igdeComboBoxFilterReference( igdeComboBoxFilter *combobox );
	
	/**
	 * \brief Create combobox reference holder with combobox from another holder.
	 * 
	 * Reference is added if combobox in holder is not NULL.
	 */
	igdeComboBoxFilterReference( const igdeComboBoxFilterReference &reference );
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
	void TakeOver( igdeComboBoxFilter *combobox );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if combobox is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to combobox. */
	operator igdeComboBoxFilter*() const;
	
	/**
	 * \brief Reference to combobox.
	 * 
	 * \throws deeNullPointer if combobox is NULL.
	 */
	operator igdeComboBoxFilter&() const;
	
	/**
	 * \brief Pointer to combobox.
	 * 
	 * \throws deeNullPointer if combobox is NULL.
	 */
	igdeComboBoxFilter* operator->() const;
	
	/**
	 * \brief Store combobox.
	 * 
	 * If an combobox is already held its reference is release and the new combobox
	 * stored. If the new combobox is not NULL a reference is added.
	 */
	igdeComboBoxFilterReference &operator=( igdeComboBoxFilter *combobox );
	
	/**
	 * \brief Store combobox.
	 * 
	 * If an combobox is already held its reference is release and the new combobox
	 * stored. If the new combobox is not NULL a reference is added.
	 */
	igdeComboBoxFilterReference &operator=( const igdeComboBoxFilterReference &reference );
	
	/** \brief Test if combobox is held by this holder. */
	bool operator==( igdeComboBoxFilter *combobox ) const;
	bool operator!=( igdeComboBoxFilter *combobox ) const;
	
	/** \brief Test if combobox is held by this holder. */
	bool operator==( const igdeComboBoxFilterReference &reference ) const;
	bool operator!=( const igdeComboBoxFilterReference &reference ) const;
	/*@}*/
};

#endif
