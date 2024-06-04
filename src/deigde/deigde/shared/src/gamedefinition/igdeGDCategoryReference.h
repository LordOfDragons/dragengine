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

#ifndef _IGDEGDCATEGORYREFERENCE_H_
#define _IGDEGDCATEGORYREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeGDCategory;


/**
 * \brief GDCategory reference.
 * 
 * Safe way to hold an clipboard data reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member clipboard data. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeGDCategoryReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty clipboard data reference holder. */
	igdeGDCategoryReference();
	
	/**
	 * \brief Create clipboard data reference holder with clipboard data.
	 * 
	 * Reference is added if clipboard data is not NULL.
	 */
	igdeGDCategoryReference( igdeGDCategory *clipboardData );
	
	/**
	 * \brief Create clipboard data reference holder with clipboard data from another holder.
	 * 
	 * Reference is added if clipboard data in holder is not NULL.
	 */
	igdeGDCategoryReference( const igdeGDCategoryReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set clipboard data without adding reference.
	 * 
	 * Use this method if the clipboard data to hold has been added a reference already. This is
	 * the case with created clipboard datas as well as certain methods returning newly created
	 * clipboard datas. In all these cases the clipboard data has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em clipboard data to be a NULL clipboard data.
	 */
	void TakeOver( igdeGDCategory *clipboardData );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if clipboard data is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to clipboard data. */
	operator igdeGDCategory*() const;
	
	/**
	 * \brief Reference to clipboard data.
	 * 
	 * \throws deeNullPointer if clipboard data is NULL.
	 */
	operator igdeGDCategory&() const;
	
	/**
	 * \brief Pointer to clipboard data.
	 * 
	 * \throws deeNullPointer if clipboard data is NULL.
	 */
	igdeGDCategory* operator->() const;
	
	/**
	 * \brief Store clipboard data.
	 * 
	 * If an clipboard data is already held its reference is release and the new clipboard data
	 * stored. If the new clipboard data is not NULL a reference is added.
	 */
	igdeGDCategoryReference &operator=( igdeGDCategory *clipboardData );
	
	/**
	 * \brief Store clipboard data.
	 * 
	 * If an clipboard data is already held its reference is release and the new clipboard data
	 * stored. If the new clipboard data is not NULL a reference is added.
	 */
	igdeGDCategoryReference &operator=( const igdeGDCategoryReference &reference );
	
	/** \brief Test if clipboard data is held by this holder. */
	bool operator==( igdeGDCategory *clipboardData ) const;
	bool operator!=( igdeGDCategory *clipboardData ) const;
	
	/** \brief Test if clipboard data is held by this holder. */
	bool operator==( const igdeGDCategoryReference &reference ) const;
	bool operator!=( const igdeGDCategoryReference &reference ) const;
	/*@}*/
};

#endif
