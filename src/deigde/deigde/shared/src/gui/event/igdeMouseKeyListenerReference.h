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

#ifndef _IGDEMOUSEKEYLISTENERREFERENCE_H_
#define _IGDEMOUSEKEYLISTENERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeMouseKeyListener;


/**
 * \brief MouseKeyListener reference.
 * 
 * Safe way to hold an listener reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member listener. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeMouseKeyListenerReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty listener reference holder. */
	igdeMouseKeyListenerReference();
	
	/**
	 * \brief Create listener reference holder with listener.
	 * 
	 * Reference is added if listener is not NULL.
	 */
	igdeMouseKeyListenerReference( igdeMouseKeyListener *listener );
	
	/**
	 * \brief Create listener reference holder with listener from another holder.
	 * 
	 * Reference is added if listener in holder is not NULL.
	 */
	igdeMouseKeyListenerReference( const igdeMouseKeyListenerReference &reference );
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
	void TakeOver( igdeMouseKeyListener *listener );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if listener is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to listener. */
	operator igdeMouseKeyListener*() const;
	
	/**
	 * \brief Reference to listener.
	 * 
	 * \throws deeNullPointer if listener is NULL.
	 */
	operator igdeMouseKeyListener&() const;
	
	/**
	 * \brief Pointer to listener.
	 * 
	 * \throws deeNullPointer if listener is NULL.
	 */
	igdeMouseKeyListener* operator->() const;
	
	/**
	 * \brief Store listener.
	 * 
	 * If an listener is already held its reference is release and the new listener
	 * stored. If the new listener is not NULL a reference is added.
	 */
	igdeMouseKeyListenerReference &operator=( igdeMouseKeyListener *listener );
	
	/**
	 * \brief Store listener.
	 * 
	 * If an listener is already held its reference is release and the new listener
	 * stored. If the new listener is not NULL a reference is added.
	 */
	igdeMouseKeyListenerReference &operator=( const igdeMouseKeyListenerReference &reference );
	
	/** \brief Test if listener is held by this holder. */
	bool operator==( igdeMouseKeyListener *listener ) const;
	bool operator!=( igdeMouseKeyListener *listener ) const;
	
	/** \brief Test if listener is held by this holder. */
	bool operator==( const igdeMouseKeyListenerReference &reference ) const;
	bool operator!=( const igdeMouseKeyListenerReference &reference ) const;
	/*@}*/
};

#endif
