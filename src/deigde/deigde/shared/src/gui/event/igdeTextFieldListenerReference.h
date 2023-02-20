/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
