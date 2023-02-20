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

#ifndef _IGDEGROUPBOXREFERENCE_H_
#define _IGDEGROUPBOXREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeGroupBox;


/**
 * \brief GroupBox reference.
 * 
 * Safe way to hold an groupbox reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member groupbox. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeGroupBoxReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty groupbox reference holder. */
	igdeGroupBoxReference();
	
	/**
	 * \brief Create groupbox reference holder with groupbox.
	 * 
	 * Reference is added if groupbox is not NULL.
	 */
	igdeGroupBoxReference( igdeGroupBox *groupbox );
	
	/**
	 * \brief Create groupbox reference holder with groupbox from another holder.
	 * 
	 * Reference is added if groupbox in holder is not NULL.
	 */
	igdeGroupBoxReference( const igdeGroupBoxReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set groupbox without adding reference.
	 * 
	 * Use this method if the groupbox to hold has been added a reference already. This is
	 * the case with created groupboxs as well as certain methods returning newly created
	 * groupboxs. In all these cases the groupbox has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em groupbox to be a NULL groupbox.
	 */
	void TakeOver( igdeGroupBox *groupbox );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if groupbox is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to groupbox. */
	operator igdeGroupBox*() const;
	
	/**
	 * \brief Reference to groupbox.
	 * 
	 * \throws deeNullPointer if groupbox is NULL.
	 */
	operator igdeGroupBox&() const;
	
	/**
	 * \brief Pointer to groupbox.
	 * 
	 * \throws deeNullPointer if groupbox is NULL.
	 */
	igdeGroupBox* operator->() const;
	
	/**
	 * \brief Store groupbox.
	 * 
	 * If an groupbox is already held its reference is release and the new groupbox
	 * stored. If the new groupbox is not NULL a reference is added.
	 */
	igdeGroupBoxReference &operator=( igdeGroupBox *groupbox );
	
	/**
	 * \brief Store groupbox.
	 * 
	 * If an groupbox is already held its reference is release and the new groupbox
	 * stored. If the new groupbox is not NULL a reference is added.
	 */
	igdeGroupBoxReference &operator=( const igdeGroupBoxReference &reference );
	
	/** \brief Test if groupbox is held by this holder. */
	bool operator==( igdeGroupBox *groupbox ) const;
	bool operator!=( igdeGroupBox *groupbox ) const;
	
	/** \brief Test if groupbox is held by this holder. */
	bool operator==( const igdeGroupBoxReference &reference ) const;
	bool operator!=( const igdeGroupBoxReference &reference ) const;
	/*@}*/
};

#endif
