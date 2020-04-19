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

#ifndef _IGDECONTAINERSPLITTEDREFERENCE_H_
#define _IGDECONTAINERSPLITTEDREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeContainerSplitted;


/**
 * \brief ContainerSplitted reference.
 * 
 * Safe way to hold an container reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member container. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeContainerSplittedReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty container reference holder. */
	igdeContainerSplittedReference();
	
	/**
	 * \brief Create container reference holder with container.
	 * 
	 * Reference is added if container is not NULL.
	 */
	igdeContainerSplittedReference( igdeContainerSplitted *container );
	
	/**
	 * \brief Create container reference holder with container from another holder.
	 * 
	 * Reference is added if container in holder is not NULL.
	 */
	igdeContainerSplittedReference( const igdeContainerSplittedReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set container without adding reference.
	 * 
	 * Use this method if the container to hold has been added a reference already. This is
	 * the case with created containers as well as certain methods returning newly created
	 * containers. In all these cases the container has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em container to be a NULL container.
	 */
	void TakeOver( igdeContainerSplitted *container );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if container is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to container. */
	operator igdeContainerSplitted*() const;
	
	/**
	 * \brief Reference to container.
	 * 
	 * \throws deeNullPointer if container is NULL.
	 */
	operator igdeContainerSplitted&() const;
	
	/**
	 * \brief Pointer to container.
	 * 
	 * \throws deeNullPointer if container is NULL.
	 */
	igdeContainerSplitted* operator->() const;
	
	/**
	 * \brief Store container.
	 * 
	 * If an container is already held its reference is release and the new container
	 * stored. If the new container is not NULL a reference is added.
	 */
	igdeContainerSplittedReference &operator=( igdeContainerSplitted *container );
	
	/**
	 * \brief Store container.
	 * 
	 * If an container is already held its reference is release and the new container
	 * stored. If the new container is not NULL a reference is added.
	 */
	igdeContainerSplittedReference &operator=( const igdeContainerSplittedReference &reference );
	
	/** \brief Test if container is held by this holder. */
	bool operator==( igdeContainerSplitted *container ) const;
	bool operator!=( igdeContainerSplitted *container ) const;
	
	/** \brief Test if container is held by this holder. */
	bool operator==( const igdeContainerSplittedReference &reference ) const;
	bool operator!=( const igdeContainerSplittedReference &reference ) const;
	/*@}*/
};

#endif
