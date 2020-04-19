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

#ifndef _IGDETABBOOKREFERENCE_H_
#define _IGDETABBOOKREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeTabBook;


/**
 * \brief TabBook reference.
 * 
 * Safe way to hold an tabbook reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member tabbook. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeTabBookReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty tabbook reference holder. */
	igdeTabBookReference();
	
	/**
	 * \brief Create tabbook reference holder with tabbook.
	 * 
	 * Reference is added if tabbook is not NULL.
	 */
	igdeTabBookReference( igdeTabBook *tabbook );
	
	/**
	 * \brief Create tabbook reference holder with tabbook from another holder.
	 * 
	 * Reference is added if tabbook in holder is not NULL.
	 */
	igdeTabBookReference( const igdeTabBookReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set tabbook without adding reference.
	 * 
	 * Use this method if the tabbook to hold has been added a reference already. This is
	 * the case with created tabbooks as well as certain methods returning newly created
	 * tabbooks. In all these cases the tabbook has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em tabbook to be a NULL tabbook.
	 */
	void TakeOver( igdeTabBook *tabbook );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if tabbook is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to tabbook. */
	operator igdeTabBook*() const;
	
	/**
	 * \brief Reference to tabbook.
	 * 
	 * \throws deeNullPointer if tabbook is NULL.
	 */
	operator igdeTabBook&() const;
	
	/**
	 * \brief Pointer to tabbook.
	 * 
	 * \throws deeNullPointer if tabbook is NULL.
	 */
	igdeTabBook* operator->() const;
	
	/**
	 * \brief Store tabbook.
	 * 
	 * If an tabbook is already held its reference is release and the new tabbook
	 * stored. If the new tabbook is not NULL a reference is added.
	 */
	igdeTabBookReference &operator=( igdeTabBook *tabbook );
	
	/**
	 * \brief Store tabbook.
	 * 
	 * If an tabbook is already held its reference is release and the new tabbook
	 * stored. If the new tabbook is not NULL a reference is added.
	 */
	igdeTabBookReference &operator=( const igdeTabBookReference &reference );
	
	/** \brief Test if tabbook is held by this holder. */
	bool operator==( igdeTabBook *tabbook ) const;
	bool operator!=( igdeTabBook *tabbook ) const;
	
	/** \brief Test if tabbook is held by this holder. */
	bool operator==( const igdeTabBookReference &reference ) const;
	bool operator!=( const igdeTabBookReference &reference ) const;
	/*@}*/
};

#endif
