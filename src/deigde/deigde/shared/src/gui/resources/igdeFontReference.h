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

#ifndef _IGDEFONTREFERENCE_H_
#define _IGDEFONTREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeFont;


/**
 * \brief Font reference.
 * 
 * Safe way to hold an font reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member font. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeFontReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty font reference holder. */
	igdeFontReference();
	
	/**
	 * \brief Create font reference holder with font.
	 * 
	 * Reference is added if font is not NULL.
	 */
	igdeFontReference( igdeFont *font );
	
	/**
	 * \brief Create font reference holder with font from another holder.
	 * 
	 * Reference is added if font in holder is not NULL.
	 */
	igdeFontReference( const igdeFontReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set font without adding reference.
	 * 
	 * Use this method if the font to hold has been added a reference already. This is
	 * the case with created fonts as well as certain methods returning newly created
	 * fonts. In all these cases the font has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em font to be a NULL font.
	 */
	void TakeOver( igdeFont *font );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if font is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to font. */
	operator igdeFont*() const;
	
	/**
	 * \brief Reference to font.
	 * 
	 * \throws deeNullPointer if font is NULL.
	 */
	operator igdeFont&() const;
	
	/**
	 * \brief Pointer to font.
	 * 
	 * \throws deeNullPointer if font is NULL.
	 */
	igdeFont* operator->() const;
	
	/**
	 * \brief Store font.
	 * 
	 * If an font is already held its reference is release and the new font
	 * stored. If the new font is not NULL a reference is added.
	 */
	igdeFontReference &operator=( igdeFont *font );
	
	/**
	 * \brief Store font.
	 * 
	 * If an font is already held its reference is release and the new font
	 * stored. If the new font is not NULL a reference is added.
	 */
	igdeFontReference &operator=( const igdeFontReference &reference );
	
	/** \brief Test if font is held by this holder. */
	bool operator==( igdeFont *font ) const;
	bool operator!=( igdeFont *font ) const;
	
	/** \brief Test if font is held by this holder. */
	bool operator==( const igdeFontReference &reference ) const;
	bool operator!=( const igdeFontReference &reference ) const;
	/*@}*/
};

#endif
