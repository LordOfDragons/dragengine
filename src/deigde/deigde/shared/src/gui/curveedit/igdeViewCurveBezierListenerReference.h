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

#ifndef _IGDEVIEWCURVEBEZIERREFERENCE_H_
#define _IGDEVIEWCURVEBEZIERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeViewCurveBezierListener;


/**
 * \brief ViewCurveBezierListener reference.
 * 
 * Safe way to hold an textfield listener reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member textfield listener. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeViewCurveBezierListenerReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty textfield listener reference holder. */
	igdeViewCurveBezierListenerReference();
	
	/**
	 * \brief Create textfield listener reference holder with textfield listener.
	 * 
	 * Reference is added if textfield listener is not NULL.
	 */
	igdeViewCurveBezierListenerReference( igdeViewCurveBezierListener *listener );
	
	/**
	 * \brief Create textfield listener reference holder with textfield listener from another holder.
	 * 
	 * Reference is added if textfield listener in holder is not NULL.
	 */
	igdeViewCurveBezierListenerReference( const igdeViewCurveBezierListenerReference &reference );
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
	void TakeOver( igdeViewCurveBezierListener *listener );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if textfield listener is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to textfield listener. */
	operator igdeViewCurveBezierListener*() const;
	
	/**
	 * \brief Reference to textfield listener.
	 * 
	 * \throws deeNullPointer if textfield listener is NULL.
	 */
	operator igdeViewCurveBezierListener&() const;
	
	/**
	 * \brief Pointer to textfield listener.
	 * 
	 * \throws deeNullPointer if textfield listener is NULL.
	 */
	igdeViewCurveBezierListener* operator->() const;
	
	/**
	 * \brief Store textfield listener.
	 * 
	 * If an textfield listener is already held its reference is release and the new textfield listener
	 * stored. If the new textfield listener is not NULL a reference is added.
	 */
	igdeViewCurveBezierListenerReference &operator=( igdeViewCurveBezierListener *listener );
	
	/**
	 * \brief Store textfield listener.
	 * 
	 * If an textfield listener is already held its reference is release and the new textfield listener
	 * stored. If the new textfield listener is not NULL a reference is added.
	 */
	igdeViewCurveBezierListenerReference &operator=( const igdeViewCurveBezierListenerReference &reference );
	
	/** \brief Test if textfield listener is held by this holder. */
	bool operator==( igdeViewCurveBezierListener *listener ) const;
	bool operator!=( igdeViewCurveBezierListener *listener ) const;
	
	/** \brief Test if textfield listener is held by this holder. */
	bool operator==( const igdeViewCurveBezierListenerReference &reference ) const;
	bool operator!=( const igdeViewCurveBezierListenerReference &reference ) const;
	/*@}*/
};

#endif
