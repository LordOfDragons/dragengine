/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _IGDETREEITEMREFERENCE_H_
#define _IGDETREEITEMREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeTreeItem;


/**
 * \brief Tree item reference.
 * 
 * Safe way to hold an tree item reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member tree item. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeTreeItemReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty tree item reference holder. */
	igdeTreeItemReference();
	
	/**
	 * \brief Create tree item reference holder with tree item.
	 * 
	 * Reference is added if tree item is not NULL.
	 */
	igdeTreeItemReference( igdeTreeItem *treeItem );
	
	/**
	 * \brief Create tree item reference holder with tree item from another holder.
	 * 
	 * Reference is added if tree item in holder is not NULL.
	 */
	igdeTreeItemReference( const igdeTreeItemReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set tree item without adding reference.
	 * 
	 * Use this method if the tree item to hold has been added a reference already. This is
	 * the case with created tree items as well as certain methods returning newly created
	 * tree items. In all these cases the tree item has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em tree item to be a NULL tree item.
	 */
	void TakeOver( igdeTreeItem *treeItem );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if tree item is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to tree item. */
	operator igdeTreeItem*() const;
	
	/**
	 * \brief Reference to tree item.
	 * 
	 * \throws deeNullPointer if tree item is NULL.
	 */
	operator igdeTreeItem&() const;
	
	/**
	 * \brief Pointer to tree item.
	 * 
	 * \throws deeNullPointer if tree item is NULL.
	 */
	igdeTreeItem* operator->() const;
	
	/**
	 * \brief Store tree item.
	 * 
	 * If an tree item is already held its reference is release and the new tree item
	 * stored. If the new tree item is not NULL a reference is added.
	 */
	igdeTreeItemReference &operator=( igdeTreeItem *treeItem );
	
	/**
	 * \brief Store tree item.
	 * 
	 * If an tree item is already held its reference is release and the new tree item
	 * stored. If the new tree item is not NULL a reference is added.
	 */
	igdeTreeItemReference &operator=( const igdeTreeItemReference &reference );
	
	/** \brief Test if tree item is held by this holder. */
	bool operator==( igdeTreeItem *treeItem ) const;
	bool operator!=( igdeTreeItem *treeItem ) const;
	
	/** \brief Test if tree item is held by this holder. */
	bool operator==( const igdeTreeItemReference &reference ) const;
	bool operator!=( const igdeTreeItemReference &reference ) const;
	/*@}*/
};

#endif
