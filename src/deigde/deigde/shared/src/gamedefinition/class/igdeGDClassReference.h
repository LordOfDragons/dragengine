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

#ifndef _IGDEGDCLASSREFERENCE_H_
#define _IGDEGDCLASSREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeGDClass;


/**
 * \brief GDClass reference.
 * 
 * Safe way to hold an game definition class reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member game definition class. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeGDClassReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty game definition class reference holder. */
	igdeGDClassReference();
	
	/**
	 * \brief Create game definition class reference holder with game definition class.
	 * 
	 * Reference is added if game definition class is not NULL.
	 */
	igdeGDClassReference( igdeGDClass *gdclass );
	
	/**
	 * \brief Create game definition class reference holder with game definition class from another holder.
	 * 
	 * Reference is added if game definition class in holder is not NULL.
	 */
	igdeGDClassReference( const igdeGDClassReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set game definition class without adding reference.
	 * 
	 * Use this method if the game definition class to hold has been added a reference already. This is
	 * the case with created game definition classs as well as certain methods returning newly created
	 * game definition classs. In all these cases the game definition class has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em game definition class to be a NULL game definition class.
	 */
	void TakeOver( igdeGDClass *gdclass );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if game definition class is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to game definition class. */
	operator igdeGDClass*() const;
	
	/**
	 * \brief Reference to game definition class.
	 * 
	 * \throws deeNullPointer if game definition class is NULL.
	 */
	operator igdeGDClass&() const;
	
	/**
	 * \brief Pointer to game definition class.
	 * 
	 * \throws deeNullPointer if game definition class is NULL.
	 */
	igdeGDClass* operator->() const;
	
	/**
	 * \brief Store game definition class.
	 * 
	 * If an game definition class is already held its reference is release and the new game definition class
	 * stored. If the new game definition class is not NULL a reference is added.
	 */
	igdeGDClassReference &operator=( igdeGDClass *gdclass );
	
	/**
	 * \brief Store game definition class.
	 * 
	 * If an game definition class is already held its reference is release and the new game definition class
	 * stored. If the new game definition class is not NULL a reference is added.
	 */
	igdeGDClassReference &operator=( const igdeGDClassReference &reference );
	
	/** \brief Test if game definition class is held by this holder. */
	bool operator==( igdeGDClass *gdclass ) const;
	bool operator!=( igdeGDClass *gdclass ) const;
	
	/** \brief Test if game definition class is held by this holder. */
	bool operator==( const igdeGDClassReference &reference ) const;
	bool operator!=( const igdeGDClassReference &reference ) const;
	/*@}*/
};

#endif

