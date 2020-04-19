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

#ifndef _IGDETOOLBARREFERENCE_H_
#define _IGDETOOLBARREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeToolBar;


/**
 * \brief ToolBar reference.
 * 
 * Safe way to hold an toolbar reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member toolbar. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeToolBarReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty toolbar reference holder. */
	igdeToolBarReference();
	
	/**
	 * \brief Create toolbar reference holder with toolbar.
	 * 
	 * Reference is added if toolbar is not NULL.
	 */
	igdeToolBarReference( igdeToolBar *toolbar );
	
	/**
	 * \brief Create toolbar reference holder with toolbar from another holder.
	 * 
	 * Reference is added if toolbar in holder is not NULL.
	 */
	igdeToolBarReference( const igdeToolBarReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set toolbar without adding reference.
	 * 
	 * Use this method if the toolbar to hold has been added a reference already. This is
	 * the case with created toolbars as well as certain methods returning newly created
	 * toolbars. In all these cases the toolbar has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em toolbar to be a NULL toolbar.
	 */
	void TakeOver( igdeToolBar *toolbar );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if toolbar is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to toolbar. */
	operator igdeToolBar*() const;
	
	/**
	 * \brief Reference to toolbar.
	 * 
	 * \throws deeNullPointer if toolbar is NULL.
	 */
	operator igdeToolBar&() const;
	
	/**
	 * \brief Pointer to toolbar.
	 * 
	 * \throws deeNullPointer if toolbar is NULL.
	 */
	igdeToolBar* operator->() const;
	
	/**
	 * \brief Store toolbar.
	 * 
	 * If an toolbar is already held its reference is release and the new toolbar
	 * stored. If the new toolbar is not NULL a reference is added.
	 */
	igdeToolBarReference &operator=( igdeToolBar *toolbar );
	
	/**
	 * \brief Store toolbar.
	 * 
	 * If an toolbar is already held its reference is release and the new toolbar
	 * stored. If the new toolbar is not NULL a reference is added.
	 */
	igdeToolBarReference &operator=( const igdeToolBarReference &reference );
	
	/** \brief Test if toolbar is held by this holder. */
	bool operator==( igdeToolBar *toolbar ) const;
	bool operator!=( igdeToolBar *toolbar ) const;
	
	/** \brief Test if toolbar is held by this holder. */
	bool operator==( const igdeToolBarReference &reference ) const;
	bool operator!=( const igdeToolBarReference &reference ) const;
	/*@}*/
};

#endif
