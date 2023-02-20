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

#ifndef _IGDEEDITVECTORLISTENERREFERENCE_H_
#define _IGDEEDITVECTORLISTENERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeEditVectorListener;


/**
 * \brief EditVectorListener reference.
 * 
 * Safe way to hold an edit vector listener reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member edit vector listener. Do not use
 *       it as vectorer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeEditVectorListenerReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty edit vector listener reference holder. */
	igdeEditVectorListenerReference();
	
	/**
	 * \brief Create edit vector listener reference holder with edit vector listener.
	 * 
	 * Reference is added if edit vector listener is not NULL.
	 */
	igdeEditVectorListenerReference( igdeEditVectorListener *listener );
	
	/**
	 * \brief Create edit vector listener reference holder with edit vector listener from another holder.
	 * 
	 * Reference is added if edit vector listener in holder is not NULL.
	 */
	igdeEditVectorListenerReference( const igdeEditVectorListenerReference &reference );
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
	void TakeOver( igdeEditVectorListener *listener );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if edit vector listener is not NULL. */
	operator bool() const;
	
	/** \brief Vectorer to edit vector listener. */
	operator igdeEditVectorListener*() const;
	
	/**
	 * \brief Reference to edit vector listener.
	 * 
	 * \throws deeNullVectorer if edit vector listener is NULL.
	 */
	operator igdeEditVectorListener&() const;
	
	/**
	 * \brief Vectorer to edit vector listener.
	 * 
	 * \throws deeNullVectorer if edit vector listener is NULL.
	 */
	igdeEditVectorListener* operator->() const;
	
	/**
	 * \brief Store edit vector listener.
	 * 
	 * If an edit vector listener is already held its reference is release and the new edit vector listener
	 * stored. If the new edit vector listener is not NULL a reference is added.
	 */
	igdeEditVectorListenerReference &operator=( igdeEditVectorListener *listener );
	
	/**
	 * \brief Store edit vector listener.
	 * 
	 * If an edit vector listener is already held its reference is release and the new edit vector listener
	 * stored. If the new edit vector listener is not NULL a reference is added.
	 */
	igdeEditVectorListenerReference &operator=( const igdeEditVectorListenerReference &reference );
	
	/** \brief Test if edit vector listener is held by this holder. */
	bool operator==( igdeEditVectorListener *listener ) const;
	bool operator!=( igdeEditVectorListener *listener ) const;
	
	/** \brief Test if edit vector listener is held by this holder. */
	bool operator==( const igdeEditVectorListenerReference &reference ) const;
	bool operator!=( const igdeEditVectorListenerReference &reference ) const;
	/*@}*/
};

#endif
