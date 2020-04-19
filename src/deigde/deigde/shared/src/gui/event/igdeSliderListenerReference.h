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

#ifndef _IGDESLIDERLISTENERREFERENCE_H_
#define _IGDESLIDERLISTENERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeSliderListener;


/**
 * \brief SliderListener reference.
 * 
 * Safe way to hold an scroll bar listener reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member scroll bar listener. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeSliderListenerReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty scroll bar listener reference holder. */
	igdeSliderListenerReference();
	
	/**
	 * \brief Create scroll bar listener reference holder with scroll bar listener.
	 * 
	 * Reference is added if scroll bar listener is not NULL.
	 */
	igdeSliderListenerReference( igdeSliderListener *listener );
	
	/**
	 * \brief Create scroll bar listener reference holder with scroll bar listener from another holder.
	 * 
	 * Reference is added if scroll bar listener in holder is not NULL.
	 */
	igdeSliderListenerReference( const igdeSliderListenerReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set scroll bar listener without adding reference.
	 * 
	 * Use this method if the scroll bar listener to hold has been added a reference already. This is
	 * the case with created scroll bar listeners as well as certain methods returning newly created
	 * scroll bar listeners. In all these cases the scroll bar listener has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em scroll bar listener to be a NULL scroll bar listener.
	 */
	void TakeOver( igdeSliderListener *listener );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if scroll bar listener is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to scroll bar listener. */
	operator igdeSliderListener*() const;
	
	/**
	 * \brief Reference to scroll bar listener.
	 * 
	 * \throws deeNullPointer if scroll bar listener is NULL.
	 */
	operator igdeSliderListener&() const;
	
	/**
	 * \brief Pointer to scroll bar listener.
	 * 
	 * \throws deeNullPointer if scroll bar listener is NULL.
	 */
	igdeSliderListener* operator->() const;
	
	/**
	 * \brief Store scroll bar listener.
	 * 
	 * If an scroll bar listener is already held its reference is release and the new scroll bar listener
	 * stored. If the new scroll bar listener is not NULL a reference is added.
	 */
	igdeSliderListenerReference &operator=( igdeSliderListener *listener );
	
	/**
	 * \brief Store scroll bar listener.
	 * 
	 * If an scroll bar listener is already held its reference is release and the new scroll bar listener
	 * stored. If the new scroll bar listener is not NULL a reference is added.
	 */
	igdeSliderListenerReference &operator=( const igdeSliderListenerReference &reference );
	
	/** \brief Test if scroll bar listener is held by this holder. */
	bool operator==( igdeSliderListener *listener ) const;
	bool operator!=( igdeSliderListener *listener ) const;
	
	/** \brief Test if scroll bar listener is held by this holder. */
	bool operator==( const igdeSliderListenerReference &reference ) const;
	bool operator!=( const igdeSliderListenerReference &reference ) const;
	/*@}*/
};

#endif
