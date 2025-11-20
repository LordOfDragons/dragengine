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

#ifndef _DETOBJECTREFERENCE_H_
#define _DETOBJECTREFERENCE_H_

#include "common/exceptions.h"

/**
 * \brief Template version of deObjectReference.
 * \version 1.5
 * 
 * Safe way to hold an object reference with correct reference handling.
 * Storing nullptr is allowed. Initial value is nullptr.
 * 
 * \note This class is designed to be used as stack or member object. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 * 
 * \warning Do not use template instances in public library interfaces.
 *          Template ABIs are \b not stable by design.
 */
template<class T> class deTObjectReference{
private:
	T *pObject;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty object reference holder. */
	deTObjectReference() : pObject(nullptr){
	}
	
	/**
	 * \brief Create object reference holder with object.
	 * 
	 * Reference is added if object is not nullptr.
	 */
	deTObjectReference(T *object) : pObject(object){
		if(pObject){
			pObject->AddReference();
		}
	}
	
	/**
	 * \brief Create object reference holder with object from another holder.
	 * 
	 * Reference is added if object in holder is not nullptr.
	 */
	deTObjectReference(const deTObjectReference &reference) : pObject(reference.pObject){
		if(pObject){
			pObject->AddReference();
		}
	}
	
	/**
	 * \brief Clean up object reference holder.
	 * 
	 * Releases reference if object is not nullptr.
	 */
	~deTObjectReference(){
		if(pObject){
			pObject->FreeReference();
		}
	}
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
	 * It is allowed for object to be a nullptr object.
	 */
	void TakeOver(T *object){
		if(object == pObject){
			if(object){
				// this is required since we are asked to take over the reference. since we
				// have the same reference already we refuse to take over the reference and
				// thus without releasing it this reference would be dangling
				object->FreeReference();
			}
			return;
		}
		
		if(pObject){
			pObject->FreeReference();
		}
		pObject = object;
	}
	
	/** \brief Move reference. */
	void TakeOver(deTObjectReference &reference){
		if(pObject){
			pObject->FreeReference();
		}
		
		pObject = reference.pObject;
		reference.pObject = nullptr;
	}
	
	/** \brief Take over reference. */
	void TakeOver(const deTObjectReference &reference){
		*this = reference;
	}
	
	/**
	 * \brief Create instance taking over reference.
	 * 
	 * Same as calling TakeOver() but using provided arguments with a 'new' call.
	 */
	template<typename... A> void TakeOverWith(A&&... args){
		TakeOver(new T(static_cast<A>(args)...));
	}
	
	/** \brief Pointer to object. */
	inline T* Pointer() const{
		return pObject;
	}
	
	/** \brief Reference to object. */
	inline T& Reference() const{
		DEASSERT_NOTNULL(pObject)
		return *pObject;
	}
	
	/** \brief Object is nullptr. */
	inline bool IsNull() const{
		return pObject == nullptr;
	}
	
	/** \brief Object is not nullptr. */
	inline bool IsNotNull() const{
		return pObject != nullptr;
	}
	
	/**
	 * \brief Create instance taking over reference.
	 * 
	 * Same as calling TakeOver() on a new instance but allows for inline use.
	 */
	static deTObjectReference New(T *object){
		deTObjectReference reference;
		reference.TakeOver(object);
		return reference;
	}
	
	/**
	 * \brief Returns reference to protect against problems.
	 */
	static deTObjectReference New(const deTObjectReference &reference){
		return reference;
	}
	
	/**
	 * \brief Returns reference to protect against problems.
	 */
	static deTObjectReference New(deTObjectReference &reference){
		return reference;
	}
	
	/**
	 * \brief Create instance taking over reference.
	 * 
	 * Same as calling New() but using provided arguments with a 'new' call.
	 */
	template<typename... A> static deTObjectReference NewWith(A&&... args){
		return New(new T(static_cast<A>(args)...));
	}
	
	/** \brief Object is nullptr. */
	inline bool operator!() const{
		return pObject == nullptr;
	}
	
	/** \brief Test if object is not nullptr. */
	inline operator bool() const{
		return pObject != nullptr;
	}
	
	/** \brief Pointer to object. */
	inline operator T*() const{
		return pObject;
	}
	
	/**
	 * \brief Reference to object.
	 * \throws deeNullPointer if object is nullptr.
	 */
	inline operator T&() const{
		DEASSERT_NOTNULL(pObject)
		return *pObject;
	}
	
	/**
	 * \brief Pointer to object.
	 * \throws deeNullPointer if object is nullptr.
	 */
	inline T* operator->() const{
		DEASSERT_NOTNULL(pObject)
		return pObject;
	}
	
	/**
	 * \brief Store object.
	 * 
	 * If an object is already held its reference is release and the new object
	 * stored. If the new object is not nullptr a reference is added.
	 */
	deTObjectReference &operator=(T *object){
		if(object == pObject){
			return *this;
		}
		
		if(pObject){
			pObject->FreeReference();
		}
		
		pObject = object;
		
		if(object){
			object->AddReference();
		}
		
		return *this;
	}
	
	/**
	 * \brief Store object.
	 * 
	 * If an object is already held its reference is release and the new object
	 * stored. If the new object is not nullptr a reference is added.
	 */
	inline deTObjectReference &operator=(const deTObjectReference &reference){
		return operator=(reference.pObject);
	}
	
	/** \brief Test if object is held by this holder. */
	inline bool operator==(T *object) const{
		return pObject == object;
	}
	
	/** \brief Test if object is held by this holder. */
	inline bool operator==(const deTObjectReference &reference) const{
		return pObject == reference.pObject;
	}
	
	/** \brief Test if object is not held by this holder. */
	inline bool operator!=(T *object) const{
		return pObject != object;
	}
	
	/** \brief Test if object is not held by this holder. */
	inline bool operator!=(const deTObjectReference &reference) const{
		return pObject != reference.pObject;
	}
	/*@}*/
};

#endif
