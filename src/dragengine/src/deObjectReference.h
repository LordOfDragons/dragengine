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

#ifndef _DEOBJECTREFERENCE_H_
#define _DEOBJECTREFERENCE_H_

#include "dragengine_export.h"


class deObject;


/**
 * \brief Object reference holder.
 * 
 * Safe way to hold an object reference with correct reference handling.
 * Storing \em NULL is allowed. Initial value is \em NULL.
 * 
 * \note This class is designed to be used as stack or member object. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT deObjectReference{
private:
	deObject *pObject;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty object reference holder. */
	deObjectReference();
	
	/**
	 * \brief Create object reference holder with object.
	 * 
	 * deObjectReference is added if object is not \em NULL.
	 */
	deObjectReference( deObject *object );
	
	/**
	 * \brief Create object reference holder with object from another holder.
	 * 
	 * deObjectReference is added if object in holder is not \em NULL.
	 */
	deObjectReference( const deObjectReference &reference );
	
	/**
	 * \brief Clean up object reference holder.
	 * 
	 * Releases reference if object is not \em NULL.
	 */
	~deObjectReference();
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
	 * It is allowed for \em object to be a \em NULL object.
	 */
	void TakeOver( deObject *object );
	
	/** \brief Object is \em NULL. */
	bool operator!() const;
	
	/** \brief Test if object is not \em NULL. */
	operator bool() const;
	
	/** \brief Pointer to object. */
	inline operator deObject*() const{ return pObject; }
	
	/**
	 * \brief deObjectReference to object.
	 * 
	 * \throws deeNullPointer if object is \em NULL.
	 */
	operator deObject&() const;
	
	/**
	 * \brief Pointer to object.
	 * 
	 * \throws deeNullPointer if object is \em NULL.
	 */
	deObject* operator->() const;
	
	/**
	 * \brief Store object.
	 * 
	 * If an object is already held its reference is release and the new object
	 * stored. If the new object is not \em NULL a reference is added.
	 */
	deObjectReference &operator=( deObject *object );
	
	/**
	 * \brief Store object.
	 * 
	 * If an object is already held its reference is release and the new object
	 * stored. If the new object is not \em NULL a reference is added.
	 */
	deObjectReference &operator=( const deObjectReference &reference );
	
	/** \brief Test if object is held by this holder. */
	inline bool operator==( deObject *object ) const{ return pObject == object; }
	
	/** \brief Test if object is held by this holder. */
	inline bool operator==( const deObjectReference &reference ) const{
		return pObject == reference.pObject;
	}
	
	/** \brief Test if object is not held by this holder. */
	inline bool operator!=( deObject *object ) const{ return pObject != object; }
	
	/** \brief Test if object is not held by this holder. */
	inline bool operator!=( const deObjectReference &reference ) const{
		return pObject != reference.pObject;
	}
	/*@}*/
};

#endif
