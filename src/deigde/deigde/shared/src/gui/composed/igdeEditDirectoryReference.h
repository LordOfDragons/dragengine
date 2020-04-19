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

#ifndef _IGDEEDITDIRECTORYREFERENCE_H_
#define _IGDEEDITDIRECTORYREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeEditDirectory;


/**
 * \brief EditDirectory reference.
 * 
 * Safe way to hold an EditDirectory reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member EditDirectory. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeEditDirectoryReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty EditDirectory reference holder. */
	igdeEditDirectoryReference();
	
	/**
	 * \brief Create EditDirectory reference holder with EditDirectory.
	 * 
	 * Reference is added if EditDirectory is not NULL.
	 */
	igdeEditDirectoryReference( igdeEditDirectory *editDirectory );
	
	/**
	 * \brief Create EditDirectory reference holder with EditDirectory from another holder.
	 * 
	 * Reference is added if EditDirectory in holder is not NULL.
	 */
	igdeEditDirectoryReference( const igdeEditDirectoryReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set EditDirectory without adding reference.
	 * 
	 * Use this method if the EditDirectory to hold has been added a reference already. This is
	 * the case with created EditDirectorys as well as certain methods returning newly created
	 * EditDirectorys. In all these cases the EditDirectory has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em EditDirectory to be a NULL EditDirectory.
	 */
	void TakeOver( igdeEditDirectory *editDirectory );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if EditDirectory is not NULL. */
	operator bool() const;
	
	/** \brief Directoryer to EditDirectory. */
	operator igdeEditDirectory*() const;
	
	/**
	 * \brief Reference to EditDirectory.
	 * 
	 * \throws deeNullDirectoryer if EditDirectory is NULL.
	 */
	operator igdeEditDirectory&() const;
	
	/**
	 * \brief Directoryer to EditDirectory.
	 * 
	 * \throws deeNullDirectoryer if EditDirectory is NULL.
	 */
	igdeEditDirectory* operator->() const;
	
	/**
	 * \brief Store EditDirectory.
	 * 
	 * If an EditDirectory is already held its reference is release and the new EditDirectory
	 * stored. If the new EditDirectory is not NULL a reference is added.
	 */
	igdeEditDirectoryReference &operator=( igdeEditDirectory *editDirectory );
	
	/**
	 * \brief Store EditDirectory.
	 * 
	 * If an EditDirectory is already held its reference is release and the new EditDirectory
	 * stored. If the new EditDirectory is not NULL a reference is added.
	 */
	igdeEditDirectoryReference &operator=( const igdeEditDirectoryReference &reference );
	
	/** \brief Test if EditDirectory is held by this holder. */
	bool operator==( igdeEditDirectory *editDirectory ) const;
	bool operator!=( igdeEditDirectory *editDirectory ) const;
	
	/** \brief Test if EditDirectory is held by this holder. */
	bool operator==( const igdeEditDirectoryReference &reference ) const;
	bool operator!=( const igdeEditDirectoryReference &reference ) const;
	/*@}*/
};

#endif
