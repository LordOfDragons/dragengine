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

#ifndef _IGDELISTITEMREFERENCE_H_
#define _IGDELISTITEMREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeListItem;


/**
 * \brief List item reference.
 * 
 * Safe way to hold an list item reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member list item. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeListItemReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty list item reference holder. */
	igdeListItemReference();
	
	/**
	 * \brief Create list item reference holder with list item.
	 * 
	 * Reference is added if list item is not NULL.
	 */
	igdeListItemReference( igdeListItem *listItem );
	
	/**
	 * \brief Create list item reference holder with list item from another holder.
	 * 
	 * Reference is added if list item in holder is not NULL.
	 */
	igdeListItemReference( const igdeListItemReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set list item without adding reference.
	 * 
	 * Use this method if the list item to hold has been added a reference already. This is
	 * the case with created list items as well as certain methods returning newly created
	 * list items. In all these cases the list item has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em list item to be a NULL list item.
	 */
	void TakeOver( igdeListItem *listItem );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if list item is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to list item. */
	operator igdeListItem*() const;
	
	/**
	 * \brief Reference to list item.
	 * 
	 * \throws deeNullPointer if list item is NULL.
	 */
	operator igdeListItem&() const;
	
	/**
	 * \brief Pointer to list item.
	 * 
	 * \throws deeNullPointer if list item is NULL.
	 */
	igdeListItem* operator->() const;
	
	/**
	 * \brief Store list item.
	 * 
	 * If an list item is already held its reference is release and the new list item
	 * stored. If the new list item is not NULL a reference is added.
	 */
	igdeListItemReference &operator=( igdeListItem *listItem );
	
	/**
	 * \brief Store list item.
	 * 
	 * If an list item is already held its reference is release and the new list item
	 * stored. If the new list item is not NULL a reference is added.
	 */
	igdeListItemReference &operator=( const igdeListItemReference &reference );
	
	/** \brief Test if list item is held by this holder. */
	bool operator==( igdeListItem *listItem ) const;
	bool operator!=( igdeListItem *listItem ) const;
	
	/** \brief Test if list item is held by this holder. */
	bool operator==( const igdeListItemReference &reference ) const;
	bool operator!=( const igdeListItemReference &reference ) const;
	/*@}*/
};

#endif
