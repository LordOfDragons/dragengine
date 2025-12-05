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

#ifndef _DETHREADSAFEOBJECT_H_
#define _DETHREADSAFEOBJECT_H_

#include "deMutex.h"
#include "deTThreadSafeObjectReference.h"


/**
 * \brief Thread safe version of deObject.
 *
 * In contrary to deObject a deMutex is used to protect reference manipulations against multi
 * threaded use. This does not imply all methods of the object are thread safe. Subclasses
 * are encouraged to use GetMutex() to provide thread safe access to methods.
 */
class DE_DLL_EXPORT deThreadSafeObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTThreadSafeObjectReference<deThreadSafeObject> Ref;
	
	

private:
	int pRefCount;
	deMutex pMutex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object with reference count of 1. */
	deThreadSafeObject();
	
protected:
	/**
	 * \brief Clean up object.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deThreadSafeObject();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Reference count. */
	int GetRefCount();
	
	/** \brief Add reference increasing reference count by 1. */
	void AddReference();
	
	/** \brief Decrease reference count by one and delete object if count reaches 0. */
	void FreeReference();
	/*@}*/
};

#endif
