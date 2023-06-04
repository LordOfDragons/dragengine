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
