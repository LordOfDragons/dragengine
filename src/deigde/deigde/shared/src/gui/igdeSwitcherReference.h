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

#ifndef _IGDESWITCHERREFERENCE_H_
#define _IGDESWITCHERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeSwitcher;


/**
 * \brief Switcher reference.
 * 
 * Safe way to hold an switcher reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member switcher. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeSwitcherReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty switcher reference holder. */
	igdeSwitcherReference();
	
	/**
	 * \brief Create switcher reference holder with switcher.
	 * 
	 * Reference is added if switcher is not NULL.
	 */
	igdeSwitcherReference( igdeSwitcher *switcher );
	
	/**
	 * \brief Create switcher reference holder with switcher from another holder.
	 * 
	 * Reference is added if switcher in holder is not NULL.
	 */
	igdeSwitcherReference( const igdeSwitcherReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set switcher without adding reference.
	 * 
	 * Use this method if the switcher to hold has been added a reference already. This is
	 * the case with created switchers as well as certain methods returning newly created
	 * switchers. In all these cases the switcher has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em switcher to be a NULL switcher.
	 */
	void TakeOver( igdeSwitcher *switcher );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if switcher is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to switcher. */
	operator igdeSwitcher*() const;
	
	/**
	 * \brief Reference to switcher.
	 * 
	 * \throws deeNullPointer if switcher is NULL.
	 */
	operator igdeSwitcher&() const;
	
	/**
	 * \brief Pointer to switcher.
	 * 
	 * \throws deeNullPointer if switcher is NULL.
	 */
	igdeSwitcher* operator->() const;
	
	/**
	 * \brief Store switcher.
	 * 
	 * If an switcher is already held its reference is release and the new switcher
	 * stored. If the new switcher is not NULL a reference is added.
	 */
	igdeSwitcherReference &operator=( igdeSwitcher *switcher );
	
	/**
	 * \brief Store switcher.
	 * 
	 * If an switcher is already held its reference is release and the new switcher
	 * stored. If the new switcher is not NULL a reference is added.
	 */
	igdeSwitcherReference &operator=( const igdeSwitcherReference &reference );
	
	/** \brief Test if switcher is held by this holder. */
	bool operator==( igdeSwitcher *switcher ) const;
	bool operator!=( igdeSwitcher *switcher ) const;
	
	/** \brief Test if switcher is held by this holder. */
	bool operator==( const igdeSwitcherReference &reference ) const;
	bool operator!=( const igdeSwitcherReference &reference ) const;
	/*@}*/
};

#endif
