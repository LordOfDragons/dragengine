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

#ifndef _IGDEICONLISTBOXREFERENCE_H_
#define _IGDEICONLISTBOXREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeIconListBox;


/**
 * \brief IconListBox reference.
 * 
 * Safe way to hold an listbox reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member listbox. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeIconListBoxReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty listbox reference holder. */
	igdeIconListBoxReference();
	
	/**
	 * \brief Create listbox reference holder with listbox.
	 * 
	 * Reference is added if listbox is not NULL.
	 */
	igdeIconListBoxReference( igdeIconListBox *listbox );
	
	/**
	 * \brief Create listbox reference holder with listbox from another holder.
	 * 
	 * Reference is added if listbox in holder is not NULL.
	 */
	igdeIconListBoxReference( const igdeIconListBoxReference &reference );
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
	void TakeOver( igdeIconListBox *listbox );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if listbox is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to listbox. */
	operator igdeIconListBox*() const;
	
	/**
	 * \brief Reference to listbox.
	 * 
	 * \throws deeNullPointer if listbox is NULL.
	 */
	operator igdeIconListBox&() const;
	
	/**
	 * \brief Pointer to listbox.
	 * 
	 * \throws deeNullPointer if listbox is NULL.
	 */
	igdeIconListBox* operator->() const;
	
	/**
	 * \brief Store listbox.
	 * 
	 * If an listbox is already held its reference is release and the new listbox
	 * stored. If the new listbox is not NULL a reference is added.
	 */
	igdeIconListBoxReference &operator=( igdeIconListBox *listbox );
	
	/**
	 * \brief Store listbox.
	 * 
	 * If an listbox is already held its reference is release and the new listbox
	 * stored. If the new listbox is not NULL a reference is added.
	 */
	igdeIconListBoxReference &operator=( const igdeIconListBoxReference &reference );
	
	/** \brief Test if listbox is held by this holder. */
	bool operator==( igdeIconListBox *listbox ) const;
	bool operator!=( igdeIconListBox *listbox ) const;
	
	/** \brief Test if listbox is held by this holder. */
	bool operator==( const igdeIconListBoxReference &reference ) const;
	bool operator!=( const igdeIconListBoxReference &reference ) const;
	/*@}*/
};

#endif
