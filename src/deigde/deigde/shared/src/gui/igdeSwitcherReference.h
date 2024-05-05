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

#ifndef _IGDESWITCHERREFERENCE_H_
#define _IGDESWITCHERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeSwitcher;


/**
 * \brief Switcher reference.
 * 
 * Safe way to hold an switcher reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member switcher. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeSwitcherReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty switcher reference holder. */
	igdeSwitcherReference();
	
	/**
	 * \brief Create switcher reference holder with switcher.
	 * 
	 * Reference is added if switcher is not NULL.
	 */
	igdeSwitcherReference( igdeSwitcher *switcher );
	
	/**
	 * \brief Create switcher reference holder with switcher from another holder.
	 * 
	 * Reference is added if switcher in holder is not NULL.
	 */
	igdeSwitcherReference( const igdeSwitcherReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set switcher without adding reference.
	 * 
	 * Use this method if the switcher to hold has been added a reference already. This is
	 * the case with created switchers as well as certain methods returning newly created
	 * switchers. In all these cases the switcher has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em switcher to be a NULL switcher.
	 */
	void TakeOver( igdeSwitcher *switcher );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if switcher is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to switcher. */
	operator igdeSwitcher*() const;
	
	/**
	 * \brief Reference to switcher.
	 * 
	 * \throws deeNullPointer if switcher is NULL.
	 */
	operator igdeSwitcher&() const;
	
	/**
	 * \brief Pointer to switcher.
	 * 
	 * \throws deeNullPointer if switcher is NULL.
	 */
	igdeSwitcher* operator->() const;
	
	/**
	 * \brief Store switcher.
	 * 
	 * If an switcher is already held its reference is release and the new switcher
	 * stored. If the new switcher is not NULL a reference is added.
	 */
	igdeSwitcherReference &operator=( igdeSwitcher *switcher );
	
	/**
	 * \brief Store switcher.
	 * 
	 * If an switcher is already held its reference is release and the new switcher
	 * stored. If the new switcher is not NULL a reference is added.
	 */
	igdeSwitcherReference &operator=( const igdeSwitcherReference &reference );
	
	/** \brief Test if switcher is held by this holder. */
	bool operator==( igdeSwitcher *switcher ) const;
	bool operator!=( igdeSwitcher *switcher ) const;
	
	/** \brief Test if switcher is held by this holder. */
	bool operator==( const igdeSwitcherReference &reference ) const;
	bool operator!=( const igdeSwitcherReference &reference ) const;
	/*@}*/
};

#endif
