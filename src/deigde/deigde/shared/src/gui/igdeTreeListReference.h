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
