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

#ifndef _IGDENVLINKREFERENCE_H_
#define _IGDENVLINKREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeNVLink;


/**
 * \brief NVLink reference.
 * 
 * Safe way to hold an link reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member link. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeNVLinkReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty link reference holder. */
	igdeNVLinkReference();
	
	/**
	 * \brief Create link reference holder with link.
	 * 
	 * Reference is added if link is not NULL.
	 */
	igdeNVLinkReference( igdeNVLink *link );
	
	/**
	 * \brief Create link reference holder with link from another holder.
	 * 
	 * Reference is added if link in holder is not NULL.
	 */
	igdeNVLinkReference( const igdeNVLinkReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set link without adding reference.
	 * 
	 * Use this method if the link to hold has been added a reference already. This is
	 * the case with created links as well as certain methods returning newly created
	 * links. In all these cases the link has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em link to be a NULL link.
	 */
	void TakeOver( igdeNVLink *link );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if link is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to link. */
	operator igdeNVLink*() const;
	
	/**
	 * \brief Reference to link.
	 * 
	 * \throws deeNullPointer if link is NULL.
	 */
	operator igdeNVLink&() const;
	
	/**
	 * \brief Pointer to link.
	 * 
	 * \throws deeNullPointer if link is NULL.
	 */
	igdeNVLink* operator->() const;
	
	/**
	 * \brief Store link.
	 * 
	 * If an link is already held its reference is release and the new link
	 * stored. If the new link is not NULL a reference is added.
	 */
	igdeNVLinkReference &operator=( igdeNVLink *link );
	
	/**
	 * \brief Store link.
	 * 
	 * If an link is already held its reference is release and the new link
	 * stored. If the new link is not NULL a reference is added.
	 */
	igdeNVLinkReference &operator=( const igdeNVLinkReference &reference );
	
	/** \brief Test if link is held by this holder. */
	bool operator==( igdeNVLink *link ) const;
	bool operator!=( igdeNVLink *link ) const;
	
	/** \brief Test if link is held by this holder. */
	bool operator==( const igdeNVLinkReference &reference ) const;
	bool operator!=( const igdeNVLinkReference &reference ) const;
	/*@}*/
};

#endif
