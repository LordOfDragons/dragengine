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

#ifndef _IGDETREEITEMSORTERREFERENCE_H_
#define _IGDETREEITEMSORTERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeTreeItemSorter;


/**
 * \brief Tree item sorter reference.
 * 
 * Safe way to hold an tree item sorter reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member tree item sorter. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeTreeItemSorterReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty tree item sorter reference holder. */
	igdeTreeItemSorterReference();
	
	/**
	 * \brief Create tree item sorter reference holder with tree item sorter.
	 * 
	 * Reference is added if tree item sorter is not NULL.
	 */
	igdeTreeItemSorterReference( igdeTreeItemSorter *sorter );
	
	/**
	 * \brief Create tree item sorter reference holder with tree item sorter from another holder.
	 * 
	 * Reference is added if tree item sorter in holder is not NULL.
	 */
	igdeTreeItemSorterReference( const igdeTreeItemSorterReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set tree item sorter without adding reference.
	 * 
	 * Use this method if the tree item sorter to hold has been added a reference already. This is
	 * the case with created tree item sorters as well as certain methods returning newly created
	 * tree item sorters. In all these cases the tree item sorter has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em tree item sorter to be a NULL tree item sorter.
	 */
	void TakeOver( igdeTreeItemSorter *sorter );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if tree item sorter is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to tree item sorter. */
	operator igdeTreeItemSorter*() const;
	
	/**
	 * \brief Reference to tree item sorter.
	 * 
	 * \throws deeNullPointer if tree item sorter is NULL.
	 */
	operator igdeTreeItemSorter&() const;
	
	/**
	 * \brief Pointer to tree item sorter.
	 * 
	 * \throws deeNullPointer if tree item sorter is NULL.
	 */
	igdeTreeItemSorter* operator->() const;
	
	/**
	 * \brief Store tree item sorter.
	 * 
	 * If an tree item sorter is already held its reference is release and the new tree item sorter
	 * stored. If the new tree item sorter is not NULL a reference is added.
	 */
	igdeTreeItemSorterReference &operator=( igdeTreeItemSorter *sorter );
	
	/**
	 * \brief Store tree item sorter.
	 * 
	 * If an tree item sorter is already held its reference is release and the new tree item sorter
	 * stored. If the new tree item sorter is not NULL a reference is added.
	 */
	igdeTreeItemSorterReference &operator=( const igdeTreeItemSorterReference &reference );
	
	/** \brief Test if tree item sorter is held by this holder. */
	bool operator==( igdeTreeItemSorter *sorter ) const;
	bool operator!=( igdeTreeItemSorter *sorter ) const;
	
	/** \brief Test if tree item sorter is held by this holder. */
	bool operator==( const igdeTreeItemSorterReference &reference ) const;
	bool operator!=( const igdeTreeItemSorterReference &reference ) const;
	/*@}*/
};

#endif
