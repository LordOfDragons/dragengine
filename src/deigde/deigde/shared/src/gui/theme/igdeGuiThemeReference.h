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

#ifndef _IGDEGUITHEMEREFERENCE_H_
#define _IGDEGUITHEMEREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeGuiTheme;


/**
 * \brief GuiTheme reference.
 * 
 * Safe way to hold an guitheme reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member guitheme. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeGuiThemeReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty guitheme reference holder. */
	igdeGuiThemeReference();
	
	/**
	 * \brief Create guitheme reference holder with guitheme.
	 * 
	 * Reference is added if guitheme is not NULL.
	 */
	igdeGuiThemeReference( igdeGuiTheme *guitheme );
	
	/**
	 * \brief Create guitheme reference holder with guitheme from another holder.
	 * 
	 * Reference is added if guitheme in holder is not NULL.
	 */
	igdeGuiThemeReference( const igdeGuiThemeReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set guitheme without adding reference.
	 * 
	 * Use this method if the guitheme to hold has been added a reference already. This is
	 * the case with created guithemes as well as certain methods returning newly created
	 * guithemes. In all these cases the guitheme has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em guitheme to be a NULL guitheme.
	 */
	void TakeOver( igdeGuiTheme *guitheme );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if guitheme is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to guitheme. */
	operator igdeGuiTheme*() const;
	
	/**
	 * \brief Reference to guitheme.
	 * 
	 * \throws deeNullPointer if guitheme is NULL.
	 */
	operator igdeGuiTheme&() const;
	
	/**
	 * \brief Pointer to guitheme.
	 * 
	 * \throws deeNullPointer if guitheme is NULL.
	 */
	igdeGuiTheme* operator->() const;
	
	/**
	 * \brief Store guitheme.
	 * 
	 * If an guitheme is already held its reference is release and the new guitheme
	 * stored. If the new guitheme is not NULL a reference is added.
	 */
	igdeGuiThemeReference &operator=( igdeGuiTheme *guitheme );
	
	/**
	 * \brief Store guitheme.
	 * 
	 * If an guitheme is already held its reference is release and the new guitheme
	 * stored. If the new guitheme is not NULL a reference is added.
	 */
	igdeGuiThemeReference &operator=( const igdeGuiThemeReference &reference );
	
	/** \brief Test if guitheme is held by this holder. */
	bool operator==( igdeGuiTheme *guitheme ) const;
	bool operator!=( igdeGuiTheme *guitheme ) const;
	
	/** \brief Test if guitheme is held by this holder. */
	bool operator==( const igdeGuiThemeReference &reference ) const;
	bool operator!=( const igdeGuiThemeReference &reference ) const;
	/*@}*/
};

#endif
