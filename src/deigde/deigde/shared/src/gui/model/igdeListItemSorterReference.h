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
