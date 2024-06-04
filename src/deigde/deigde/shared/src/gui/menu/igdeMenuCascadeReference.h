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

#ifndef _IGDEMENUCASCADEREFERENCE_H_
#define _IGDEMENUCASCADEREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeMenuCascade;


/**
 * \brief Menu cascade reference.
 * 
 * Safe way to hold an menu cascade reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member menu cascade. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeMenuCascadeReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty menu cascade reference holder. */
	igdeMenuCascadeReference();
	
	/**
	 * \brief Create menu cascade reference holder with menu cascade.
	 * 
	 * Reference is added if menu cascade is not NULL.
	 */
	igdeMenuCascadeReference( igdeMenuCascade *menuCascade );
	
	/**
	 * \brief Create menu cascade reference holder with menu cascade from another holder.
	 * 
	 * Reference is added if menu cascade in holder is not NULL.
	 */
	igdeMenuCascadeReference( const igdeMenuCascadeReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set menu cascade without adding reference.
	 * 
	 * Use this method if the menu cascade to hold has been added a reference already. This is
	 * the case with created menu cascades as well as certain methods returning newly created
	 * menu cascades. In all these cases the menu cascade has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em menu cascade to be a NULL menu cascade.
	 */
	void TakeOver( igdeMenuCascade *menuCascade );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if menu cascade is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to menu cascade. */
	operator igdeMenuCascade*() const;
	
	/**
	 * \brief Reference to menu cascade.
	 * 
	 * \throws deeNullPointer if menu cascade is NULL.
	 */
	operator igdeMenuCascade&() const;
	
	/**
	 * \brief Pointer to menu cascade.
	 * 
	 * \throws deeNullPointer if menu cascade is NULL.
	 */
	igdeMenuCascade* operator->() const;
	
	/**
	 * \brief Store menu cascade.
	 * 
	 * If an menu cascade is already held its reference is release and the new menu cascade
	 * stored. If the new menu cascade is not NULL a reference is added.
	 */
	igdeMenuCascadeReference &operator=( igdeMenuCascade *menuCascade );
	
	/**
	 * \brief Store menu cascade.
	 * 
	 * If an menu cascade is already held its reference is release and the new menu cascade
	 * stored. If the new menu cascade is not NULL a reference is added.
	 */
	igdeMenuCascadeReference &operator=( const igdeMenuCascadeReference &reference );
	
	/** \brief Test if menu cascade is held by this holder. */
	bool operator==( igdeMenuCascade *menuCascade ) const;
	bool operator!=( igdeMenuCascade *menuCascade ) const;
	
	/** \brief Test if menu cascade is held by this holder. */
	bool operator==( const igdeMenuCascadeReference &reference ) const;
	bool operator!=( const igdeMenuCascadeReference &reference ) const;
	/*@}*/
};

#endif
