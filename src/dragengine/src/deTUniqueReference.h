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

#ifndef _DETUNIQUEREFERENCE_H_
#define _DETUNIQUEREFERENCE_H_

#include <type_traits>
#include <utility>

#include "common/exceptions_reduced.h"


/**
 * \brief Unique ownership object pointer.
 * \version 1.6
 *
 * Unique (move-only) smart pointer wrapper for objects of type T.
 * Owning pointer is deleted with operator delete when the wrapper is destroyed
 * or reset. Storing nullptr is allowed. Initial value is nullptr.
 *
 * \note
 * This is a move-only type. Copy constructor and copy assignment are deleted.
 * 
 * \note
 * This template requires the full type definition of T to be present at instantiation
 * because it is copy constructible. The copy constructor calls T::AddReference() which
 * requires knowing the complete type definition. This can cause issues if you try to
 * use this template with forward-declared types in header files.
 * 
 * To avoid this, you can either:
 * 1. Delete the copy constructors if not needed
 * 2. Declare copy constructors in the header and define them in source files
 * 
 * Example to avoid requiring full type definition:
 * 
 * \code{cpp}
 * // In header file:
 * class MyClass;
 *
 * class MyContainer{
 * private:
 *    deTUniqueReference<MyClass> pObject;
 * public:
 *    MyContainer();
 *    ~MyContainer();
 *    MyContainer(const MyContainer &other); // declare only
 *    MyContainer &operator=(const MyContainer &other); // declare only
 * };
 * 
 * // In source file:
 * #include "MyClass.h" // full definition here
 * 
 * MyContainer::MyContainer() = default;
 * MyContainer::~MyContainer() = default;
 * MyContainer::MyContainer(const MyContainer &other) = default;
 * MyContainer &MyContainer::operator=(const MyContainer &other) = default;
 * \endcode
 */
template<class T> class deTUniqueReference{
private:
	T *pObject;
	
	// Allow all template instantiations to access each other's private members
	template<class U> friend class deTUniqueReference;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty object reference holder. */
	deTUniqueReference() noexcept : pObject(nullptr){}
	
	/** No copy semantics for unique ownership. */
	deTUniqueReference(const deTUniqueReference&) = delete;
	deTUniqueReference& operator=(const deTUniqueReference&) = delete;
	
	/** \brief Move constructor. */
	deTUniqueReference(deTUniqueReference &&other) noexcept : pObject(other.pObject){
		other.pObject = nullptr;
	}
	
	/** \brief Move constructor. */
	template<typename U, typename = typename std::enable_if<std::is_base_of<T, U>::value>::type>
	explicit deTUniqueReference(deTUniqueReference<U> &&other) noexcept : pObject(static_cast<T*>(other.Pointer())){
		other = nullptr;
	}
	
	/**
	 * \brief Clean up object reference holder.
	 * 
	 * Releases reference if object is not nullptr.
	 */
	~deTUniqueReference(){
		Clear();
	}
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
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
	 */
	template<typename... A> static deTUniqueReference New(A&&... args){
		deTUniqueReference reference;
		reference.pObject = new T(std::forward<A>(args)...);
		return reference;
	}
	
	/** Hash. */
	inline unsigned int Hash() const{
		return pObject ? DEHash(pObject) : 0;
	}
	
	/** Clear pointer. */
	void Clear() noexcept{
		if(pObject){
			delete pObject;
		}
		pObject = nullptr;
	}
	
	/** Swap with another unique reference. */
	void Swap(deTUniqueReference &other) noexcept{
		T *tmp = pObject;
		pObject = other.pObject;
		other.pObject = tmp;
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
	
	/** \brief Move assignment operator. */
	deTUniqueReference& operator=(deTUniqueReference &&other) noexcept{
		if(this != &other){
			Clear();
			pObject = other.pObject;
			other.pObject = nullptr;
		}
		return *this;
	}
	
	/** \brief Move assignment operator. */
	template<typename U, typename = typename std::enable_if<std::is_base_of<T, U>::value>::type>
	deTUniqueReference& operator=(deTUniqueReference<U> &&other) noexcept{
		Clear();
		pObject = static_cast<T*>(other.Pointer());
		other = nullptr;
		return *this;
	}
	
	/** \brief Test if object is held by this holder. */
	inline bool operator==(T *object) const noexcept{
		return pObject == object;
	}
	
	/** \brief Test if object is held by this holder. */
	inline bool operator==(const T *object) const noexcept{
		return pObject == object;
	}
	
	/** \brief Test if object is not held by this holder. */
	inline bool operator!=(T *object) const noexcept{
		return pObject != object;
	}
	
	/**
	 * Static cast to a deTUniqueReference<U> when called on an rvalue (moves ownership).
	 */
	template<typename U, typename = typename std::enable_if<std::is_base_of<U, T>::value>::type>
	operator deTUniqueReference<U>() && {
		deTUniqueReference<U> r;
		r.pObject = static_cast<U*>(pObject);
		pObject = nullptr;
		return r;
	}
	
	/**
	 * \brief Compare with another object reference.
	 * \throws deeNullPointer if either object is nullptr.
	 * Uses DECompare on the referenced object.
	 */
	int Compare(const deTUniqueReference<T> &other) const{
		DEASSERT_NOTNULL(pObject)
		DEASSERT_NOTNULL(other.pObject)
		return DECompare(*pObject, *other.pObject);
	}
};


// avoid compile errors with reverse order checks like "a == b" if b is T::Ref
template<class T>
inline bool operator==(T *a, const deTUniqueReference<T> &b){
	return b == a;
}

template<class T>
inline bool operator==(const T *a, const deTUniqueReference<T> &b){
	return b == a;
}

template<class T>
inline bool operator!=(T *a, const deTUniqueReference<T> &b){
	return b != a;
}

template<class T>
inline bool operator!=(const T *a, const deTUniqueReference<T> &b){
	return b != a;
}

#endif
