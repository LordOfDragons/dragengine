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

#ifndef _IGDEGUITHEMEREFERENCE_H_
#define _IGDEGUITHEMEREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeGuiTheme;


/**
 * \brief GuiTheme reference.
 * 
 * Safe way to hold an guitheme reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member guitheme. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeGuiThemeReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty guitheme reference holder. */
	igdeGuiThemeReference();
	
	/**
	 * \brief Create guitheme reference holder with guitheme.
	 * 
	 * Reference is added if guitheme is not NULL.
	 */
	igdeGuiThemeReference( igdeGuiTheme *guitheme );
	
	/**
	 * \brief Create guitheme reference holder with guitheme from another holder.
	 * 
	 * Reference is added if guitheme in holder is not NULL.
	 */
	igdeGuiThemeReference( const igdeGuiThemeReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set guitheme without adding reference.
	 * 
	 * Use this method if the guitheme to hold has been added a reference already. This is
	 * the case with created guithemes as well as certain methods returning newly created
	 * guithemes. In all these cases the guitheme has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em guitheme to be a NULL guitheme.
	 */
	void TakeOver( igdeGuiTheme *guitheme );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if guitheme is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to guitheme. */
	operator igdeGuiTheme*() const;
	
	/**
	 * \brief Reference to guitheme.
	 * 
	 * \throws deeNullPointer if guitheme is NULL.
	 */
	operator igdeGuiTheme&() const;
	
	/**
	 * \brief Pointer to guitheme.
	 * 
	 * \throws deeNullPointer if guitheme is NULL.
	 */
	igdeGuiTheme* operator->() const;
	
	/**
	 * \brief Store guitheme.
	 * 
	 * If an guitheme is already held its reference is release and the new guitheme
	 * stored. If the new guitheme is not NULL a reference is added.
	 */
	igdeGuiThemeReference &operator=( igdeGuiTheme *guitheme );
	
	/**
	 * \brief Store guitheme.
	 * 
	 * If an guitheme is already held its reference is release and the new guitheme
	 * stored. If the new guitheme is not NULL a reference is added.
	 */
	igdeGuiThemeReference &operator=( const igdeGuiThemeReference &reference );
	
	/** \brief Test if guitheme is held by this holder. */
	bool operator==( igdeGuiTheme *guitheme ) const;
	bool operator!=( igdeGuiTheme *guitheme ) const;
	
	/** \brief Test if guitheme is held by this holder. */
	bool operator==( const igdeGuiThemeReference &reference ) const;
	bool operator!=( const igdeGuiThemeReference &reference ) const;
	/*@}*/
};

#endif
