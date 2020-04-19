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

#ifndef _IGDEMENUCASCADEREFERENCE_H_
#define _IGDEMENUCASCADEREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeMenuCascade;


/**
 * \brief Menu cascade reference.
 * 
 * Safe way to hold an menu cascade reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member menu cascade. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeMenuCascadeReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty menu cascade reference holder. */
	igdeMenuCascadeReference();
	
	/**
	 * \brief Create menu cascade reference holder with menu cascade.
	 * 
	 * Reference is added if menu cascade is not NULL.
	 */
	igdeMenuCascadeReference( igdeMenuCascade *menuCascade );
	
	/**
	 * \brief Create menu cascade reference holder with menu cascade from another holder.
	 * 
	 * Reference is added if menu cascade in holder is not NULL.
	 */
	igdeMenuCascadeReference( const igdeMenuCascadeReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set menu cascade without adding reference.
	 * 
	 * Use this method if the menu cascade to hold has been added a reference already. This is
	 * the case with created menu cascades as well as certain methods returning newly created
	 * menu cascades. In all these cases the menu cascade has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em menu cascade to be a NULL menu cascade.
	 */
	void TakeOver( igdeMenuCascade *menuCascade );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if menu cascade is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to menu cascade. */
	operator igdeMenuCascade*() const;
	
	/**
	 * \brief Reference to menu cascade.
	 * 
	 * \throws deeNullPointer if menu cascade is NULL.
	 */
	operator igdeMenuCascade&() const;
	
	/**
	 * \brief Pointer to menu cascade.
	 * 
	 * \throws deeNullPointer if menu cascade is NULL.
	 */
	igdeMenuCascade* operator->() const;
	
	/**
	 * \brief Store menu cascade.
	 * 
	 * If an menu cascade is already held its reference is release and the new menu cascade
	 * stored. If the new menu cascade is not NULL a reference is added.
	 */
	igdeMenuCascadeReference &operator=( igdeMenuCascade *menuCascade );
	
	/**
	 * \brief Store menu cascade.
	 * 
	 * If an menu cascade is already held its reference is release and the new menu cascade
	 * stored. If the new menu cascade is not NULL a reference is added.
	 */
	igdeMenuCascadeReference &operator=( const igdeMenuCascadeReference &reference );
	
	/** \brief Test if menu cascade is held by this holder. */
	bool operator==( igdeMenuCascade *menuCascade ) const;
	bool operator!=( igdeMenuCascade *menuCascade ) const;
	
	/** \brief Test if menu cascade is held by this holder. */
	bool operator==( const igdeMenuCascadeReference &reference ) const;
	bool operator!=( const igdeMenuCascadeReference &reference ) const;
	/*@}*/
};

#endif
