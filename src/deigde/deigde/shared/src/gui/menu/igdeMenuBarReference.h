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

#ifndef _IGDEMENUBARREFERENCE_H_
#define _IGDEMENUBARREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeMenuBar;


/**
 * \brief MenuBar reference.
 * 
 * Safe way to hold a menu bar reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member menu bar. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeMenuBarReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty menu bar reference holder. */
	igdeMenuBarReference();
	
	/**
	 * \brief Create menu bar reference holder with menu bar.
	 * 
	 * Reference is added if menu bar is not NULL.
	 */
	igdeMenuBarReference( igdeMenuBar *menubar );
	
	/**
	 * \brief Create menu bar reference holder with menu bar from another holder.
	 * 
	 * Reference is added if menu bar in holder is not NULL.
	 */
	igdeMenuBarReference( const igdeMenuBarReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set menu bar without adding reference.
	 * 
	 * Use this method if the menu bar to hold has been added a reference already. This is
	 * the case with created menu bars as well as certain methods returning newly created
	 * menu bars. In all these cases the menu bar has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em menu bar to be a NULL menu bar.
	 */
	void TakeOver( igdeMenuBar *menubar );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if menu bar is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to menu bar. */
	operator igdeMenuBar*() const;
	
	/**
	 * \brief Reference to menu bar.
	 * 
	 * \throws deeNullPointer if menu bar is NULL.
	 */
	operator igdeMenuBar&() const;
	
	/**
	 * \brief Pointer to menu bar.
	 * 
	 * \throws deeNullPointer if menu bar is NULL.
	 */
	igdeMenuBar* operator->() const;
	
	/**
	 * \brief Store menu bar.
	 * 
	 * If an menu bar is already held its reference is release and the new menu bar
	 * stored. If the new menu bar is not NULL a reference is added.
	 */
	igdeMenuBarReference &operator=( igdeMenuBar *menubar );
	
	/**
	 * \brief Store menu bar.
	 * 
	 * If an menu bar is already held its reference is release and the new menu bar
	 * stored. If the new menu bar is not NULL a reference is added.
	 */
	igdeMenuBarReference &operator=( const igdeMenuBarReference &reference );
	
	/** \brief Test if menu bar is held by this holder. */
	bool operator==( igdeMenuBar *menubar ) const;
	bool operator!=( igdeMenuBar *menubar ) const;
	
	/** \brief Test if menu bar is held by this holder. */
	bool operator==( const igdeMenuBarReference &reference ) const;
	bool operator!=( const igdeMenuBarReference &reference ) const;
	/*@}*/
};

#endif
