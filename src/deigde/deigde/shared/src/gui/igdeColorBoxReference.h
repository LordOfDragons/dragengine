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

#ifndef _IGDECOLORBOXREFERENCE_H_
#define _IGDECOLORBOXREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeColorBox;


/**
 * \brief ColorBox reference.
 * 
 * Safe way to hold an colorbox reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member colorbox. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeColorBoxReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty colorbox reference holder. */
	igdeColorBoxReference();
	
	/**
	 * \brief Create colorbox reference holder with colorbox.
	 * 
	 * Reference is added if colorbox is not NULL.
	 */
	igdeColorBoxReference( igdeColorBox *colorbox );
	
	/**
	 * \brief Create colorbox reference holder with colorbox from another holder.
	 * 
	 * Reference is added if colorbox in holder is not NULL.
	 */
	igdeColorBoxReference( const igdeColorBoxReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set colorbox without adding reference.
	 * 
	 * Use this method if the colorbox to hold has been added a reference already. This is
	 * the case with created colorboxs as well as certain methods returning newly created
	 * colorboxs. In all these cases the colorbox has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em colorbox to be a NULL colorbox.
	 */
	void TakeOver( igdeColorBox *colorbox );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if colorbox is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to colorbox. */
	operator igdeColorBox*() const;
	
	/**
	 * \brief Reference to colorbox.
	 * 
	 * \throws deeNullPointer if colorbox is NULL.
	 */
	operator igdeColorBox&() const;
	
	/**
	 * \brief Pointer to colorbox.
	 * 
	 * \throws deeNullPointer if colorbox is NULL.
	 */
	igdeColorBox* operator->() const;
	
	/**
	 * \brief Store colorbox.
	 * 
	 * If an colorbox is already held its reference is release and the new colorbox
	 * stored. If the new colorbox is not NULL a reference is added.
	 */
	igdeColorBoxReference &operator=( igdeColorBox *colorbox );
	
	/**
	 * \brief Store colorbox.
	 * 
	 * If an colorbox is already held its reference is release and the new colorbox
	 * stored. If the new colorbox is not NULL a reference is added.
	 */
	igdeColorBoxReference &operator=( const igdeColorBoxReference &reference );
	
	/** \brief Test if colorbox is held by this holder. */
	bool operator==( igdeColorBox *colorbox ) const;
	bool operator!=( igdeColorBox *colorbox ) const;
	
	/** \brief Test if colorbox is held by this holder. */
	bool operator==( const igdeColorBoxReference &reference ) const;
	bool operator!=( const igdeColorBoxReference &reference ) const;
	/*@}*/
};

#endif
