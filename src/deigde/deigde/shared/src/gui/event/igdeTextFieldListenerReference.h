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

#ifndef _IGDETEXTFIELDRREFERENCE_H_
#define _IGDETEXTFIELDRREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeTextFieldListener;


/**
 * \brief TextFieldListener reference.
 * 
 * Safe way to hold an textfield listener reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member textfield listener. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeTextFieldListenerReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty textfield listener reference holder. */
	igdeTextFieldListenerReference();
	
	/**
	 * \brief Create textfield listener reference holder with textfield listener.
	 * 
	 * Reference is added if textfield listener is not NULL.
	 */
	igdeTextFieldListenerReference( igdeTextFieldListener *listener );
	
	/**
	 * \brief Create textfield listener reference holder with textfield listener from another holder.
	 * 
	 * Reference is added if textfield listener in holder is not NULL.
	 */
	igdeTextFieldListenerReference( const igdeTextFieldListenerReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set textfield listener without adding reference.
	 * 
	 * Use this method if the textfield listener to hold has been added a reference already. This is
	 * the case with created textfield listeners as well as certain methods returning newly created
	 * textfield listeners. In all these cases the textfield listener has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em textfield listener to be a NULL textfield listener.
	 */
	void TakeOver( igdeTextFieldListener *listener );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if textfield listener is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to textfield listener. */
	operator igdeTextFieldListener*() const;
	
	/**
	 * \brief Reference to textfield listener.
	 * 
	 * \throws deeNullPointer if textfield listener is NULL.
	 */
	operator igdeTextFieldListener&() const;
	
	/**
	 * \brief Pointer to textfield listener.
	 * 
	 * \throws deeNullPointer if textfield listener is NULL.
	 */
	igdeTextFieldListener* operator->() const;
	
	/**
	 * \brief Store textfield listener.
	 * 
	 * If an textfield listener is already held its reference is release and the new textfield listener
	 * stored. If the new textfield listener is not NULL a reference is added.
	 */
	igdeTextFieldListenerReference &operator=( igdeTextFieldListener *listener );
	
	/**
	 * \brief Store textfield listener.
	 * 
	 * If an textfield listener is already held its reference is release and the new textfield listener
	 * stored. If the new textfield listener is not NULL a reference is added.
	 */
	igdeTextFieldListenerReference &operator=( const igdeTextFieldListenerReference &reference );
	
	/** \brief Test if textfield listener is held by this holder. */
	bool operator==( igdeTextFieldListener *listener ) const;
	bool operator!=( igdeTextFieldListener *listener ) const;
	
	/** \brief Test if textfield listener is held by this holder. */
	bool operator==( const igdeTextFieldListenerReference &reference ) const;
	bool operator!=( const igdeTextFieldListenerReference &reference ) const;
	/*@}*/
};

#endif
