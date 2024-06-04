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

#ifndef _IGDECHECKBOXREFERENCE_H_
#define _IGDECHECKBOXREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeCheckBox;


/**
 * \brief CheckBox reference.
 * 
 * Safe way to hold an checkbox reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member checkbox. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeCheckBoxReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty checkbox reference holder. */
	igdeCheckBoxReference();
	
	/**
	 * \brief Create checkbox reference holder with checkbox.
	 * 
	 * Reference is added if checkbox is not NULL.
	 */
	igdeCheckBoxReference( igdeCheckBox *checkbox );
	
	/**
	 * \brief Create checkbox reference holder with checkbox from another holder.
	 * 
	 * Reference is added if checkbox in holder is not NULL.
	 */
	igdeCheckBoxReference( const igdeCheckBoxReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set checkbox without adding reference.
	 * 
	 * Use this method if the checkbox to hold has been added a reference already. This is
	 * the case with created checkboxs as well as certain methods returning newly created
	 * checkboxs. In all these cases the checkbox has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em checkbox to be a NULL checkbox.
	 */
	void TakeOver( igdeCheckBox *checkbox );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if checkbox is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to checkbox. */
	operator igdeCheckBox*() const;
	
	/**
	 * \brief Reference to checkbox.
	 * 
	 * \throws deeNullPointer if checkbox is NULL.
	 */
	operator igdeCheckBox&() const;
	
	/**
	 * \brief Pointer to checkbox.
	 * 
	 * \throws deeNullPointer if checkbox is NULL.
	 */
	igdeCheckBox* operator->() const;
	
	/**
	 * \brief Store checkbox.
	 * 
	 * If an checkbox is already held its reference is release and the new checkbox
	 * stored. If the new checkbox is not NULL a reference is added.
	 */
	igdeCheckBoxReference &operator=( igdeCheckBox *checkbox );
	
	/**
	 * \brief Store checkbox.
	 * 
	 * If an checkbox is already held its reference is release and the new checkbox
	 * stored. If the new checkbox is not NULL a reference is added.
	 */
	igdeCheckBoxReference &operator=( const igdeCheckBoxReference &reference );
	
	/** \brief Test if checkbox is held by this holder. */
	bool operator==( igdeCheckBox *checkbox ) const;
	bool operator!=( igdeCheckBox *checkbox ) const;
	
	/** \brief Test if checkbox is held by this holder. */
	bool operator==( const igdeCheckBoxReference &reference ) const;
	bool operator!=( const igdeCheckBoxReference &reference ) const;
	/*@}*/
};

#endif
