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
class igdeEditDVectorListenerReference : protected deObjectReference{
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
