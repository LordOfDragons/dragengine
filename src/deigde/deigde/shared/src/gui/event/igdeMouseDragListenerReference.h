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

#ifndef _IGDEMOUSEDRAGLISTENERREFERENCE_H_
#define _IGDEMOUSEDRAGLISTENERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeMouseDragListener;


/**
 * \brief MouseDragListener reference.
 * 
 * Safe way to hold an listener reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member listener. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeMouseDragListenerReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty listener reference holder. */
	igdeMouseDragListenerReference();
	
	/**
	 * \brief Create listener reference holder with listener.
	 * 
	 * Reference is added if listener is not NULL.
	 */
	igdeMouseDragListenerReference( igdeMouseDragListener *listener );
	
	/**
	 * \brief Create listener reference holder with listener from another holder.
	 * 
	 * Reference is added if listener in holder is not NULL.
	 */
	igdeMouseDragListenerReference( const igdeMouseDragListenerReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set listener without adding reference.
	 * 
	 * Use this method if the listener to hold has been added a reference already. This is
	 * the case with created listeners as well as certain methods returning newly created
	 * listeners. In all these cases the listener has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em listener to be a NULL listener.
	 */
	void TakeOver( igdeMouseDragListener *listener );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if listener is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to listener. */
	operator igdeMouseDragListener*() const;
	
	/**
	 * \brief Reference to listener.
	 * 
	 * \throws deeNullPointer if listener is NULL.
	 */
	operator igdeMouseDragListener&() const;
	
	/**
	 * \brief Pointer to listener.
	 * 
	 * \throws deeNullPointer if listener is NULL.
	 */
	igdeMouseDragListener* operator->() const;
	
	/**
	 * \brief Store listener.
	 * 
	 * If an listener is already held its reference is release and the new listener
	 * stored. If the new listener is not NULL a reference is added.
	 */
	igdeMouseDragListenerReference &operator=( igdeMouseDragListener *listener );
	
	/**
	 * \brief Store listener.
	 * 
	 * If an listener is already held its reference is release and the new listener
	 * stored. If the new listener is not NULL a reference is added.
	 */
	igdeMouseDragListenerReference &operator=( const igdeMouseDragListenerReference &reference );
	
	/** \brief Test if listener is held by this holder. */
	bool operator==( igdeMouseDragListener *listener ) const;
	bool operator!=( igdeMouseDragListener *listener ) const;
	
	/** \brief Test if listener is held by this holder. */
	bool operator==( const igdeMouseDragListenerReference &reference ) const;
	bool operator!=( const igdeMouseDragListenerReference &reference ) const;
	/*@}*/
};

#endif
