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

#ifndef _IGDEEDITDVECTORLISTENERREFERENCE_H_
#define _IGDEEDITDVECTORLISTENERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeEditDVectorListener;


/**
 * \brief EditDVectorListener reference.
 * 
 * Safe way to hold an edit vector listener reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member edit vector listener. Do not use
 *       it as vectorer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeEditDVectorListenerReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty edit vector listener reference holder. */
	igdeEditDVectorListenerReference();
	
	/**
	 * \brief Create edit vector listener reference holder with edit vector listener.
	 * 
	 * Reference is added if edit vector listener is not NULL.
	 */
	igdeEditDVectorListenerReference( igdeEditDVectorListener *listener );
	
	/**
	 * \brief Create edit vector listener reference holder with edit vector listener from another holder.
	 * 
	 * Reference is added if edit vector listener in holder is not NULL.
	 */
	igdeEditDVectorListenerReference( const igdeEditDVectorListenerReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set edit vector listener without adding reference.
	 * 
	 * Use this method if the edit vector listener to hold has been added a reference already. This is
	 * the case with created edit vector listeners as well as certain methods returning newly created
	 * edit vector listeners. In all these cases the edit vector listener has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em edit vector listener to be a NULL edit vector listener.
	 */
	void TakeOver( igdeEditDVectorListener *listener );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if edit vector listener is not NULL. */
	operator bool() const;
	
	/** \brief DVectorer to edit vector listener. */
	operator igdeEditDVectorListener*() const;
	
	/**
	 * \brief Reference to edit vector listener.
	 * 
	 * \throws deeNullDVectorer if edit vector listener is NULL.
	 */
	operator igdeEditDVectorListener&() const;
	
	/**
	 * \brief DVectorer to edit vector listener.
	 * 
	 * \throws deeNullDVectorer if edit vector listener is NULL.
	 */
	igdeEditDVectorListener* operator->() const;
	
	/**
	 * \brief Store edit vector listener.
	 * 
	 * If an edit vector listener is already held its reference is release and the new edit vector listener
	 * stored. If the new edit vector listener is not NULL a reference is added.
	 */
	igdeEditDVectorListenerReference &operator=( igdeEditDVectorListener *listener );
	
	/**
	 * \brief Store edit vector listener.
	 * 
	 * If an edit vector listener is already held its reference is release and the new edit vector listener
	 * stored. If the new edit vector listener is not NULL a reference is added.
	 */
	igdeEditDVectorListenerReference &operator=( const igdeEditDVectorListenerReference &reference );
	
	/** \brief Test if edit vector listener is held by this holder. */
	bool operator==( igdeEditDVectorListener *listener ) const;
	bool operator!=( igdeEditDVectorListener *listener ) const;
	
	/** \brief Test if edit vector listener is held by this holder. */
	bool operator==( const igdeEditDVectorListenerReference &reference ) const;
	bool operator!=( const igdeEditDVectorListenerReference &reference ) const;
	/*@}*/
};

#endif
