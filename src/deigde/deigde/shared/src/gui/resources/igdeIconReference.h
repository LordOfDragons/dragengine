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

#ifndef _IGDEICONREFERENCE_H_
#define _IGDEICONREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeIcon;


/**
 * \brief Icon reference.
 * 
 * Safe way to hold an icon reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member icon. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeIconReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty icon reference holder. */
	igdeIconReference();
	
	/**
	 * \brief Create icon reference holder with icon.
	 * 
	 * Reference is added if icon is not NULL.
	 */
	igdeIconReference( igdeIcon *icon );
	
	/**
	 * \brief Create icon reference holder with icon from another holder.
	 * 
	 * Reference is added if icon in holder is not NULL.
	 */
	igdeIconReference( const igdeIconReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set icon without adding reference.
	 * 
	 * Use this method if the icon to hold has been added a reference already. This is
	 * the case with created icons as well as certain methods returning newly created
	 * icons. In all these cases the icon has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em icon to be a NULL icon.
	 */
	void TakeOver( igdeIcon *icon );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if icon is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to icon. */
	operator igdeIcon*() const;
	
	/**
	 * \brief Reference to icon.
	 * 
	 * \throws deeNullPointer if icon is NULL.
	 */
	operator igdeIcon&() const;
	
	/**
	 * \brief Pointer to icon.
	 * 
	 * \throws deeNullPointer if icon is NULL.
	 */
	igdeIcon* operator->() const;
	
	/**
	 * \brief Store icon.
	 * 
	 * If an icon is already held its reference is release and the new icon
	 * stored. If the new icon is not NULL a reference is added.
	 */
	igdeIconReference &operator=( igdeIcon *icon );
	
	/**
	 * \brief Store icon.
	 * 
	 * If an icon is already held its reference is release and the new icon
	 * stored. If the new icon is not NULL a reference is added.
	 */
	igdeIconReference &operator=( const igdeIconReference &reference );
	
	/** \brief Test if icon is held by this holder. */
	bool operator==( igdeIcon *icon ) const;
	bool operator!=( igdeIcon *icon ) const;
	
	/** \brief Test if icon is held by this holder. */
	bool operator==( const igdeIconReference &reference ) const;
	bool operator!=( const igdeIconReference &reference ) const;
	/*@}*/
};

#endif
