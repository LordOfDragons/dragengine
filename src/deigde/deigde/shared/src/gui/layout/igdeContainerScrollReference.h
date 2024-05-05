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
