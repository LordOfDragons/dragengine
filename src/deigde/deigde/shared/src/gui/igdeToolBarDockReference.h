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

#ifndef _IGDETOOLBARDOCKREFERENCE_H_
#define _IGDETOOLBARDOCKREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeToolBarDock;


/**
 * \brief ToolBarDock reference.
 * 
 * Safe way to hold a toolbar dock reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member toolbar dock. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeToolBarDockReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty toolbar dock reference holder. */
	igdeToolBarDockReference();
	
	/**
	 * \brief Create toolbar dock reference holder with toolbar dock.
	 * 
	 * Reference is added if toolbar dock is not NULL.
	 */
	igdeToolBarDockReference( igdeToolBarDock *dock );
	
	/**
	 * \brief Create toolbar dock reference holder with toolbar dock from another holder.
	 * 
	 * Reference is added if toolbar dock in holder is not NULL.
	 */
	igdeToolBarDockReference( const igdeToolBarDockReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set toolbar dock without adding reference.
	 * 
	 * Use this method if the toolbar dock to hold has been added a reference already. This is
	 * the case with created toolbar docks as well as certain methods returning newly created
	 * toolbar docks. In all these cases the toolbar dock has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em toolbar dock to be a NULL toolbar dock.
	 */
	void TakeOver( igdeToolBarDock *dock );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if toolbar dock is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to toolbar dock. */
	operator igdeToolBarDock*() const;
	
	/**
	 * \brief Reference to toolbar dock.
	 * 
	 * \throws deeNullPointer if toolbar dock is NULL.
	 */
	operator igdeToolBarDock&() const;
	
	/**
	 * \brief Pointer to toolbar dock.
	 * 
	 * \throws deeNullPointer if toolbar dock is NULL.
	 */
	igdeToolBarDock* operator->() const;
	
	/**
	 * \brief Store toolbar dock.
	 * 
	 * If an toolbar dock is already held its reference is release and the new toolbar dock
	 * stored. If the new toolbar dock is not NULL a reference is added.
	 */
	igdeToolBarDockReference &operator=( igdeToolBarDock *dock );
	
	/**
	 * \brief Store toolbar dock.
	 * 
	 * If an toolbar dock is already held its reference is release and the new toolbar dock
	 * stored. If the new toolbar dock is not NULL a reference is added.
	 */
	igdeToolBarDockReference &operator=( const igdeToolBarDockReference &reference );
	
	/** \brief Test if toolbar dock is held by this holder. */
	bool operator==( igdeToolBarDock *dock ) const;
	bool operator!=( igdeToolBarDock *dock ) const;
	
	/** \brief Test if toolbar dock is held by this holder. */
	bool operator==( const igdeToolBarDockReference &reference ) const;
	bool operator!=( const igdeToolBarDockReference &reference ) const;
	/*@}*/
};

#endif
