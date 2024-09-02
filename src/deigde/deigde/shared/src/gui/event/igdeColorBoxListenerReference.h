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

#ifndef _IGDECOLORBOXRREFERENCE_H_
#define _IGDECOLORBOXRREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeColorBoxListener;


/**
 * \brief ColorBoxListener reference.
 * 
 * Safe way to hold an color box listener reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member color box listener. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeColorBoxListenerReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty color box listener reference holder. */
	igdeColorBoxListenerReference();
	
	/**
	 * \brief Create color box listener reference holder with color box listener.
	 * 
	 * Reference is added if color box listener is not NULL.
	 */
	igdeColorBoxListenerReference( igdeColorBoxListener *listener );
	
	/**
	 * \brief Create color box listener reference holder with color box listener from another holder.
	 * 
	 * Reference is added if color box listener in holder is not NULL.
	 */
	igdeColorBoxListenerReference( const igdeColorBoxListenerReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set color box listener without adding reference.
	 * 
	 * Use this method if the color box listener to hold has been added a reference already. This is
	 * the case with created color box listeners as well as certain methods returning newly created
	 * color box listeners. In all these cases the color box listener has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em color box listener to be a NULL color box listener.
	 */
	void TakeOver( igdeColorBoxListener *listener );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if color box listener is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to color box listener. */
	operator igdeColorBoxListener*() const;
	
	/**
	 * \brief Reference to color box listener.
	 * 
	 * \throws deeNullPointer if color box listener is NULL.
	 */
	operator igdeColorBoxListener&() const;
	
	/**
	 * \brief Pointer to color box listener.
	 * 
	 * \throws deeNullPointer if color box listener is NULL.
	 */
	igdeColorBoxListener* operator->() const;
	
	/**
	 * \brief Store color box listener.
	 * 
	 * If an color box listener is already held its reference is release and the new color box listener
	 * stored. If the new color box listener is not NULL a reference is added.
	 */
	igdeColorBoxListenerReference &operator=( igdeColorBoxListener *listener );
	
	/**
	 * \brief Store color box listener.
	 * 
	 * If an color box listener is already held its reference is release and the new color box listener
	 * stored. If the new color box listener is not NULL a reference is added.
	 */
	igdeColorBoxListenerReference &operator=( const igdeColorBoxListenerReference &reference );
	
	/** \brief Test if color box listener is held by this holder. */
	bool operator==( igdeColorBoxListener *listener ) const;
	bool operator!=( igdeColorBoxListener *listener ) const;
	
	/** \brief Test if color box listener is held by this holder. */
	bool operator==( const igdeColorBoxListenerReference &reference ) const;
	bool operator!=( const igdeColorBoxListenerReference &reference ) const;
	/*@}*/
};

#endif
