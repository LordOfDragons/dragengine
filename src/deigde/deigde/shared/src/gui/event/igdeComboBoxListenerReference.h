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

#ifndef _IGDECOMBOBOXLISTENERREFERENCE_H_
#define _IGDECOMBOBOXLISTENERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeComboBoxListener;


/**
 * \brief ComboBoxListener reference.
 * 
 * Safe way to hold an combobox listener reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member combobox listener. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeComboBoxListenerReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty combobox listener reference holder. */
	igdeComboBoxListenerReference();
	
	/**
	 * \brief Create combobox listener reference holder with combobox listener.
	 * 
	 * Reference is added if combobox listener is not NULL.
	 */
	igdeComboBoxListenerReference( igdeComboBoxListener *listener );
	
	/**
	 * \brief Create combobox listener reference holder with combobox listener from another holder.
	 * 
	 * Reference is added if combobox listener in holder is not NULL.
	 */
	igdeComboBoxListenerReference( const igdeComboBoxListenerReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set combobox listener without adding reference.
	 * 
	 * Use this method if the combobox listener to hold has been added a reference already. This is
	 * the case with created combobox listeners as well as certain methods returning newly created
	 * combobox listeners. In all these cases the combobox listener has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em combobox listener to be a NULL combobox listener.
	 */
	void TakeOver( igdeComboBoxListener *listener );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if combobox listener is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to combobox listener. */
	operator igdeComboBoxListener*() const;
	
	/**
	 * \brief Reference to combobox listener.
	 * 
	 * \throws deeNullPointer if combobox listener is NULL.
	 */
	operator igdeComboBoxListener&() const;
	
	/**
	 * \brief Pointer to combobox listener.
	 * 
	 * \throws deeNullPointer if combobox listener is NULL.
	 */
	igdeComboBoxListener* operator->() const;
	
	/**
	 * \brief Store combobox listener.
	 * 
	 * If an combobox listener is already held its reference is release and the new combobox listener
	 * stored. If the new combobox listener is not NULL a reference is added.
	 */
	igdeComboBoxListenerReference &operator=( igdeComboBoxListener *listener );
	
	/**
	 * \brief Store combobox listener.
	 * 
	 * If an combobox listener is already held its reference is release and the new combobox listener
	 * stored. If the new combobox listener is not NULL a reference is added.
	 */
	igdeComboBoxListenerReference &operator=( const igdeComboBoxListenerReference &reference );
	
	/** \brief Test if combobox listener is held by this holder. */
	bool operator==( igdeComboBoxListener *listener ) const;
	bool operator!=( igdeComboBoxListener *listener ) const;
	
	/** \brief Test if combobox listener is held by this holder. */
	bool operator==( const igdeComboBoxListenerReference &reference ) const;
	bool operator!=( const igdeComboBoxListenerReference &reference ) const;
	/*@}*/
};

#endif
