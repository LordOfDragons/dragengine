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

#ifndef _DEOBJECT_H_
#define _DEOBJECT_H_

#include "deTObjectReference.h"


/**
 * \brief Basic object interface.
 *
 * Provides the basic class for all engine objects. Each object has a
 * reference count. Objects stay alive as long as the reference count
 * is larger than 0. If you want to add a reference use the AddReference
 * function. If you want to free the reference you have added before use
 * the FreeReference function and do not call 'delete'. Upon creation
 * each object is assigned a reference count of 1 so you do not have to
 * add a refernece after creating an object.
 */
class DE_DLL_EXPORT deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deObject> Ref;
	
	
	/** \brief Weak reference data. */
	class cWeakRefData{
	private:
		deObject *pObject;
		int pRefCount;
		
		
	public:
		/** \brief Constructor. */
		explicit cWeakRefData(deObject *object);
		
		/** \brief Destructor. */
		~cWeakRefData();
		
		/** \brief Object or nullptr if deleted. */
		inline deObject *Object() const{ return pObject; }
		
		/** \brief Reference count. */
		inline int RefCount() const{ return pRefCount; }
		
		/** \brief Increase reference count by one. */
		void AddReference();
		
		/** \brief Decrease reference count by one and delete data if count reaches 0. */
		void FreeReference();
		
		friend class deObject;
	};
	
	friend class cWeakRefData;
	
	
private:
	int pRefCount;
	cWeakRefData *pWeakRefData;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object with reference count of 1. */
	deObject();
	
protected:
	/**
	 * \brief Clean up object.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deObject();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Reference count. */
	inline int GetRefCount() const{ return pRefCount; }
	
	/** \brief Add reference increasing reference count by 1. */
	void AddReference();
	
	/** \brief Decrease reference count by one and delete object if count reaches 0. */
	void FreeReference();
	
	/** \brief Add weak reference. */
	cWeakRefData *AddWeakReference();
	/*@}*/
};

#endif
