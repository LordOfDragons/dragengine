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

#ifndef _IGDESCROLLBARLISTENERREFERENCE_H_
#define _IGDESCROLLBARLISTENERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeScrollBarListener;


/**
 * \brief ScrollBarListener reference.
 * 
 * Safe way to hold an scroll bar listener reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member scroll bar listener. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeScrollBarListenerReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty scroll bar listener reference holder. */
	igdeScrollBarListenerReference();
	
	/**
	 * \brief Create scroll bar listener reference holder with scroll bar listener.
	 * 
	 * Reference is added if scroll bar listener is not NULL.
	 */
	igdeScrollBarListenerReference( igdeScrollBarListener *listener );
	
	/**
	 * \brief Create scroll bar listener reference holder with scroll bar listener from another holder.
	 * 
	 * Reference is added if scroll bar listener in holder is not NULL.
	 */
	igdeScrollBarListenerReference( const igdeScrollBarListenerReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set scroll bar listener without adding reference.
	 * 
	 * Use this method if the scroll bar listener to hold has been added a reference already. This is
	 * the case with created scroll bar listeners as well as certain methods returning newly created
	 * scroll bar listeners. In all these cases the scroll bar listener has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em scroll bar listener to be a NULL scroll bar listener.
	 */
	void TakeOver( igdeScrollBarListener *listener );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if scroll bar listener is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to scroll bar listener. */
	operator igdeScrollBarListener*() const;
	
	/**
	 * \brief Reference to scroll bar listener.
	 * 
	 * \throws deeNullPointer if scroll bar listener is NULL.
	 */
	operator igdeScrollBarListener&() const;
	
	/**
	 * \brief Pointer to scroll bar listener.
	 * 
	 * \throws deeNullPointer if scroll bar listener is NULL.
	 */
	igdeScrollBarListener* operator->() const;
	
	/**
	 * \brief Store scroll bar listener.
	 * 
	 * If an scroll bar listener is already held its reference is release and the new scroll bar listener
	 * stored. If the new scroll bar listener is not NULL a reference is added.
	 */
	igdeScrollBarListenerReference &operator=( igdeScrollBarListener *listener );
	
	/**
	 * \brief Store scroll bar listener.
	 * 
	 * If an scroll bar listener is already held its reference is release and the new scroll bar listener
	 * stored. If the new scroll bar listener is not NULL a reference is added.
	 */
	igdeScrollBarListenerReference &operator=( const igdeScrollBarListenerReference &reference );
	
	/** \brief Test if scroll bar listener is held by this holder. */
	bool operator==( igdeScrollBarListener *listener ) const;
	bool operator!=( igdeScrollBarListener *listener ) const;
	
	/** \brief Test if scroll bar listener is held by this holder. */
	bool operator==( const igdeScrollBarListenerReference &reference ) const;
	bool operator!=( const igdeScrollBarListenerReference &reference ) const;
	/*@}*/
};

#endif
