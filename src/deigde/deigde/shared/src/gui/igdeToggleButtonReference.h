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

#ifndef _IGDETOGGLEBUTTONREFERENCE_H_
#define _IGDETOGGLEBUTTONREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeToggleButton;


/**
 * \brief ToggleButton reference.
 * 
 * Safe way to hold an button reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member button. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeToggleButtonReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty button reference holder. */
	igdeToggleButtonReference();
	
	/**
	 * \brief Create button reference holder with button.
	 * 
	 * Reference is added if button is not NULL.
	 */
	igdeToggleButtonReference( igdeToggleButton *button );
	
	/**
	 * \brief Create button reference holder with button from another holder.
	 * 
	 * Reference is added if button in holder is not NULL.
	 */
	igdeToggleButtonReference( const igdeToggleButtonReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set button without adding reference.
	 * 
	 * Use this method if the button to hold has been added a reference already. This is
	 * the case with created buttons as well as certain methods returning newly created
	 * buttons. In all these cases the button has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em button to be a NULL button.
	 */
	void TakeOver( igdeToggleButton *button );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if button is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to button. */
	operator igdeToggleButton*() const;
	
	/**
	 * \brief Reference to button.
	 * 
	 * \throws deeNullPointer if button is NULL.
	 */
	operator igdeToggleButton&() const;
	
	/**
	 * \brief Pointer to button.
	 * 
	 * \throws deeNullPointer if button is NULL.
	 */
	igdeToggleButton* operator->() const;
	
	/**
	 * \brief Store button.
	 * 
	 * If an button is already held its reference is release and the new button
	 * stored. If the new button is not NULL a reference is added.
	 */
	igdeToggleButtonReference &operator=( igdeToggleButton *button );
	
	/**
	 * \brief Store button.
	 * 
	 * If an button is already held its reference is release and the new button
	 * stored. If the new button is not NULL a reference is added.
	 */
	igdeToggleButtonReference &operator=( const igdeToggleButtonReference &reference );
	
	/** \brief Test if button is held by this holder. */
	bool operator==( igdeToggleButton *button ) const;
	bool operator!=( igdeToggleButton *button ) const;
	
	/** \brief Test if button is held by this holder. */
	bool operator==( const igdeToggleButtonReference &reference ) const;
	bool operator!=( const igdeToggleButtonReference &reference ) const;
	/*@}*/
};

#endif
