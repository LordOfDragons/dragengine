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

#ifndef _IGDECOMBOBOXREFERENCE_H_
#define _IGDECOMBOBOXREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeComboBox;


/**
 * \brief ComboBox reference.
 * 
 * Safe way to hold an combobox reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member combobox. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeComboBoxReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty combobox reference holder. */
	igdeComboBoxReference();
	
	/**
	 * \brief Create combobox reference holder with combobox.
	 * 
	 * Reference is added if combobox is not NULL.
	 */
	igdeComboBoxReference( igdeComboBox *combobox );
	
	/**
	 * \brief Create combobox reference holder with combobox from another holder.
	 * 
	 * Reference is added if combobox in holder is not NULL.
	 */
	igdeComboBoxReference( const igdeComboBoxReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set combobox without adding reference.
	 * 
	 * Use this method if the combobox to hold has been added a reference already. This is
	 * the case with created comboboxs as well as certain methods returning newly created
	 * comboboxs. In all these cases the combobox has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em combobox to be a NULL combobox.
	 */
	void TakeOver( igdeComboBox *combobox );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if combobox is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to combobox. */
	operator igdeComboBox*() const;
	
	/**
	 * \brief Reference to combobox.
	 * 
	 * \throws deeNullPointer if combobox is NULL.
	 */
	operator igdeComboBox&() const;
	
	/**
	 * \brief Pointer to combobox.
	 * 
	 * \throws deeNullPointer if combobox is NULL.
	 */
	igdeComboBox* operator->() const;
	
	/**
	 * \brief Store combobox.
	 * 
	 * If an combobox is already held its reference is release and the new combobox
	 * stored. If the new combobox is not NULL a reference is added.
	 */
	igdeComboBoxReference &operator=( igdeComboBox *combobox );
	
	/**
	 * \brief Store combobox.
	 * 
	 * If an combobox is already held its reference is release and the new combobox
	 * stored. If the new combobox is not NULL a reference is added.
	 */
	igdeComboBoxReference &operator=( const igdeComboBoxReference &reference );
	
	/** \brief Test if combobox is held by this holder. */
	bool operator==( igdeComboBox *combobox ) const;
	bool operator!=( igdeComboBox *combobox ) const;
	
	/** \brief Test if combobox is held by this holder. */
	bool operator==( const igdeComboBoxReference &reference ) const;
	bool operator!=( const igdeComboBoxReference &reference ) const;
	/*@}*/
};

#endif
