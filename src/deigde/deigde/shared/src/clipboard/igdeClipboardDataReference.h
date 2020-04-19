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

#ifndef _IGDECLIPBOARDDATAREFERENCE_H_
#define _IGDECLIPBOARDDATAREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeClipboardData;


/**
 * \brief ClipboardData reference.
 * 
 * Safe way to hold an clipboard data reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member clipboard data. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeClipboardDataReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty clipboard data reference holder. */
	igdeClipboardDataReference();
	
	/**
	 * \brief Create clipboard data reference holder with clipboard data.
	 * 
	 * Reference is added if clipboard data is not NULL.
	 */
	igdeClipboardDataReference( igdeClipboardData *clipboardData );
	
	/**
	 * \brief Create clipboard data reference holder with clipboard data from another holder.
	 * 
	 * Reference is added if clipboard data in holder is not NULL.
	 */
	igdeClipboardDataReference( const igdeClipboardDataReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set clipboard data without adding reference.
	 * 
	 * Use this method if the clipboard data to hold has been added a reference already. This is
	 * the case with created clipboard datas as well as certain methods returning newly created
	 * clipboard datas. In all these cases the clipboard data has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em clipboard data to be a NULL clipboard data.
	 */
	void TakeOver( igdeClipboardData *clipboardData );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if clipboard data is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to clipboard data. */
	operator igdeClipboardData*() const;
	
	/**
	 * \brief Reference to clipboard data.
	 * 
	 * \throws deeNullPointer if clipboard data is NULL.
	 */
	operator igdeClipboardData&() const;
	
	/**
	 * \brief Pointer to clipboard data.
	 * 
	 * \throws deeNullPointer if clipboard data is NULL.
	 */
	igdeClipboardData* operator->() const;
	
	/**
	 * \brief Store clipboard data.
	 * 
	 * If an clipboard data is already held its reference is release and the new clipboard data
	 * stored. If the new clipboard data is not NULL a reference is added.
	 */
	igdeClipboardDataReference &operator=( igdeClipboardData *clipboardData );
	
	/**
	 * \brief Store clipboard data.
	 * 
	 * If an clipboard data is already held its reference is release and the new clipboard data
	 * stored. If the new clipboard data is not NULL a reference is added.
	 */
	igdeClipboardDataReference &operator=( const igdeClipboardDataReference &reference );
	
	/** \brief Test if clipboard data is held by this holder. */
	bool operator==( igdeClipboardData *clipboardData ) const;
	bool operator!=( igdeClipboardData *clipboardData ) const;
	
	/** \brief Test if clipboard data is held by this holder. */
	bool operator==( const igdeClipboardDataReference &reference ) const;
	bool operator!=( const igdeClipboardDataReference &reference ) const;
	/*@}*/
};

#endif
