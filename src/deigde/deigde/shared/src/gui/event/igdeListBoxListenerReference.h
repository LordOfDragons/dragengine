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

#ifndef _IGDELISTBOXLISTENERREFERENCE_H_
#define _IGDELISTBOXLISTENERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeListBoxListener;


/**
 * \brief ListBoxListener reference.
 * 
 * Safe way to hold an listbox listener reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member listbox listener. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeListBoxListenerReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty listbox listener reference holder. */
	igdeListBoxListenerReference();
	
	/**
	 * \brief Create listbox listener reference holder with listbox listener.
	 * 
	 * Reference is added if listbox listener is not NULL.
	 */
	igdeListBoxListenerReference( igdeListBoxListener *listener );
	
	/**
	 * \brief Create listbox listener reference holder with listbox listener from another holder.
	 * 
	 * Reference is added if listbox listener in holder is not NULL.
	 */
	igdeListBoxListenerReference( const igdeListBoxListenerReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set listbox listener without adding reference.
	 * 
	 * Use this method if the listbox listener to hold has been added a reference already. This is
	 * the case with created listbox listeners as well as certain methods returning newly created
	 * listbox listeners. In all these cases the listbox listener has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em listbox listener to be a NULL listbox listener.
	 */
	void TakeOver( igdeListBoxListener *listener );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if listbox listener is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to listbox listener. */
	operator igdeListBoxListener*() const;
	
	/**
	 * \brief Reference to listbox listener.
	 * 
	 * \throws deeNullPointer if listbox listener is NULL.
	 */
	operator igdeListBoxListener&() const;
	
	/**
	 * \brief Pointer to listbox listener.
	 * 
	 * \throws deeNullPointer if listbox listener is NULL.
	 */
	igdeListBoxListener* operator->() const;
	
	/**
	 * \brief Store listbox listener.
	 * 
	 * If an listbox listener is already held its reference is release and the new listbox listener
	 * stored. If the new listbox listener is not NULL a reference is added.
	 */
	igdeListBoxListenerReference &operator=( igdeListBoxListener *listener );
	
	/**
	 * \brief Store listbox listener.
	 * 
	 * If an listbox listener is already held its reference is release and the new listbox listener
	 * stored. If the new listbox listener is not NULL a reference is added.
	 */
	igdeListBoxListenerReference &operator=( const igdeListBoxListenerReference &reference );
	
	/** \brief Test if listbox listener is held by this holder. */
	bool operator==( igdeListBoxListener *listener ) const;
	bool operator!=( igdeListBoxListener *listener ) const;
	
	/** \brief Test if listbox listener is held by this holder. */
	bool operator==( const igdeListBoxListenerReference &reference ) const;
	bool operator!=( const igdeListBoxListenerReference &reference ) const;
	/*@}*/
};

#endif
