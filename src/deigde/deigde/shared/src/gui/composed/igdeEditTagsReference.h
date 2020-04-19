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

#ifndef _IGDEEDITTAGSREFERENCE_H_
#define _IGDEEDITTAGSREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeEditTags;


/**
 * \brief Edit tags reference.
 * 
 * Safe way to hold an edit tags reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member edit tags. Do not use
 *       it as tagser and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeEditTagsReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty edit tags reference holder. */
	igdeEditTagsReference();
	
	/**
	 * \brief Create edit tags reference holder with edit tags.
	 * 
	 * Reference is added if edit tags is not NULL.
	 */
	igdeEditTagsReference( igdeEditTags *widget );
	
	/**
	 * \brief Create edit tags reference holder with edit tags from another holder.
	 * 
	 * Reference is added if edit tags in holder is not NULL.
	 */
	igdeEditTagsReference( const igdeEditTagsReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set edit tags without adding reference.
	 * 
	 * Use this method if the edit tags to hold has been added a reference already. This is
	 * the case with created edit tagss as well as certain methods returning newly created
	 * edit tagss. In all these cases the edit tags has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em edit tags to be a NULL edit tags.
	 */
	void TakeOver( igdeEditTags *widget );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if edit tags is not NULL. */
	operator bool() const;
	
	/** \brief TagLister to edit tags. */
	operator igdeEditTags*() const;
	
	/**
	 * \brief Reference to edit tags.
	 * 
	 * \throws deeNullTagLister if edit tags is NULL.
	 */
	operator igdeEditTags&() const;
	
	/**
	 * \brief TagLister to edit tags.
	 * 
	 * \throws deeNullTagLister if edit tags is NULL.
	 */
	igdeEditTags* operator->() const;
	
	/**
	 * \brief Store edit tags.
	 * 
	 * If an edit tags is already held its reference is release and the new edit tags
	 * stored. If the new edit tags is not NULL a reference is added.
	 */
	igdeEditTagsReference &operator=( igdeEditTags *widget );
	
	/**
	 * \brief Store edit tags.
	 * 
	 * If an edit tags is already held its reference is release and the new edit tags
	 * stored. If the new edit tags is not NULL a reference is added.
	 */
	igdeEditTagsReference &operator=( const igdeEditTagsReference &reference );
	
	/** \brief Test if edit tags is held by this holder. */
	bool operator==( igdeEditTags *widget ) const;
	bool operator!=( igdeEditTags *widget ) const;
	
	/** \brief Test if edit tags is held by this holder. */
	bool operator==( const igdeEditTagsReference &reference ) const;
	bool operator!=( const igdeEditTagsReference &reference ) const;
	/*@}*/
};

#endif
