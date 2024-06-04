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

#ifndef _IGDEFONTREFERENCE_H_
#define _IGDEFONTREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeFont;


/**
 * \brief Font reference.
 * 
 * Safe way to hold an font reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member font. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeFontReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty font reference holder. */
	igdeFontReference();
	
	/**
	 * \brief Create font reference holder with font.
	 * 
	 * Reference is added if font is not NULL.
	 */
	igdeFontReference( igdeFont *font );
	
	/**
	 * \brief Create font reference holder with font from another holder.
	 * 
	 * Reference is added if font in holder is not NULL.
	 */
	igdeFontReference( const igdeFontReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set font without adding reference.
	 * 
	 * Use this method if the font to hold has been added a reference already. This is
	 * the case with created fonts as well as certain methods returning newly created
	 * fonts. In all these cases the font has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em font to be a NULL font.
	 */
	void TakeOver( igdeFont *font );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if font is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to font. */
	operator igdeFont*() const;
	
	/**
	 * \brief Reference to font.
	 * 
	 * \throws deeNullPointer if font is NULL.
	 */
	operator igdeFont&() const;
	
	/**
	 * \brief Pointer to font.
	 * 
	 * \throws deeNullPointer if font is NULL.
	 */
	igdeFont* operator->() const;
	
	/**
	 * \brief Store font.
	 * 
	 * If an font is already held its reference is release and the new font
	 * stored. If the new font is not NULL a reference is added.
	 */
	igdeFontReference &operator=( igdeFont *font );
	
	/**
	 * \brief Store font.
	 * 
	 * If an font is already held its reference is release and the new font
	 * stored. If the new font is not NULL a reference is added.
	 */
	igdeFontReference &operator=( const igdeFontReference &reference );
	
	/** \brief Test if font is held by this holder. */
	bool operator==( igdeFont *font ) const;
	bool operator!=( igdeFont *font ) const;
	
	/** \brief Test if font is held by this holder. */
	bool operator==( const igdeFontReference &reference ) const;
	bool operator!=( const igdeFontReference &reference ) const;
	/*@}*/
};

#endif
