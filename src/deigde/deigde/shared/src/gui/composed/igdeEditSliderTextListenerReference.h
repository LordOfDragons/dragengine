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

#ifndef _IGDEEDITSLIDERTEXTLISTENERREFERENCE_H_
#define _IGDEEDITSLIDERTEXTLISTENERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeEditSliderTextListener;


/**
 * \brief EditSliderTextListener reference.
 * 
 * Safe way to hold an edit point listener reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member edit point listener. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeEditSliderTextListenerReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty edit point listener reference holder. */
	igdeEditSliderTextListenerReference();
	
	/**
	 * \brief Create edit point listener reference holder with edit point listener.
	 * 
	 * Reference is added if edit point listener is not NULL.
	 */
	igdeEditSliderTextListenerReference( igdeEditSliderTextListener *listener );
	
	/**
	 * \brief Create edit point listener reference holder with edit point listener from another holder.
	 * 
	 * Reference is added if edit point listener in holder is not NULL.
	 */
	igdeEditSliderTextListenerReference( const igdeEditSliderTextListenerReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set edit point listener without adding reference.
	 * 
	 * Use this method if the edit point listener to hold has been added a reference already. This is
	 * the case with created edit point listeners as well as certain methods returning newly created
	 * edit point listeners. In all these cases the edit point listener has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em edit point listener to be a NULL edit point listener.
	 */
	void TakeOver( igdeEditSliderTextListener *listener );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if edit point listener is not NULL. */
	operator bool() const;
	
	/** \brief SliderTexter to edit point listener. */
	operator igdeEditSliderTextListener*() const;
	
	/**
	 * \brief Reference to edit point listener.
	 * 
	 * \throws deeNullSliderTexter if edit point listener is NULL.
	 */
	operator igdeEditSliderTextListener&() const;
	
	/**
	 * \brief SliderTexter to edit point listener.
	 * 
	 * \throws deeNullSliderTexter if edit point listener is NULL.
	 */
	igdeEditSliderTextListener* operator->() const;
	
	/**
	 * \brief Store edit point listener.
	 * 
	 * If an edit point listener is already held its reference is release and the new edit point listener
	 * stored. If the new edit point listener is not NULL a reference is added.
	 */
	igdeEditSliderTextListenerReference &operator=( igdeEditSliderTextListener *listener );
	
	/**
	 * \brief Store edit point listener.
	 * 
	 * If an edit point listener is already held its reference is release and the new edit point listener
	 * stored. If the new edit point listener is not NULL a reference is added.
	 */
	igdeEditSliderTextListenerReference &operator=( const igdeEditSliderTextListenerReference &reference );
	
	/** \brief Test if edit point listener is held by this holder. */
	bool operator==( igdeEditSliderTextListener *listener ) const;
	bool operator!=( igdeEditSliderTextListener *listener ) const;
	
	/** \brief Test if edit point listener is held by this holder. */
	bool operator==( const igdeEditSliderTextListenerReference &reference ) const;
	bool operator!=( const igdeEditSliderTextListenerReference &reference ) const;
	/*@}*/
};

#endif
