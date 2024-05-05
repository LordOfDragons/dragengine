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

#ifndef _IGDELISTBOXLISTENERREFERENCE_H_
#define _IGDELISTBOXLISTENERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeListBoxListener;


/**
 * \brief ListBoxListener reference.
 * 
 * Safe way to hold an listbox listener reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member listbox listener. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeListBoxListenerReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty listbox listener reference holder. */
	igdeListBoxListenerReference();
	
	/**
	 * \brief Create listbox listener reference holder with listbox listener.
	 * 
	 * Reference is added if listbox listener is not NULL.
	 */
	igdeListBoxListenerReference( igdeListBoxListener *listener );
	
	/**
	 * \brief Create listbox listener reference holder with listbox listener from another holder.
	 * 
	 * Reference is added if listbox listener in holder is not NULL.
	 */
	igdeListBoxListenerReference( const igdeListBoxListenerReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set listbox listener without adding reference.
	 * 
	 * Use this method if the listbox listener to hold has been added a reference already. This is
	 * the case with created listbox listeners as well as certain methods returning newly created
	 * listbox listeners. In all these cases the listbox listener has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em listbox listener to be a NULL listbox listener.
	 */
	void TakeOver( igdeListBoxListener *listener );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if listbox listener is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to listbox listener. */
	operator igdeListBoxListener*() const;
	
	/**
	 * \brief Reference to listbox listener.
	 * 
	 * \throws deeNullPointer if listbox listener is NULL.
	 */
	operator igdeListBoxListener&() const;
	
	/**
	 * \brief Pointer to listbox listener.
	 * 
	 * \throws deeNullPointer if listbox listener is NULL.
	 */
	igdeListBoxListener* operator->() const;
	
	/**
	 * \brief Store listbox listener.
	 * 
	 * If an listbox listener is already held its reference is release and the new listbox listener
	 * stored. If the new listbox listener is not NULL a reference is added.
	 */
	igdeListBoxListenerReference &operator=( igdeListBoxListener *listener );
	
	/**
	 * \brief Store listbox listener.
	 * 
	 * If an listbox listener is already held its reference is release and the new listbox listener
	 * stored. If the new listbox listener is not NULL a reference is added.
	 */
	igdeListBoxListenerReference &operator=( const igdeListBoxListenerReference &reference );
	
	/** \brief Test if listbox listener is held by this holder. */
	bool operator==( igdeListBoxListener *listener ) const;
	bool operator!=( igdeListBoxListener *listener ) const;
	
	/** \brief Test if listbox listener is held by this holder. */
	bool operator==( const igdeListBoxListenerReference &reference ) const;
	bool operator!=( const igdeListBoxListenerReference &reference ) const;
	/*@}*/
};

#endif
