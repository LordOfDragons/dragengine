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

#ifndef _IGDETREELISTREFERENCE_H_
#define _IGDETREELISTREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeTreeList;


/**
 * \brief TreeList reference.
 * 
 * Safe way to hold an treeList reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member treeList. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeTreeListReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty treeList reference holder. */
	igdeTreeListReference();
	
	/**
	 * \brief Create treeList reference holder with treeList.
	 * 
	 * Reference is added if treeList is not NULL.
	 */
	igdeTreeListReference( igdeTreeList *treeList );
	
	/**
	 * \brief Create treeList reference holder with treeList from another holder.
	 * 
	 * Reference is added if treeList in holder is not NULL.
	 */
	igdeTreeListReference( const igdeTreeListReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set treeList without adding reference.
	 * 
	 * Use this method if the treeList to hold has been added a reference already. This is
	 * the case with created treeLists as well as certain methods returning newly created
	 * treeLists. In all these cases the treeList has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em treeList to be a NULL treeList.
	 */
	void TakeOver( igdeTreeList *treeList );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if treeList is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to treeList. */
	operator igdeTreeList*() const;
	
	/**
	 * \brief Reference to treeList.
	 * 
	 * \throws deeNullPointer if treeList is NULL.
	 */
	operator igdeTreeList&() const;
	
	/**
	 * \brief Pointer to treeList.
	 * 
	 * \throws deeNullPointer if treeList is NULL.
	 */
	igdeTreeList* operator->() const;
	
	/**
	 * \brief Store treeList.
	 * 
	 * If an treeList is already held its reference is release and the new treeList
	 * stored. If the new treeList is not NULL a reference is added.
	 */
	igdeTreeListReference &operator=( igdeTreeList *treeList );
	
	/**
	 * \brief Store treeList.
	 * 
	 * If an treeList is already held its reference is release and the new treeList
	 * stored. If the new treeList is not NULL a reference is added.
	 */
	igdeTreeListReference &operator=( const igdeTreeListReference &reference );
	
	/** \brief Test if treeList is held by this holder. */
	bool operator==( igdeTreeList *treeList ) const;
	bool operator!=( igdeTreeList *treeList ) const;
	
	/** \brief Test if treeList is held by this holder. */
	bool operator==( const igdeTreeListReference &reference ) const;
	bool operator!=( const igdeTreeListReference &reference ) const;
	/*@}*/
};

#endif
