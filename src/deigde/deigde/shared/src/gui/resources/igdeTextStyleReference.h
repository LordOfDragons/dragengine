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

#ifndef _IGDETEXTSTYLEREFERENCE_H_
#define _IGDETEXTSTYLEREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeTextStyle;


/**
 * \brief TextStyle reference.
 * 
 * Safe way to hold an text style reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member text style. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeTextStyleReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty text style reference holder. */
	igdeTextStyleReference();
	
	/**
	 * \brief Create text style reference holder with text style.
	 * 
	 * Reference is added if text style is not NULL.
	 */
	igdeTextStyleReference( igdeTextStyle *textStyle );
	
	/**
	 * \brief Create text style reference holder with text style from another holder.
	 * 
	 * Reference is added if text style in holder is not NULL.
	 */
	igdeTextStyleReference( const igdeTextStyleReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set text style without adding reference.
	 * 
	 * Use this method if the text style to hold has been added a reference already. This is
	 * the case with created text styles as well as certain methods returning newly created
	 * text styles. In all these cases the text style has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em text style to be a NULL text style.
	 */
	void TakeOver( igdeTextStyle *textStyle );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if text style is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to text style. */
	operator igdeTextStyle*() const;
	
	/**
	 * \brief Reference to text style.
	 * 
	 * \throws deeNullPointer if text style is NULL.
	 */
	operator igdeTextStyle&() const;
	
	/**
	 * \brief Pointer to text style.
	 * 
	 * \throws deeNullPointer if text style is NULL.
	 */
	igdeTextStyle* operator->() const;
	
	/**
	 * \brief Store text style.
	 * 
	 * If an text style is already held its reference is release and the new text style
	 * stored. If the new text style is not NULL a reference is added.
	 */
	igdeTextStyleReference &operator=( igdeTextStyle *textStyle );
	
	/**
	 * \brief Store text style.
	 * 
	 * If an text style is already held its reference is release and the new text style
	 * stored. If the new text style is not NULL a reference is added.
	 */
	igdeTextStyleReference &operator=( const igdeTextStyleReference &reference );
	
	/** \brief Test if text style is held by this holder. */
	bool operator==( igdeTextStyle *textStyle ) const;
	bool operator!=( igdeTextStyle *textStyle ) const;
	
	/** \brief Test if text style is held by this holder. */
	bool operator==( const igdeTextStyleReference &reference ) const;
	bool operator!=( const igdeTextStyleReference &reference ) const;
	/*@}*/
};

#endif
