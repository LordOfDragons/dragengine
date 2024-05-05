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

#ifndef _IGDETEXTAREAREFERENCE_H_
#define _IGDETEXTAREAREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeTextArea;


/**
 * \brief TextArea reference.
 * 
 * Safe way to hold an textarea reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member textarea. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeTextAreaReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty textarea reference holder. */
	igdeTextAreaReference();
	
	/**
	 * \brief Create textarea reference holder with textarea.
	 * 
	 * Reference is added if textarea is not NULL.
	 */
	igdeTextAreaReference( igdeTextArea *textarea );
	
	/**
	 * \brief Create textarea reference holder with textarea from another holder.
	 * 
	 * Reference is added if textarea in holder is not NULL.
	 */
	igdeTextAreaReference( const igdeTextAreaReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set textarea without adding reference.
	 * 
	 * Use this method if the textarea to hold has been added a reference already. This is
	 * the case with created textareas as well as certain methods returning newly created
	 * textareas. In all these cases the textarea has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em textarea to be a NULL textarea.
	 */
	void TakeOver( igdeTextArea *textarea );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if textarea is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to textarea. */
	operator igdeTextArea*() const;
	
	/**
	 * \brief Reference to textarea.
	 * 
	 * \throws deeNullPointer if textarea is NULL.
	 */
	operator igdeTextArea&() const;
	
	/**
	 * \brief Pointer to textarea.
	 * 
	 * \throws deeNullPointer if textarea is NULL.
	 */
	igdeTextArea* operator->() const;
	
	/**
	 * \brief Store textarea.
	 * 
	 * If an textarea is already held its reference is release and the new textarea
	 * stored. If the new textarea is not NULL a reference is added.
	 */
	igdeTextAreaReference &operator=( igdeTextArea *textarea );
	
	/**
	 * \brief Store textarea.
	 * 
	 * If an textarea is already held its reference is release and the new textarea
	 * stored. If the new textarea is not NULL a reference is added.
	 */
	igdeTextAreaReference &operator=( const igdeTextAreaReference &reference );
	
	/** \brief Test if textarea is held by this holder. */
	bool operator==( igdeTextArea *textarea ) const;
	bool operator!=( igdeTextArea *textarea ) const;
	
	/** \brief Test if textarea is held by this holder. */
	bool operator==( const igdeTextAreaReference &reference ) const;
	bool operator!=( const igdeTextAreaReference &reference ) const;
	/*@}*/
};

#endif
