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

#ifndef _IGDEWINDOWLOGGERREFERENCE_H_
#define _IGDEWINDOWLOGGERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeWindowLogger;


/**
 * \brief Window logger reference.
 * 
 * Safe way to hold an window reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member window. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeWindowLoggerReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty window reference holder. */
	igdeWindowLoggerReference();
	
	/**
	 * \brief Create window reference holder with window.
	 * 
	 * Reference is added if window is not NULL.
	 */
	igdeWindowLoggerReference( igdeWindowLogger *window );
	
	/**
	 * \brief Create window reference holder with window from another holder.
	 * 
	 * Reference is added if window in holder is not NULL.
	 */
	igdeWindowLoggerReference( const igdeWindowLoggerReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set window without adding reference.
	 * 
	 * Use this method if the window to hold has been added a reference already. This is
	 * the case with created windows as well as certain methods returning newly created
	 * windows. In all these cases the window has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em window to be a NULL window.
	 */
	void TakeOver( igdeWindowLogger *window );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if window is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to window. */
	operator igdeWindowLogger*() const;
	
	/**
	 * \brief Reference to window.
	 * 
	 * \throws deeNullPointer if window is NULL.
	 */
	operator igdeWindowLogger&() const;
	
	/**
	 * \brief Pointer to window.
	 * 
	 * \throws deeNullPointer if window is NULL.
	 */
	igdeWindowLogger* operator->() const;
	
	/**
	 * \brief Store window.
	 * 
	 * If an window is already held its reference is release and the new window
	 * stored. If the new window is not NULL a reference is added.
	 */
	igdeWindowLoggerReference &operator=( igdeWindowLogger *window );
	
	/**
	 * \brief Store window.
	 * 
	 * If an window is already held its reference is release and the new window
	 * stored. If the new window is not NULL a reference is added.
	 */
	igdeWindowLoggerReference &operator=( const igdeWindowLoggerReference &reference );
	
	/** \brief Test if window is held by this holder. */
	bool operator==( igdeWindowLogger *window ) const;
	bool operator!=( igdeWindowLogger *window ) const;
	
	/** \brief Test if window is held by this holder. */
	bool operator==( const igdeWindowLoggerReference &reference ) const;
	bool operator!=( const igdeWindowLoggerReference &reference ) const;
	/*@}*/
};

#endif
