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

#ifndef _IGDEEDITPOINTREFERENCE_H_
#define _IGDEEDITPOINTREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeEditPointListener;


/**
 * \brief EditPointListener reference.
 * 
 * Safe way to hold an edit point listener reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member edit point listener. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeEditPointListenerReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty edit point listener reference holder. */
	igdeEditPointListenerReference();
	
	/**
	 * \brief Create edit point listener reference holder with edit point listener.
	 * 
	 * Reference is added if edit point listener is not NULL.
	 */
	igdeEditPointListenerReference( igdeEditPointListener *listener );
	
	/**
	 * \brief Create edit point listener reference holder with edit point listener from another holder.
	 * 
	 * Reference is added if edit point listener in holder is not NULL.
	 */
	igdeEditPointListenerReference( const igdeEditPointListenerReference &reference );
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
	void TakeOver( igdeEditPointListener *listener );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if edit point listener is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to edit point listener. */
	operator igdeEditPointListener*() const;
	
	/**
	 * \brief Reference to edit point listener.
	 * 
	 * \throws deeNullPointer if edit point listener is NULL.
	 */
	operator igdeEditPointListener&() const;
	
	/**
	 * \brief Pointer to edit point listener.
	 * 
	 * \throws deeNullPointer if edit point listener is NULL.
	 */
	igdeEditPointListener* operator->() const;
	
	/**
	 * \brief Store edit point listener.
	 * 
	 * If an edit point listener is already held its reference is release and the new edit point listener
	 * stored. If the new edit point listener is not NULL a reference is added.
	 */
	igdeEditPointListenerReference &operator=( igdeEditPointListener *listener );
	
	/**
	 * \brief Store edit point listener.
	 * 
	 * If an edit point listener is already held its reference is release and the new edit point listener
	 * stored. If the new edit point listener is not NULL a reference is added.
	 */
	igdeEditPointListenerReference &operator=( const igdeEditPointListenerReference &reference );
	
	/** \brief Test if edit point listener is held by this holder. */
	bool operator==( igdeEditPointListener *listener ) const;
	bool operator!=( igdeEditPointListener *listener ) const;
	
	/** \brief Test if edit point listener is held by this holder. */
	bool operator==( const igdeEditPointListenerReference &reference ) const;
	bool operator!=( const igdeEditPointListenerReference &reference ) const;
	/*@}*/
};

#endif
