/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _IGDECONTAINERREFERENCE_H_
#define _IGDECONTAINERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeContainer;


/**
 * \brief Container reference.
 * 
 * Safe way to hold an container reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member container. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeContainerReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty container reference holder. */
	igdeContainerReference();
	
	/**
	 * \brief Create container reference holder with container.
	 * 
	 * Reference is added if container is not NULL.
	 */
	igdeContainerReference( igdeContainer *container );
	
	/**
	 * \brief Create container reference holder with container from another holder.
	 * 
	 * Reference is added if container in holder is not NULL.
	 */
	igdeContainerReference( const igdeContainerReference &reference );
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
	void TakeOver( igdeContainer *container );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if container is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to container. */
	operator igdeContainer*() const;
	
	/**
	 * \brief Reference to container.
	 * 
	 * \throws deeNullPointer if container is NULL.
	 */
	operator igdeContainer&() const;
	
	/**
	 * \brief Pointer to container.
	 * 
	 * \throws deeNullPointer if container is NULL.
	 */
	igdeContainer* operator->() const;
	
	/**
	 * \brief Store container.
	 * 
	 * If an container is already held its reference is release and the new container
	 * stored. If the new container is not NULL a reference is added.
	 */
	igdeContainerReference &operator=( igdeContainer *container );
	
	/**
	 * \brief Store container.
	 * 
	 * If an container is already held its reference is release and the new container
	 * stored. If the new container is not NULL a reference is added.
	 */
	igdeContainerReference &operator=( const igdeContainerReference &reference );
	
	/** \brief Test if container is held by this holder. */
	bool operator==( igdeContainer *container ) const;
	bool operator!=( igdeContainer *container ) const;
	
	/** \brief Test if container is held by this holder. */
	bool operator==( const igdeContainerReference &reference ) const;
	bool operator!=( const igdeContainerReference &reference ) const;
	/*@}*/
};

#endif
