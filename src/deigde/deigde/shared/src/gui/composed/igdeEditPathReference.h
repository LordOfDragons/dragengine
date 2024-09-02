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

#ifndef _IGDEEDITPATHREFERENCE_H_
#define _IGDEEDITPATHREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeEditPath;


/**
 * \brief EditPath reference.
 * 
 * Safe way to hold an EditPath reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member EditPath. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeEditPathReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty EditPath reference holder. */
	igdeEditPathReference();
	
	/**
	 * \brief Create EditPath reference holder with EditPath.
	 * 
	 * Reference is added if EditPath is not NULL.
	 */
	igdeEditPathReference( igdeEditPath *editPath );
	
	/**
	 * \brief Create EditPath reference holder with EditPath from another holder.
	 * 
	 * Reference is added if EditPath in holder is not NULL.
	 */
	igdeEditPathReference( const igdeEditPathReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set EditPath without adding reference.
	 * 
	 * Use this method if the EditPath to hold has been added a reference already. This is
	 * the case with created EditPaths as well as certain methods returning newly created
	 * EditPaths. In all these cases the EditPath has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em EditPath to be a NULL EditPath.
	 */
	void TakeOver( igdeEditPath *editPath );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if EditPath is not NULL. */
	operator bool() const;
	
	/** \brief Pather to EditPath. */
	operator igdeEditPath*() const;
	
	/**
	 * \brief Reference to EditPath.
	 * 
	 * \throws deeNullPather if EditPath is NULL.
	 */
	operator igdeEditPath&() const;
	
	/**
	 * \brief Pather to EditPath.
	 * 
	 * \throws deeNullPather if EditPath is NULL.
	 */
	igdeEditPath* operator->() const;
	
	/**
	 * \brief Store EditPath.
	 * 
	 * If an EditPath is already held its reference is release and the new EditPath
	 * stored. If the new EditPath is not NULL a reference is added.
	 */
	igdeEditPathReference &operator=( igdeEditPath *editPath );
	
	/**
	 * \brief Store EditPath.
	 * 
	 * If an EditPath is already held its reference is release and the new EditPath
	 * stored. If the new EditPath is not NULL a reference is added.
	 */
	igdeEditPathReference &operator=( const igdeEditPathReference &reference );
	
	/** \brief Test if EditPath is held by this holder. */
	bool operator==( igdeEditPath *editPath ) const;
	bool operator!=( igdeEditPath *editPath ) const;
	
	/** \brief Test if EditPath is held by this holder. */
	bool operator==( const igdeEditPathReference &reference ) const;
	bool operator!=( const igdeEditPathReference &reference ) const;
	/*@}*/
};

#endif
