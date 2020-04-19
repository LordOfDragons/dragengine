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

#ifndef _IGDECOLORBOXREFERENCE_H_
#define _IGDECOLORBOXREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeColorBox;


/**
 * \brief ColorBox reference.
 * 
 * Safe way to hold an colorbox reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member colorbox. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeColorBoxReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty colorbox reference holder. */
	igdeColorBoxReference();
	
	/**
	 * \brief Create colorbox reference holder with colorbox.
	 * 
	 * Reference is added if colorbox is not NULL.
	 */
	igdeColorBoxReference( igdeColorBox *colorbox );
	
	/**
	 * \brief Create colorbox reference holder with colorbox from another holder.
	 * 
	 * Reference is added if colorbox in holder is not NULL.
	 */
	igdeColorBoxReference( const igdeColorBoxReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set colorbox without adding reference.
	 * 
	 * Use this method if the colorbox to hold has been added a reference already. This is
	 * the case with created colorboxs as well as certain methods returning newly created
	 * colorboxs. In all these cases the colorbox has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em colorbox to be a NULL colorbox.
	 */
	void TakeOver( igdeColorBox *colorbox );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if colorbox is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to colorbox. */
	operator igdeColorBox*() const;
	
	/**
	 * \brief Reference to colorbox.
	 * 
	 * \throws deeNullPointer if colorbox is NULL.
	 */
	operator igdeColorBox&() const;
	
	/**
	 * \brief Pointer to colorbox.
	 * 
	 * \throws deeNullPointer if colorbox is NULL.
	 */
	igdeColorBox* operator->() const;
	
	/**
	 * \brief Store colorbox.
	 * 
	 * If an colorbox is already held its reference is release and the new colorbox
	 * stored. If the new colorbox is not NULL a reference is added.
	 */
	igdeColorBoxReference &operator=( igdeColorBox *colorbox );
	
	/**
	 * \brief Store colorbox.
	 * 
	 * If an colorbox is already held its reference is release and the new colorbox
	 * stored. If the new colorbox is not NULL a reference is added.
	 */
	igdeColorBoxReference &operator=( const igdeColorBoxReference &reference );
	
	/** \brief Test if colorbox is held by this holder. */
	bool operator==( igdeColorBox *colorbox ) const;
	bool operator!=( igdeColorBox *colorbox ) const;
	
	/** \brief Test if colorbox is held by this holder. */
	bool operator==( const igdeColorBoxReference &reference ) const;
	bool operator!=( const igdeColorBoxReference &reference ) const;
	/*@}*/
};

#endif
