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

#ifndef _IGDELISTBOXREFERENCE_H_
#define _IGDELISTBOXREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeListBox;


/**
 * \brief ListBox reference.
 * 
 * Safe way to hold an listbox reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member listbox. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeListBoxReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty listbox reference holder. */
	igdeListBoxReference();
	
	/**
	 * \brief Create listbox reference holder with listbox.
	 * 
	 * Reference is added if listbox is not NULL.
	 */
	igdeListBoxReference( igdeListBox *listbox );
	
	/**
	 * \brief Create listbox reference holder with listbox from another holder.
	 * 
	 * Reference is added if listbox in holder is not NULL.
	 */
	igdeListBoxReference( const igdeListBoxReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set listbox without adding reference.
	 * 
	 * Use this method if the listbox to hold has been added a reference already. This is
	 * the case with created listboxs as well as certain methods returning newly created
	 * listboxs. In all these cases the listbox has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em listbox to be a NULL listbox.
	 */
	void TakeOver( igdeListBox *listbox );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if listbox is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to listbox. */
	operator igdeListBox*() const;
	
	/**
	 * \brief Reference to listbox.
	 * 
	 * \throws deeNullPointer if listbox is NULL.
	 */
	operator igdeListBox&() const;
	
	/**
	 * \brief Pointer to listbox.
	 * 
	 * \throws deeNullPointer if listbox is NULL.
	 */
	igdeListBox* operator->() const;
	
	/**
	 * \brief Store listbox.
	 * 
	 * If an listbox is already held its reference is release and the new listbox
	 * stored. If the new listbox is not NULL a reference is added.
	 */
	igdeListBoxReference &operator=( igdeListBox *listbox );
	
	/**
	 * \brief Store listbox.
	 * 
	 * If an listbox is already held its reference is release and the new listbox
	 * stored. If the new listbox is not NULL a reference is added.
	 */
	igdeListBoxReference &operator=( const igdeListBoxReference &reference );
	
	/** \brief Test if listbox is held by this holder. */
	bool operator==( igdeListBox *listbox ) const;
	bool operator!=( igdeListBox *listbox ) const;
	
	/** \brief Test if listbox is held by this holder. */
	bool operator==( const igdeListBoxReference &reference ) const;
	bool operator!=( const igdeListBoxReference &reference ) const;
	/*@}*/
};

#endif
