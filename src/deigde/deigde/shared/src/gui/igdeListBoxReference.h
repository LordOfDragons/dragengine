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

#ifndef _IGDELISTBOXREFERENCE_H_
#define _IGDELISTBOXREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeListBox;


/**
 * \brief ListBox reference.
 * 
 * Safe way to hold an listbox reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member listbox. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeListBoxReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty listbox reference holder. */
	igdeListBoxReference();
	
	/**
	 * \brief Create listbox reference holder with listbox.
	 * 
	 * Reference is added if listbox is not NULL.
	 */
	igdeListBoxReference( igdeListBox *listbox );
	
	/**
	 * \brief Create listbox reference holder with listbox from another holder.
	 * 
	 * Reference is added if listbox in holder is not NULL.
	 */
	igdeListBoxReference( const igdeListBoxReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set listbox without adding reference.
	 * 
	 * Use this method if the listbox to hold has been added a reference already. This is
	 * the case with created listboxs as well as certain methods returning newly created
	 * listboxs. In all these cases the listbox has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em listbox to be a NULL listbox.
	 */
	void TakeOver( igdeListBox *listbox );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if listbox is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to listbox. */
	operator igdeListBox*() const;
	
	/**
	 * \brief Reference to listbox.
	 * 
	 * \throws deeNullPointer if listbox is NULL.
	 */
	operator igdeListBox&() const;
	
	/**
	 * \brief Pointer to listbox.
	 * 
	 * \throws deeNullPointer if listbox is NULL.
	 */
	igdeListBox* operator->() const;
	
	/**
	 * \brief Store listbox.
	 * 
	 * If an listbox is already held its reference is release and the new listbox
	 * stored. If the new listbox is not NULL a reference is added.
	 */
	igdeListBoxReference &operator=( igdeListBox *listbox );
	
	/**
	 * \brief Store listbox.
	 * 
	 * If an listbox is already held its reference is release and the new listbox
	 * stored. If the new listbox is not NULL a reference is added.
	 */
	igdeListBoxReference &operator=( const igdeListBoxReference &reference );
	
	/** \brief Test if listbox is held by this holder. */
	bool operator==( igdeListBox *listbox ) const;
	bool operator!=( igdeListBox *listbox ) const;
	
	/** \brief Test if listbox is held by this holder. */
	bool operator==( const igdeListBoxReference &reference ) const;
	bool operator!=( const igdeListBoxReference &reference ) const;
	/*@}*/
};

#endif
