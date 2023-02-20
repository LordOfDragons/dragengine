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

#ifndef _IGDECONTAINERSCROLLREFERENCE_H_
#define _IGDECONTAINERSCROLLREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeContainerScroll;


/**
 * \brief ContainerScroll reference.
 * 
 * Safe way to hold an scroll container reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member scroll container. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeContainerScrollReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty scroll container reference holder. */
	igdeContainerScrollReference();
	
	/**
	 * \brief Create scroll container reference holder with scroll container.
	 * 
	 * Reference is added if scroll container is not NULL.
	 */
	igdeContainerScrollReference( igdeContainerScroll *container );
	
	/**
	 * \brief Create scroll container reference holder with scroll container from another holder.
	 * 
	 * Reference is added if scroll container in holder is not NULL.
	 */
	igdeContainerScrollReference( const igdeContainerScrollReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set scroll container without adding reference.
	 * 
	 * Use this method if the scroll container to hold has been added a reference already. This is
	 * the case with created scroll containers as well as certain methods returning newly created
	 * scroll containers. In all these cases the scroll container has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em scroll container to be a NULL scroll container.
	 */
	void TakeOver( igdeContainerScroll *container );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if scroll container is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to scroll container. */
	operator igdeContainerScroll*() const;
	
	/**
	 * \brief Reference to scroll container.
	 * 
	 * \throws deeNullPointer if scroll container is NULL.
	 */
	operator igdeContainerScroll&() const;
	
	/**
	 * \brief Pointer to scroll container.
	 * 
	 * \throws deeNullPointer if scroll container is NULL.
	 */
	igdeContainerScroll* operator->() const;
	
	/**
	 * \brief Store scroll container.
	 * 
	 * If an scroll container is already held its reference is release and the new scroll container
	 * stored. If the new scroll container is not NULL a reference is added.
	 */
	igdeContainerScrollReference &operator=( igdeContainerScroll *container );
	
	/**
	 * \brief Store scroll container.
	 * 
	 * If an scroll container is already held its reference is release and the new scroll container
	 * stored. If the new scroll container is not NULL a reference is added.
	 */
	igdeContainerScrollReference &operator=( const igdeContainerScrollReference &reference );
	
	/** \brief Test if scroll container is held by this holder. */
	bool operator==( igdeContainerScroll *container ) const;
	bool operator!=( igdeContainerScroll *container ) const;
	
	/** \brief Test if scroll container is held by this holder. */
	bool operator==( const igdeContainerScrollReference &reference ) const;
	bool operator!=( const igdeContainerScrollReference &reference ) const;
	/*@}*/
};

#endif
