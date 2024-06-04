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

#ifndef _IGDEEDITPROPERTYVALUEREFERENCE_H_
#define _IGDEEDITPROPERTYVALUEREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeEditPropertyValueListener;


/**
 * \brief EditPropertyValueListener reference.
 * 
 * Safe way to hold an edit property value listener reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member edit property value listener. Do not use
 *       it as property valueer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeEditPropertyValueListenerReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty edit property value listener reference holder. */
	igdeEditPropertyValueListenerReference();
	
	/**
	 * \brief Create edit property value listener reference holder with edit property value listener.
	 * 
	 * Reference is added if edit property value listener is not NULL.
	 */
	igdeEditPropertyValueListenerReference( igdeEditPropertyValueListener *listener );
	
	/**
	 * \brief Create edit property value listener reference holder with edit property value listener from another holder.
	 * 
	 * Reference is added if edit property value listener in holder is not NULL.
	 */
	igdeEditPropertyValueListenerReference( const igdeEditPropertyValueListenerReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set edit property value listener without adding reference.
	 * 
	 * Use this method if the edit property value listener to hold has been added a reference already. This is
	 * the case with created edit property value listeners as well as certain methods returning newly created
	 * edit property value listeners. In all these cases the edit property value listener has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em edit property value listener to be a NULL edit property value listener.
	 */
	void TakeOver( igdeEditPropertyValueListener *listener );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if edit property value listener is not NULL. */
	operator bool() const;
	
	/** \brief PropertyValueer to edit property value listener. */
	operator igdeEditPropertyValueListener*() const;
	
	/**
	 * \brief Reference to edit property value listener.
	 * 
	 * \throws deeNullPropertyValueer if edit property value listener is NULL.
	 */
	operator igdeEditPropertyValueListener&() const;
	
	/**
	 * \brief PropertyValueer to edit property value listener.
	 * 
	 * \throws deeNullPropertyValueer if edit property value listener is NULL.
	 */
	igdeEditPropertyValueListener* operator->() const;
	
	/**
	 * \brief Store edit property value listener.
	 * 
	 * If an edit property value listener is already held its reference is release and the new edit property value listener
	 * stored. If the new edit property value listener is not NULL a reference is added.
	 */
	igdeEditPropertyValueListenerReference &operator=( igdeEditPropertyValueListener *listener );
	
	/**
	 * \brief Store edit property value listener.
	 * 
	 * If an edit property value listener is already held its reference is release and the new edit property value listener
	 * stored. If the new edit property value listener is not NULL a reference is added.
	 */
	igdeEditPropertyValueListenerReference &operator=( const igdeEditPropertyValueListenerReference &reference );
	
	/** \brief Test if edit property value listener is held by this holder. */
	bool operator==( igdeEditPropertyValueListener *listener ) const;
	bool operator!=( igdeEditPropertyValueListener *listener ) const;
	
	/** \brief Test if edit property value listener is held by this holder. */
	bool operator==( const igdeEditPropertyValueListenerReference &reference ) const;
	bool operator!=( const igdeEditPropertyValueListenerReference &reference ) const;
	/*@}*/
};

#endif
