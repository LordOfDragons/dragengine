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

#ifndef _IGDEBUTTONREFERENCE_H_
#define _IGDEBUTTONREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeButton;


/**
 * \brief Button reference.
 * 
 * Safe way to hold an button reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member button. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeButtonReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty button reference holder. */
	igdeButtonReference();
	
	/**
	 * \brief Create button reference holder with button.
	 * 
	 * Reference is added if button is not NULL.
	 */
	igdeButtonReference( igdeButton *button );
	
	/**
	 * \brief Create button reference holder with button from another holder.
	 * 
	 * Reference is added if button in holder is not NULL.
	 */
	igdeButtonReference( const igdeButtonReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set button without adding reference.
	 * 
	 * Use this method if the button to hold has been added a reference already. This is
	 * the case with created buttons as well as certain methods returning newly created
	 * buttons. In all these cases the button has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em button to be a NULL button.
	 */
	void TakeOver( igdeButton *button );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if button is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to button. */
	operator igdeButton*() const;
	
	/**
	 * \brief Reference to button.
	 * 
	 * \throws deeNullPointer if button is NULL.
	 */
	operator igdeButton&() const;
	
	/**
	 * \brief Pointer to button.
	 * 
	 * \throws deeNullPointer if button is NULL.
	 */
	igdeButton* operator->() const;
	
	/**
	 * \brief Store button.
	 * 
	 * If an button is already held its reference is release and the new button
	 * stored. If the new button is not NULL a reference is added.
	 */
	igdeButtonReference &operator=( igdeButton *button );
	
	/**
	 * \brief Store button.
	 * 
	 * If an button is already held its reference is release and the new button
	 * stored. If the new button is not NULL a reference is added.
	 */
	igdeButtonReference &operator=( const igdeButtonReference &reference );
	
	/** \brief Test if button is held by this holder. */
	bool operator==( igdeButton *button ) const;
	bool operator!=( igdeButton *button ) const;
	
	/** \brief Test if button is held by this holder. */
	bool operator==( const igdeButtonReference &reference ) const;
	bool operator!=( const igdeButtonReference &reference ) const;
	/*@}*/
};

#endif
