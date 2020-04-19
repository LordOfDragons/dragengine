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

#ifndef _IGDESTATUSBARREFERENCE_H_
#define _IGDESTATUSBARREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeStatusBar;


/**
 * \brief StatusBar reference.
 * 
 * Safe way to hold an statusBar reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member statusBar. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeStatusBarReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty statusBar reference holder. */
	igdeStatusBarReference();
	
	/**
	 * \brief Create statusBar reference holder with statusBar.
	 * 
	 * Reference is added if statusBar is not NULL.
	 */
	igdeStatusBarReference( igdeStatusBar *statusBar );
	
	/**
	 * \brief Create statusBar reference holder with statusBar from another holder.
	 * 
	 * Reference is added if statusBar in holder is not NULL.
	 */
	igdeStatusBarReference( const igdeStatusBarReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set statusBar without adding reference.
	 * 
	 * Use this method if the statusBar to hold has been added a reference already. This is
	 * the case with created statusBars as well as certain methods returning newly created
	 * statusBars. In all these cases the statusBar has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em statusBar to be a NULL statusBar.
	 */
	void TakeOver( igdeStatusBar *statusBar );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if statusBar is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to statusBar. */
	operator igdeStatusBar*() const;
	
	/**
	 * \brief Reference to statusBar.
	 * 
	 * \throws deeNullPointer if statusBar is NULL.
	 */
	operator igdeStatusBar&() const;
	
	/**
	 * \brief Pointer to statusBar.
	 * 
	 * \throws deeNullPointer if statusBar is NULL.
	 */
	igdeStatusBar* operator->() const;
	
	/**
	 * \brief Store statusBar.
	 * 
	 * If an statusBar is already held its reference is release and the new statusBar
	 * stored. If the new statusBar is not NULL a reference is added.
	 */
	igdeStatusBarReference &operator=( igdeStatusBar *statusBar );
	
	/**
	 * \brief Store statusBar.
	 * 
	 * If an statusBar is already held its reference is release and the new statusBar
	 * stored. If the new statusBar is not NULL a reference is added.
	 */
	igdeStatusBarReference &operator=( const igdeStatusBarReference &reference );
	
	/** \brief Test if statusBar is held by this holder. */
	bool operator==( igdeStatusBar *statusBar ) const;
	bool operator!=( igdeStatusBar *statusBar ) const;
	
	/** \brief Test if statusBar is held by this holder. */
	bool operator==( const igdeStatusBarReference &reference ) const;
	bool operator!=( const igdeStatusBarReference &reference ) const;
	/*@}*/
};

#endif
