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

#ifndef _DETWEAKOBJECTREFERENCE_H_
#define _DETWEAKOBJECTREFERENCE_H_

#include <type_traits>

#include "deObject.h"


/**
 * \brief Weak object reference.
 */
template<class T> class deTWeakObjectReference{
private:
	typename deObject::cWeakRefData *pReference;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty weak object reference. */
	deTWeakObjectReference() : pReference(nullptr){
	}
	
	/** \brief Create weak object reference or nullptr. */
	explicit deTWeakObjectReference(T *object) : pReference(nullptr){
		if(object){
			pReference = object->AddWeakReference();
		}
	}
	
	/** \brief Copy constructor. */
	deTWeakObjectReference(const deTWeakObjectReference &reference) : pReference(reference.pReference){
		if(pReference){
			pReference->AddReference();
		}
	}
	
	/** \brief Create up casting weak object reference. */
	template<typename U, typename = typename std::enable_if<std::is_base_of<T, U>::value>::type>
	explicit deTWeakObjectReference(const deTWeakObjectReference<U> &reference) : pReference(nullptr){
		if(reference.pReference){
			operator=(static_cast<T*>(reference.Pointer()));
		}
	}
	
	/** \brief Move constructor. */
	deTWeakObjectReference(deTWeakObjectReference &&reference) : pReference(reference.pReference){
		reference.pReference = nullptr;
	}
	
	/** \brief Move constructor. */
	template<typename U, typename = typename std::enable_if<std::is_base_of<T, U>::value>::type>
	explicit deTWeakObjectReference(deTWeakObjectReference<U> &&reference) : pReference(nullptr){
		if(reference.pReference){
			operator=(static_cast<T*>(reference.Pointer()));
			reference = nullptr;
		}
	}
	
	/** \brief Clean up weak weak reference. */
	~deTWeakObjectReference(){
		if(pReference){
			pReference->FreeReference();
		}
	}
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Pointer to object or nullptr if weak object reference is nullptr or object has been deleted. */
	inline T *Pointer() const{
		return pReference ? static_cast<T*>(pReference->Object()) : nullptr;
	}
	
	/**
	 * \brief Reference to object.
	 * \throws deeNullPointer if IsNull() returns true.
	 */
	inline T& Reference() const{
		DEASSERT_NOTNULL(pReference)
		DEASSERT_NOTNULL(pReference->Object())
		return *static_cast<T*>(pReference->Object());
	}
	
	/** \brief Weak object reference is nullptr or object has been deleted. */
	inline bool IsNull() const{
		return pReference == nullptr || pReference->Object() == nullptr;
	}
	
	/** \brief Weak object reference is not null and object has not been deleted. */
	inline bool IsNotNull() const{
		return pReference != nullptr && pReference->Object() != nullptr;
	}
	
	/** \brief Bool test. */
	inline bool operator!() const{
		return IsNull();
	}
	
	/** \brief Bool test. */
	inline operator bool() const{
		return IsNotNull();
	}
	
	/** \brief Pointer conversion. */
	inline operator T*() const{
		return Pointer();
	}
	
	/**
	 * \brief Reference to object.
	 * \throws deeNullPointer if IsNull() is true.
	 */
	inline operator T&() const{
		return Reference();
	}
	
	/**
	 * \brief Pointer access.
	 * \throws deeNullPointer if IsNull() returns true.
	 */
	inline T* operator->() const{
		DEASSERT_NOTNULL(pReference)
		DEASSERT_NOTNULL(pReference->Object())
		return static_cast<T*>(pReference->Object());
	}
	
	/** \brief Assign object. */
	deTWeakObjectReference &operator=(T *object){
		if(Pointer() == object){
			return *this;
		}
		
		if(pReference){
			pReference->FreeReference();
		}
		
		pReference = nullptr;
		
		if(object){
			pReference = object->AddWeakReference();
		}
		
		return *this;
	}
	
	/** \brief Assign weak reference. */
	inline deTWeakObjectReference &operator=(const deTWeakObjectReference &reference){
		return operator=(reference.pReference);
	}
	
	/** \brief Store object. */
	template<typename U, typename = typename std::enable_if<std::is_base_of<T, U>::value>::type>
	inline deTWeakObjectReference &operator=(const deTWeakObjectReference<U> &reference){
		return operator=(static_cast<T*>(reference.Pointer()));
	}
	
	/** \brief Test equality with object. */
	inline bool operator==(const T *object) const{
		return Pointer() == object;
	}
	
	/** \brief Test equality with another weak reference */
	inline bool operator==(const deTWeakObjectReference &reference) const{
		return pReference == reference.pReference;
	}
	
	/** \brief Test if object is held by this holder. */
	template<typename U, typename = typename std::enable_if<std::is_base_of<T, U>::value>::type>
	inline bool operator==(const deTWeakObjectReference<U> &reference) const{
		return Pointer() == static_cast<T*>(reference.Pointer());
	}
	
	/** \brief Test inequality with object. */
	inline bool operator!=(const T *object) const{
		return Pointer() != object;
	}
	
	/** \brief Test inequality with another weak reference. */
	inline bool operator!=(const deTWeakObjectReference &reference) const{
		return pReference != reference.pReference;
	}
	
	/** \brief Auto-cast to super class (static cast). */
	template<typename U, typename = typename std::enable_if<std::is_base_of<U, T>::value>::type>
	operator deTWeakObjectReference<U>() const{
		return deTWeakObjectReference<U>(static_cast<U*>(Pointer()));
	}
	
	/** \brief Static cast to super class. */
	template<typename U> deTWeakObjectReference<U> StaticCast() const{
		return deTWeakObjectReference<U>(static_cast<U*>(Pointer()));
	}
	
	/**
	 * \brief Dynamic cast to sub class.
	 * 
	 * Can be nullptr if target class is not a sub class.
	 */
	template<typename U> deTWeakObjectReference<U> DynamicCast() const{
		return deTWeakObjectReference<U>(dynamic_cast<U*>(Pointer()));
	}
	/*@}*/
};


// avoid compile errors with reverse order checks like "a == b" if b is T::Ref
template<class T>
inline bool operator==(T *a, const deTWeakObjectReference<T> &b){
	return b == a;
}

template<class T>
inline bool operator!=(T *a, const deTWeakObjectReference<T> &b){
	return b != a;
}

#endif
