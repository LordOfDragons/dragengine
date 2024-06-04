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

#ifndef _IGDEMENUBARREFERENCE_H_
#define _IGDEMENUBARREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeMenuBar;


/**
 * \brief MenuBar reference.
 * 
 * Safe way to hold a menu bar reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member menu bar. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeMenuBarReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty menu bar reference holder. */
	igdeMenuBarReference();
	
	/**
	 * \brief Create menu bar reference holder with menu bar.
	 * 
	 * Reference is added if menu bar is not NULL.
	 */
	igdeMenuBarReference( igdeMenuBar *menubar );
	
	/**
	 * \brief Create menu bar reference holder with menu bar from another holder.
	 * 
	 * Reference is added if menu bar in holder is not NULL.
	 */
	igdeMenuBarReference( const igdeMenuBarReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set menu bar without adding reference.
	 * 
	 * Use this method if the menu bar to hold has been added a reference already. This is
	 * the case with created menu bars as well as certain methods returning newly created
	 * menu bars. In all these cases the menu bar has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em menu bar to be a NULL menu bar.
	 */
	void TakeOver( igdeMenuBar *menubar );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if menu bar is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to menu bar. */
	operator igdeMenuBar*() const;
	
	/**
	 * \brief Reference to menu bar.
	 * 
	 * \throws deeNullPointer if menu bar is NULL.
	 */
	operator igdeMenuBar&() const;
	
	/**
	 * \brief Pointer to menu bar.
	 * 
	 * \throws deeNullPointer if menu bar is NULL.
	 */
	igdeMenuBar* operator->() const;
	
	/**
	 * \brief Store menu bar.
	 * 
	 * If an menu bar is already held its reference is release and the new menu bar
	 * stored. If the new menu bar is not NULL a reference is added.
	 */
	igdeMenuBarReference &operator=( igdeMenuBar *menubar );
	
	/**
	 * \brief Store menu bar.
	 * 
	 * If an menu bar is already held its reference is release and the new menu bar
	 * stored. If the new menu bar is not NULL a reference is added.
	 */
	igdeMenuBarReference &operator=( const igdeMenuBarReference &reference );
	
	/** \brief Test if menu bar is held by this holder. */
	bool operator==( igdeMenuBar *menubar ) const;
	bool operator!=( igdeMenuBar *menubar ) const;
	
	/** \brief Test if menu bar is held by this holder. */
	bool operator==( const igdeMenuBarReference &reference ) const;
	bool operator!=( const igdeMenuBarReference &reference ) const;
	/*@}*/
};

#endif
