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

#ifndef _IGDETABBOOKREFERENCE_H_
#define _IGDETABBOOKREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeTabBook;


/**
 * \brief TabBook reference.
 * 
 * Safe way to hold an tabbook reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member tabbook. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeTabBookReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty tabbook reference holder. */
	igdeTabBookReference();
	
	/**
	 * \brief Create tabbook reference holder with tabbook.
	 * 
	 * Reference is added if tabbook is not NULL.
	 */
	igdeTabBookReference( igdeTabBook *tabbook );
	
	/**
	 * \brief Create tabbook reference holder with tabbook from another holder.
	 * 
	 * Reference is added if tabbook in holder is not NULL.
	 */
	igdeTabBookReference( const igdeTabBookReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set tabbook without adding reference.
	 * 
	 * Use this method if the tabbook to hold has been added a reference already. This is
	 * the case with created tabbooks as well as certain methods returning newly created
	 * tabbooks. In all these cases the tabbook has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em tabbook to be a NULL tabbook.
	 */
	void TakeOver( igdeTabBook *tabbook );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if tabbook is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to tabbook. */
	operator igdeTabBook*() const;
	
	/**
	 * \brief Reference to tabbook.
	 * 
	 * \throws deeNullPointer if tabbook is NULL.
	 */
	operator igdeTabBook&() const;
	
	/**
	 * \brief Pointer to tabbook.
	 * 
	 * \throws deeNullPointer if tabbook is NULL.
	 */
	igdeTabBook* operator->() const;
	
	/**
	 * \brief Store tabbook.
	 * 
	 * If an tabbook is already held its reference is release and the new tabbook
	 * stored. If the new tabbook is not NULL a reference is added.
	 */
	igdeTabBookReference &operator=( igdeTabBook *tabbook );
	
	/**
	 * \brief Store tabbook.
	 * 
	 * If an tabbook is already held its reference is release and the new tabbook
	 * stored. If the new tabbook is not NULL a reference is added.
	 */
	igdeTabBookReference &operator=( const igdeTabBookReference &reference );
	
	/** \brief Test if tabbook is held by this holder. */
	bool operator==( igdeTabBook *tabbook ) const;
	bool operator!=( igdeTabBook *tabbook ) const;
	
	/** \brief Test if tabbook is held by this holder. */
	bool operator==( const igdeTabBookReference &reference ) const;
	bool operator!=( const igdeTabBookReference &reference ) const;
	/*@}*/
};

#endif
