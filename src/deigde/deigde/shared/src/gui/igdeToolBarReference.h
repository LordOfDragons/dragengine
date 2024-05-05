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

#ifndef _IGDETOOLBARREFERENCE_H_
#define _IGDETOOLBARREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeToolBar;


/**
 * \brief ToolBar reference.
 * 
 * Safe way to hold an toolbar reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member toolbar. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeToolBarReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty toolbar reference holder. */
	igdeToolBarReference();
	
	/**
	 * \brief Create toolbar reference holder with toolbar.
	 * 
	 * Reference is added if toolbar is not NULL.
	 */
	igdeToolBarReference( igdeToolBar *toolbar );
	
	/**
	 * \brief Create toolbar reference holder with toolbar from another holder.
	 * 
	 * Reference is added if toolbar in holder is not NULL.
	 */
	igdeToolBarReference( const igdeToolBarReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set toolbar without adding reference.
	 * 
	 * Use this method if the toolbar to hold has been added a reference already. This is
	 * the case with created toolbars as well as certain methods returning newly created
	 * toolbars. In all these cases the toolbar has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em toolbar to be a NULL toolbar.
	 */
	void TakeOver( igdeToolBar *toolbar );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if toolbar is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to toolbar. */
	operator igdeToolBar*() const;
	
	/**
	 * \brief Reference to toolbar.
	 * 
	 * \throws deeNullPointer if toolbar is NULL.
	 */
	operator igdeToolBar&() const;
	
	/**
	 * \brief Pointer to toolbar.
	 * 
	 * \throws deeNullPointer if toolbar is NULL.
	 */
	igdeToolBar* operator->() const;
	
	/**
	 * \brief Store toolbar.
	 * 
	 * If an toolbar is already held its reference is release and the new toolbar
	 * stored. If the new toolbar is not NULL a reference is added.
	 */
	igdeToolBarReference &operator=( igdeToolBar *toolbar );
	
	/**
	 * \brief Store toolbar.
	 * 
	 * If an toolbar is already held its reference is release and the new toolbar
	 * stored. If the new toolbar is not NULL a reference is added.
	 */
	igdeToolBarReference &operator=( const igdeToolBarReference &reference );
	
	/** \brief Test if toolbar is held by this holder. */
	bool operator==( igdeToolBar *toolbar ) const;
	bool operator!=( igdeToolBar *toolbar ) const;
	
	/** \brief Test if toolbar is held by this holder. */
	bool operator==( const igdeToolBarReference &reference ) const;
	bool operator!=( const igdeToolBarReference &reference ) const;
	/*@}*/
};

#endif
