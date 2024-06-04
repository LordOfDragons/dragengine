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

#ifndef _IGDETEXTSTYLEREFERENCE_H_
#define _IGDETEXTSTYLEREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeTextStyle;


/**
 * \brief TextStyle reference.
 * 
 * Safe way to hold an text style reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member text style. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeTextStyleReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty text style reference holder. */
	igdeTextStyleReference();
	
	/**
	 * \brief Create text style reference holder with text style.
	 * 
	 * Reference is added if text style is not NULL.
	 */
	igdeTextStyleReference( igdeTextStyle *textStyle );
	
	/**
	 * \brief Create text style reference holder with text style from another holder.
	 * 
	 * Reference is added if text style in holder is not NULL.
	 */
	igdeTextStyleReference( const igdeTextStyleReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set text style without adding reference.
	 * 
	 * Use this method if the text style to hold has been added a reference already. This is
	 * the case with created text styles as well as certain methods returning newly created
	 * text styles. In all these cases the text style has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em text style to be a NULL text style.
	 */
	void TakeOver( igdeTextStyle *textStyle );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if text style is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to text style. */
	operator igdeTextStyle*() const;
	
	/**
	 * \brief Reference to text style.
	 * 
	 * \throws deeNullPointer if text style is NULL.
	 */
	operator igdeTextStyle&() const;
	
	/**
	 * \brief Pointer to text style.
	 * 
	 * \throws deeNullPointer if text style is NULL.
	 */
	igdeTextStyle* operator->() const;
	
	/**
	 * \brief Store text style.
	 * 
	 * If an text style is already held its reference is release and the new text style
	 * stored. If the new text style is not NULL a reference is added.
	 */
	igdeTextStyleReference &operator=( igdeTextStyle *textStyle );
	
	/**
	 * \brief Store text style.
	 * 
	 * If an text style is already held its reference is release and the new text style
	 * stored. If the new text style is not NULL a reference is added.
	 */
	igdeTextStyleReference &operator=( const igdeTextStyleReference &reference );
	
	/** \brief Test if text style is held by this holder. */
	bool operator==( igdeTextStyle *textStyle ) const;
	bool operator!=( igdeTextStyle *textStyle ) const;
	
	/** \brief Test if text style is held by this holder. */
	bool operator==( const igdeTextStyleReference &reference ) const;
	bool operator!=( const igdeTextStyleReference &reference ) const;
	/*@}*/
};

#endif
