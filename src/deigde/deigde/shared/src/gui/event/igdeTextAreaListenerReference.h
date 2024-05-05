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

#ifndef _IGDETEXTAREARREFERENCE_H_
#define _IGDETEXTAREARREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeTextAreaListener;


/**
 * \brief TextAreaListener reference.
 * 
 * Safe way to hold an textarea listener reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member textarea listener. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeTextAreaListenerReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty textarea listener reference holder. */
	igdeTextAreaListenerReference();
	
	/**
	 * \brief Create textarea listener reference holder with textarea listener.
	 * 
	 * Reference is added if textarea listener is not NULL.
	 */
	igdeTextAreaListenerReference( igdeTextAreaListener *listener );
	
	/**
	 * \brief Create textarea listener reference holder with textarea listener from another holder.
	 * 
	 * Reference is added if textarea listener in holder is not NULL.
	 */
	igdeTextAreaListenerReference( const igdeTextAreaListenerReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set textarea listener without adding reference.
	 * 
	 * Use this method if the textarea listener to hold has been added a reference already. This is
	 * the case with created textarea listeners as well as certain methods returning newly created
	 * textarea listeners. In all these cases the textarea listener has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em textarea listener to be a NULL textarea listener.
	 */
	void TakeOver( igdeTextAreaListener *listener );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if textarea listener is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to textarea listener. */
	operator igdeTextAreaListener*() const;
	
	/**
	 * \brief Reference to textarea listener.
	 * 
	 * \throws deeNullPointer if textarea listener is NULL.
	 */
	operator igdeTextAreaListener&() const;
	
	/**
	 * \brief Pointer to textarea listener.
	 * 
	 * \throws deeNullPointer if textarea listener is NULL.
	 */
	igdeTextAreaListener* operator->() const;
	
	/**
	 * \brief Store textarea listener.
	 * 
	 * If an textarea listener is already held its reference is release and the new textarea listener
	 * stored. If the new textarea listener is not NULL a reference is added.
	 */
	igdeTextAreaListenerReference &operator=( igdeTextAreaListener *listener );
	
	/**
	 * \brief Store textarea listener.
	 * 
	 * If an textarea listener is already held its reference is release and the new textarea listener
	 * stored. If the new textarea listener is not NULL a reference is added.
	 */
	igdeTextAreaListenerReference &operator=( const igdeTextAreaListenerReference &reference );
	
	/** \brief Test if textarea listener is held by this holder. */
	bool operator==( igdeTextAreaListener *listener ) const;
	bool operator!=( igdeTextAreaListener *listener ) const;
	
	/** \brief Test if textarea listener is held by this holder. */
	bool operator==( const igdeTextAreaListenerReference &reference ) const;
	bool operator!=( const igdeTextAreaListenerReference &reference ) const;
	/*@}*/
};

#endif
