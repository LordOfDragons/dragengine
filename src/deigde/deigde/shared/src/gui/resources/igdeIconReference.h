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

#ifndef _IGDEICONREFERENCE_H_
#define _IGDEICONREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeIcon;


/**
 * \brief Icon reference.
 * 
 * Safe way to hold an icon reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member icon. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeIconReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty icon reference holder. */
	igdeIconReference();
	
	/**
	 * \brief Create icon reference holder with icon.
	 * 
	 * Reference is added if icon is not NULL.
	 */
	igdeIconReference( igdeIcon *icon );
	
	/**
	 * \brief Create icon reference holder with icon from another holder.
	 * 
	 * Reference is added if icon in holder is not NULL.
	 */
	igdeIconReference( const igdeIconReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set icon without adding reference.
	 * 
	 * Use this method if the icon to hold has been added a reference already. This is
	 * the case with created icons as well as certain methods returning newly created
	 * icons. In all these cases the icon has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em icon to be a NULL icon.
	 */
	void TakeOver( igdeIcon *icon );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if icon is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to icon. */
	operator igdeIcon*() const;
	
	/**
	 * \brief Reference to icon.
	 * 
	 * \throws deeNullPointer if icon is NULL.
	 */
	operator igdeIcon&() const;
	
	/**
	 * \brief Pointer to icon.
	 * 
	 * \throws deeNullPointer if icon is NULL.
	 */
	igdeIcon* operator->() const;
	
	/**
	 * \brief Store icon.
	 * 
	 * If an icon is already held its reference is release and the new icon
	 * stored. If the new icon is not NULL a reference is added.
	 */
	igdeIconReference &operator=( igdeIcon *icon );
	
	/**
	 * \brief Store icon.
	 * 
	 * If an icon is already held its reference is release and the new icon
	 * stored. If the new icon is not NULL a reference is added.
	 */
	igdeIconReference &operator=( const igdeIconReference &reference );
	
	/** \brief Test if icon is held by this holder. */
	bool operator==( igdeIcon *icon ) const;
	bool operator!=( igdeIcon *icon ) const;
	
	/** \brief Test if icon is held by this holder. */
	bool operator==( const igdeIconReference &reference ) const;
	bool operator!=( const igdeIconReference &reference ) const;
	/*@}*/
};

#endif
