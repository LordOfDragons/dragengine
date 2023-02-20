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

#ifndef _IGDECOLORBOXRREFERENCE_H_
#define _IGDECOLORBOXRREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeColorBoxListener;


/**
 * \brief ColorBoxListener reference.
 * 
 * Safe way to hold an color box listener reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member color box listener. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeColorBoxListenerReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty color box listener reference holder. */
	igdeColorBoxListenerReference();
	
	/**
	 * \brief Create color box listener reference holder with color box listener.
	 * 
	 * Reference is added if color box listener is not NULL.
	 */
	igdeColorBoxListenerReference( igdeColorBoxListener *listener );
	
	/**
	 * \brief Create color box listener reference holder with color box listener from another holder.
	 * 
	 * Reference is added if color box listener in holder is not NULL.
	 */
	igdeColorBoxListenerReference( const igdeColorBoxListenerReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set color box listener without adding reference.
	 * 
	 * Use this method if the color box listener to hold has been added a reference already. This is
	 * the case with created color box listeners as well as certain methods returning newly created
	 * color box listeners. In all these cases the color box listener has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em color box listener to be a NULL color box listener.
	 */
	void TakeOver( igdeColorBoxListener *listener );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if color box listener is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to color box listener. */
	operator igdeColorBoxListener*() const;
	
	/**
	 * \brief Reference to color box listener.
	 * 
	 * \throws deeNullPointer if color box listener is NULL.
	 */
	operator igdeColorBoxListener&() const;
	
	/**
	 * \brief Pointer to color box listener.
	 * 
	 * \throws deeNullPointer if color box listener is NULL.
	 */
	igdeColorBoxListener* operator->() const;
	
	/**
	 * \brief Store color box listener.
	 * 
	 * If an color box listener is already held its reference is release and the new color box listener
	 * stored. If the new color box listener is not NULL a reference is added.
	 */
	igdeColorBoxListenerReference &operator=( igdeColorBoxListener *listener );
	
	/**
	 * \brief Store color box listener.
	 * 
	 * If an color box listener is already held its reference is release and the new color box listener
	 * stored. If the new color box listener is not NULL a reference is added.
	 */
	igdeColorBoxListenerReference &operator=( const igdeColorBoxListenerReference &reference );
	
	/** \brief Test if color box listener is held by this holder. */
	bool operator==( igdeColorBoxListener *listener ) const;
	bool operator!=( igdeColorBoxListener *listener ) const;
	
	/** \brief Test if color box listener is held by this holder. */
	bool operator==( const igdeColorBoxListenerReference &reference ) const;
	bool operator!=( const igdeColorBoxListenerReference &reference ) const;
	/*@}*/
};

#endif
