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

#ifndef _DEVIRTUALFILESYSTEMREFERENCE_H_
#define _DEVIRTUALFILESYSTEMREFERENCE_H_

#include "../deObjectReference.h"

class deVirtualFileSystem;


/**
 * \brief Virtual file system container reference.
 * 
 * Safe way to hold an object reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member object. Do not use
 * it as pointer and memory allocate it. For this reason the destructor
 * is on purpose not virtual.
 */
class DE_DLL_EXPORT deVirtualFileSystemReference : protected deObjectReference {
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty object reference holder. */
	deVirtualFileSystemReference();
	
	/**
	 * \brief Create object reference holder with object.
	 * 
	 * Reference is added if object is not NULL.
	 */
	deVirtualFileSystemReference( deVirtualFileSystem *object );
	
	/**
	 * \brief Create object reference holder with object from another holder.
	 * 
	 * Reference is added if object in holder is not NULL.
	 */
	deVirtualFileSystemReference( const deVirtualFileSystemReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set object without adding reference.
	 * 
	 * Use this method if the object to hold has been added a reference already. This is
	 * the case with created objects as well as certain methods returning newly created
	 * objects. In all these cases the object has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em object to be a NULL object.
	 */
	void TakeOver( deVirtualFileSystem *object );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if object is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to object. */
	operator deVirtualFileSystem*() const;
	
	/**
	 * \brief Reference to object.
	 * \throws deeNullPointer if object is NULL.
	 */
	operator deVirtualFileSystem&() const;
	
	/**
	 * \brief Pointer to object.
	 * \throws deeNullPointer if object is NULL.
	 */
	deVirtualFileSystem* operator->() const;
	
	/**
	 * \brief Store object.
	 * 
	 * If an object is already held its reference is release and the new object
	 * stored. If the new object is not NULL a reference is added.
	 */
	deVirtualFileSystemReference &operator=( deVirtualFileSystem *object );
	
	/**
	 * \brief Store object.
	 * 
	 * If an object is already held its reference is release and the new object
	 * stored. If the new object is not NULL a reference is added.
	 */
	deVirtualFileSystemReference &operator=( const deVirtualFileSystemReference &reference );
	
	/** \brief Test if object is held by this holder. */
	bool operator==( deVirtualFileSystem *object ) const;
	bool operator!=( deVirtualFileSystem *object ) const;
	
	/** \brief Test if object is held by this holder. */
	bool operator==( const deVirtualFileSystemReference &reference ) const;
	bool operator!=( const deVirtualFileSystemReference &reference ) const;
	/*@}*/
};

#endif
