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

#ifndef _IGDEEDITPROPERTYVALUEREFERENCE_H_
#define _IGDEEDITPROPERTYVALUEREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeEditPropertyValueListener;


/**
 * \brief EditPropertyValueListener reference.
 * 
 * Safe way to hold an edit property value listener reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member edit property value listener. Do not use
 *       it as property valueer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeEditPropertyValueListenerReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty edit property value listener reference holder. */
	igdeEditPropertyValueListenerReference();
	
	/**
	 * \brief Create edit property value listener reference holder with edit property value listener.
	 * 
	 * Reference is added if edit property value listener is not NULL.
	 */
	igdeEditPropertyValueListenerReference( igdeEditPropertyValueListener *listener );
	
	/**
	 * \brief Create edit property value listener reference holder with edit property value listener from another holder.
	 * 
	 * Reference is added if edit property value listener in holder is not NULL.
	 */
	igdeEditPropertyValueListenerReference( const igdeEditPropertyValueListenerReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set edit property value listener without adding reference.
	 * 
	 * Use this method if the edit property value listener to hold has been added a reference already. This is
	 * the case with created edit property value listeners as well as certain methods returning newly created
	 * edit property value listeners. In all these cases the edit property value listener has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em edit property value listener to be a NULL edit property value listener.
	 */
	void TakeOver( igdeEditPropertyValueListener *listener );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if edit property value listener is not NULL. */
	operator bool() const;
	
	/** \brief PropertyValueer to edit property value listener. */
	operator igdeEditPropertyValueListener*() const;
	
	/**
	 * \brief Reference to edit property value listener.
	 * 
	 * \throws deeNullPropertyValueer if edit property value listener is NULL.
	 */
	operator igdeEditPropertyValueListener&() const;
	
	/**
	 * \brief PropertyValueer to edit property value listener.
	 * 
	 * \throws deeNullPropertyValueer if edit property value listener is NULL.
	 */
	igdeEditPropertyValueListener* operator->() const;
	
	/**
	 * \brief Store edit property value listener.
	 * 
	 * If an edit property value listener is already held its reference is release and the new edit property value listener
	 * stored. If the new edit property value listener is not NULL a reference is added.
	 */
	igdeEditPropertyValueListenerReference &operator=( igdeEditPropertyValueListener *listener );
	
	/**
	 * \brief Store edit property value listener.
	 * 
	 * If an edit property value listener is already held its reference is release and the new edit property value listener
	 * stored. If the new edit property value listener is not NULL a reference is added.
	 */
	igdeEditPropertyValueListenerReference &operator=( const igdeEditPropertyValueListenerReference &reference );
	
	/** \brief Test if edit property value listener is held by this holder. */
	bool operator==( igdeEditPropertyValueListener *listener ) const;
	bool operator!=( igdeEditPropertyValueListener *listener ) const;
	
	/** \brief Test if edit property value listener is held by this holder. */
	bool operator==( const igdeEditPropertyValueListenerReference &reference ) const;
	bool operator!=( const igdeEditPropertyValueListenerReference &reference ) const;
	/*@}*/
};

#endif
