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

#ifndef _IGDESCROLLBARREFERENCE_H_
#define _IGDESCROLLBARREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeScrollBar;


/**
 * \brief ScrollBar reference.
 * 
 * Safe way to hold an scrollbar reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member scrollbar. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeScrollBarReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty scrollbar reference holder. */
	igdeScrollBarReference();
	
	/**
	 * \brief Create scrollbar reference holder with scrollbar.
	 * 
	 * Reference is added if scrollbar is not NULL.
	 */
	igdeScrollBarReference( igdeScrollBar *scrollbar );
	
	/**
	 * \brief Create scrollbar reference holder with scrollbar from another holder.
	 * 
	 * Reference is added if scrollbar in holder is not NULL.
	 */
	igdeScrollBarReference( const igdeScrollBarReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set scrollbar without adding reference.
	 * 
	 * Use this method if the scrollbar to hold has been added a reference already. This is
	 * the case with created scrollbars as well as certain methods returning newly created
	 * scrollbars. In all these cases the scrollbar has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em scrollbar to be a NULL scrollbar.
	 */
	void TakeOver( igdeScrollBar *scrollbar );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if scrollbar is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to scrollbar. */
	operator igdeScrollBar*() const;
	
	/**
	 * \brief Reference to scrollbar.
	 * 
	 * \throws deeNullPointer if scrollbar is NULL.
	 */
	operator igdeScrollBar&() const;
	
	/**
	 * \brief Pointer to scrollbar.
	 * 
	 * \throws deeNullPointer if scrollbar is NULL.
	 */
	igdeScrollBar* operator->() const;
	
	/**
	 * \brief Store scrollbar.
	 * 
	 * If an scrollbar is already held its reference is release and the new scrollbar
	 * stored. If the new scrollbar is not NULL a reference is added.
	 */
	igdeScrollBarReference &operator=( igdeScrollBar *scrollbar );
	
	/**
	 * \brief Store scrollbar.
	 * 
	 * If an scrollbar is already held its reference is release and the new scrollbar
	 * stored. If the new scrollbar is not NULL a reference is added.
	 */
	igdeScrollBarReference &operator=( const igdeScrollBarReference &reference );
	
	/** \brief Test if scrollbar is held by this holder. */
	bool operator==( igdeScrollBar *scrollbar ) const;
	bool operator!=( igdeScrollBar *scrollbar ) const;
	
	/** \brief Test if scrollbar is held by this holder. */
	bool operator==( const igdeScrollBarReference &reference ) const;
	bool operator!=( const igdeScrollBarReference &reference ) const;
	/*@}*/
};

#endif
