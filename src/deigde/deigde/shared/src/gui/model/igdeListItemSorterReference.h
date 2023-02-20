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

#ifndef _IGDELISTITEMSORTERREFERENCE_H_
#define _IGDELISTITEMSORTERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeListItemSorter;


/**
 * \brief List item sorter reference.
 * 
 * Safe way to hold an list item sorter reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member list item sorter. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeListItemSorterReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty list item sorter reference holder. */
	igdeListItemSorterReference();
	
	/**
	 * \brief Create list item sorter reference holder with list item sorter.
	 * 
	 * Reference is added if list item sorter is not NULL.
	 */
	igdeListItemSorterReference( igdeListItemSorter *sorter );
	
	/**
	 * \brief Create list item sorter reference holder with list item sorter from another holder.
	 * 
	 * Reference is added if list item sorter in holder is not NULL.
	 */
	igdeListItemSorterReference( const igdeListItemSorterReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set list item sorter without adding reference.
	 * 
	 * Use this method if the list item sorter to hold has been added a reference already. This is
	 * the case with created list item sorters as well as certain methods returning newly created
	 * list item sorters. In all these cases the list item sorter has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em list item sorter to be a NULL list item sorter.
	 */
	void TakeOver( igdeListItemSorter *sorter );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if list item sorter is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to list item sorter. */
	operator igdeListItemSorter*() const;
	
	/**
	 * \brief Reference to list item sorter.
	 * 
	 * \throws deeNullPointer if list item sorter is NULL.
	 */
	operator igdeListItemSorter&() const;
	
	/**
	 * \brief Pointer to list item sorter.
	 * 
	 * \throws deeNullPointer if list item sorter is NULL.
	 */
	igdeListItemSorter* operator->() const;
	
	/**
	 * \brief Store list item sorter.
	 * 
	 * If an list item sorter is already held its reference is release and the new list item sorter
	 * stored. If the new list item sorter is not NULL a reference is added.
	 */
	igdeListItemSorterReference &operator=( igdeListItemSorter *sorter );
	
	/**
	 * \brief Store list item sorter.
	 * 
	 * If an list item sorter is already held its reference is release and the new list item sorter
	 * stored. If the new list item sorter is not NULL a reference is added.
	 */
	igdeListItemSorterReference &operator=( const igdeListItemSorterReference &reference );
	
	/** \brief Test if list item sorter is held by this holder. */
	bool operator==( igdeListItemSorter *sorter ) const;
	bool operator!=( igdeListItemSorter *sorter ) const;
	
	/** \brief Test if list item sorter is held by this holder. */
	bool operator==( const igdeListItemSorterReference &reference ) const;
	bool operator!=( const igdeListItemSorterReference &reference ) const;
	/*@}*/
};

#endif
