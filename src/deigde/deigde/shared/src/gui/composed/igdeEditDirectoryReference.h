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
class DE_DLL_EXPORT igdeEditDirectoryReference : protected deObjectReference{
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
