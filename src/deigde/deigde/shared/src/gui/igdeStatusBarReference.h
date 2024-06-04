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

#ifndef _IGDESTATUSBARREFERENCE_H_
#define _IGDESTATUSBARREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeStatusBar;


/**
 * \brief StatusBar reference.
 * 
 * Safe way to hold an statusBar reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member statusBar. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeStatusBarReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty statusBar reference holder. */
	igdeStatusBarReference();
	
	/**
	 * \brief Create statusBar reference holder with statusBar.
	 * 
	 * Reference is added if statusBar is not NULL.
	 */
	igdeStatusBarReference( igdeStatusBar *statusBar );
	
	/**
	 * \brief Create statusBar reference holder with statusBar from another holder.
	 * 
	 * Reference is added if statusBar in holder is not NULL.
	 */
	igdeStatusBarReference( const igdeStatusBarReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set statusBar without adding reference.
	 * 
	 * Use this method if the statusBar to hold has been added a reference already. This is
	 * the case with created statusBars as well as certain methods returning newly created
	 * statusBars. In all these cases the statusBar has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em statusBar to be a NULL statusBar.
	 */
	void TakeOver( igdeStatusBar *statusBar );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if statusBar is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to statusBar. */
	operator igdeStatusBar*() const;
	
	/**
	 * \brief Reference to statusBar.
	 * 
	 * \throws deeNullPointer if statusBar is NULL.
	 */
	operator igdeStatusBar&() const;
	
	/**
	 * \brief Pointer to statusBar.
	 * 
	 * \throws deeNullPointer if statusBar is NULL.
	 */
	igdeStatusBar* operator->() const;
	
	/**
	 * \brief Store statusBar.
	 * 
	 * If an statusBar is already held its reference is release and the new statusBar
	 * stored. If the new statusBar is not NULL a reference is added.
	 */
	igdeStatusBarReference &operator=( igdeStatusBar *statusBar );
	
	/**
	 * \brief Store statusBar.
	 * 
	 * If an statusBar is already held its reference is release and the new statusBar
	 * stored. If the new statusBar is not NULL a reference is added.
	 */
	igdeStatusBarReference &operator=( const igdeStatusBarReference &reference );
	
	/** \brief Test if statusBar is held by this holder. */
	bool operator==( igdeStatusBar *statusBar ) const;
	bool operator!=( igdeStatusBar *statusBar ) const;
	
	/** \brief Test if statusBar is held by this holder. */
	bool operator==( const igdeStatusBarReference &reference ) const;
	bool operator!=( const igdeStatusBarReference &reference ) const;
	/*@}*/
};

#endif
