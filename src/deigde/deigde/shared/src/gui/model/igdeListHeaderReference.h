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

#ifndef _IGDELISTHEADERREFERENCE_H_
#define _IGDELISTHEADERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeListHeader;


/**
 * \brief List header reference.
 * 
 * Safe way to hold an list header reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member list header. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeListHeaderReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty list header reference holder. */
	igdeListHeaderReference();
	
	/**
	 * \brief Create list header reference holder with list header.
	 * 
	 * Reference is added if list header is not NULL.
	 */
	igdeListHeaderReference( igdeListHeader *header );
	
	/**
	 * \brief Create list header reference holder with list header from another holder.
	 * 
	 * Reference is added if list header in holder is not NULL.
	 */
	igdeListHeaderReference( const igdeListHeaderReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set list header without adding reference.
	 * 
	 * Use this method if the list header to hold has been added a reference already. This is
	 * the case with created list headers as well as certain methods returning newly created
	 * list headers. In all these cases the list header has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em list header to be a NULL list header.
	 */
	void TakeOver( igdeListHeader *header );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if list header is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to list header. */
	operator igdeListHeader*() const;
	
	/**
	 * \brief Reference to list header.
	 * 
	 * \throws deeNullPointer if list header is NULL.
	 */
	operator igdeListHeader&() const;
	
	/**
	 * \brief Pointer to list header.
	 * 
	 * \throws deeNullPointer if list header is NULL.
	 */
	igdeListHeader* operator->() const;
	
	/**
	 * \brief Store list header.
	 * 
	 * If an list header is already held its reference is release and the new list header
	 * stored. If the new list header is not NULL a reference is added.
	 */
	igdeListHeaderReference &operator=( igdeListHeader *header );
	
	/**
	 * \brief Store list header.
	 * 
	 * If an list header is already held its reference is release and the new list header
	 * stored. If the new list header is not NULL a reference is added.
	 */
	igdeListHeaderReference &operator=( const igdeListHeaderReference &reference );
	
	/** \brief Test if list header is held by this holder. */
	bool operator==( igdeListHeader *header ) const;
	bool operator!=( igdeListHeader *header ) const;
	
	/** \brief Test if list header is held by this holder. */
	bool operator==( const igdeListHeaderReference &reference ) const;
	bool operator!=( const igdeListHeaderReference &reference ) const;
	/*@}*/
};

#endif
