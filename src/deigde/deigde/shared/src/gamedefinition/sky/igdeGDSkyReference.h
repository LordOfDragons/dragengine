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

#ifndef _IGDEGDSKYREFERENCE_H_
#define _IGDEGDSKYREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeGDSky;


/**
 * \brief GDSky reference.
 * 
 * Safe way to hold an game definition sky reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member game definition sky. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeGDSkyReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty game definition sky reference holder. */
	igdeGDSkyReference();
	
	/**
	 * \brief Create game definition sky reference holder with game definition sky.
	 * 
	 * Reference is added if game definition sky is not NULL.
	 */
	igdeGDSkyReference( igdeGDSky *gdsky );
	
	/**
	 * \brief Create game definition sky reference holder with game definition sky from another holder.
	 * 
	 * Reference is added if game definition sky in holder is not NULL.
	 */
	igdeGDSkyReference( const igdeGDSkyReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set game definition sky without adding reference.
	 * 
	 * Use this method if the game definition sky to hold has been added a reference already. This is
	 * the case with created game definition skys as well as certain methods returning newly created
	 * game definition skys. In all these cases the game definition sky has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em game definition sky to be a NULL game definition sky.
	 */
	void TakeOver( igdeGDSky *gdsky );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if game definition sky is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to game definition sky. */
	operator igdeGDSky*() const;
	
	/**
	 * \brief Reference to game definition sky.
	 * 
	 * \throws deeNullPointer if game definition sky is NULL.
	 */
	operator igdeGDSky&() const;
	
	/**
	 * \brief Pointer to game definition sky.
	 * 
	 * \throws deeNullPointer if game definition sky is NULL.
	 */
	igdeGDSky* operator->() const;
	
	/**
	 * \brief Store game definition sky.
	 * 
	 * If an game definition sky is already held its reference is release and the new game definition sky
	 * stored. If the new game definition sky is not NULL a reference is added.
	 */
	igdeGDSkyReference &operator=( igdeGDSky *gdsky );
	
	/**
	 * \brief Store game definition sky.
	 * 
	 * If an game definition sky is already held its reference is release and the new game definition sky
	 * stored. If the new game definition sky is not NULL a reference is added.
	 */
	igdeGDSkyReference &operator=( const igdeGDSkyReference &reference );
	
	/** \brief Test if game definition sky is held by this holder. */
	bool operator==( igdeGDSky *gdsky ) const;
	bool operator!=( igdeGDSky *gdsky ) const;
	
	/** \brief Test if game definition sky is held by this holder. */
	bool operator==( const igdeGDSkyReference &reference ) const;
	bool operator!=( const igdeGDSkyReference &reference ) const;
	/*@}*/
};

#endif

