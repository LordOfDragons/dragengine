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
class DE_DLL_EXPORT igdeNVLinkReference : protected deObjectReference{
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
