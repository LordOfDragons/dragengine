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

#ifndef _IGDESCROLLBARREFERENCE_H_
#define _IGDESCROLLBARREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeScrollBar;


/**
 * \brief ScrollBar reference.
 * 
 * Safe way to hold an scrollbar reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member scrollbar. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeScrollBarReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty scrollbar reference holder. */
	igdeScrollBarReference();
	
	/**
	 * \brief Create scrollbar reference holder with scrollbar.
	 * 
	 * Reference is added if scrollbar is not NULL.
	 */
	igdeScrollBarReference( igdeScrollBar *scrollbar );
	
	/**
	 * \brief Create scrollbar reference holder with scrollbar from another holder.
	 * 
	 * Reference is added if scrollbar in holder is not NULL.
	 */
	igdeScrollBarReference( const igdeScrollBarReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set scrollbar without adding reference.
	 * 
	 * Use this method if the scrollbar to hold has been added a reference already. This is
	 * the case with created scrollbars as well as certain methods returning newly created
	 * scrollbars. In all these cases the scrollbar has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em scrollbar to be a NULL scrollbar.
	 */
	void TakeOver( igdeScrollBar *scrollbar );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if scrollbar is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to scrollbar. */
	operator igdeScrollBar*() const;
	
	/**
	 * \brief Reference to scrollbar.
	 * 
	 * \throws deeNullPointer if scrollbar is NULL.
	 */
	operator igdeScrollBar&() const;
	
	/**
	 * \brief Pointer to scrollbar.
	 * 
	 * \throws deeNullPointer if scrollbar is NULL.
	 */
	igdeScrollBar* operator->() const;
	
	/**
	 * \brief Store scrollbar.
	 * 
	 * If an scrollbar is already held its reference is release and the new scrollbar
	 * stored. If the new scrollbar is not NULL a reference is added.
	 */
	igdeScrollBarReference &operator=( igdeScrollBar *scrollbar );
	
	/**
	 * \brief Store scrollbar.
	 * 
	 * If an scrollbar is already held its reference is release and the new scrollbar
	 * stored. If the new scrollbar is not NULL a reference is added.
	 */
	igdeScrollBarReference &operator=( const igdeScrollBarReference &reference );
	
	/** \brief Test if scrollbar is held by this holder. */
	bool operator==( igdeScrollBar *scrollbar ) const;
	bool operator!=( igdeScrollBar *scrollbar ) const;
	
	/** \brief Test if scrollbar is held by this holder. */
	bool operator==( const igdeScrollBarReference &reference ) const;
	bool operator!=( const igdeScrollBarReference &reference ) const;
	/*@}*/
};

#endif
