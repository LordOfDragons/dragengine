/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	deTObjectReference() : pObject( nullptr ){
	}
	
	/**
	 * \brief Create object reference holder with object.
	 * 
	 * Reference is added if object is not nullptr.
	 */
	deTObjectReference( T *object ) : pObject( object ){
		if( pObject ){
			pObject->AddReference();
		}
	}
	
	/**
	 * \brief Create object reference holder with object from another holder.
	 * 
	 * Reference is added if object in holder is not nullptr.
	 */
	deTObjectReference( const deTObjectReference &reference ) : pObject( reference.pObject ){
		if( pObject ){
			pObject->AddReference();
		}
	}
	
	/**
	 * \brief Clean up object reference holder.
	 * 
	 * Releases reference if object is not nullptr.
	 */
	~deTObjectReference(){
		if( pObject ){
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
	void TakeOver( T *object ){
		if( object == pObject ){
			if( object ){
				// this is required since we are asked to take over the reference. since we
				// have the same reference already we refuse to take over the reference and
				// thus without releasing it this reference would be dangling
				object->FreeReference();
			}
			return;
		}
		
		if( pObject ){
			pObject->FreeReference();
		}
		pObject = object;
	}
	
	/**
	 * \brief Move reference.
	 */
	void TakeOver( deTObjectReference &reference ){
		if( pObject ){
			pObject->FreeReference();
		}
		
		pObject = reference.pObject;
		reference.pObject = nullptr;
	}
	
	/**
	 * \brief Add reference.
	 */
	void TakeOver( const deTObjectReference &reference ){
		*this = reference;
	}
	
	/**
	 * \brief Create instance taking over reference.
	 * 
	 * Same as calling TakeOver() on a new instance but allows for inline use.
	 */
	static deTObjectReference New( T *object ){
		deTObjectReference reference;
		reference.TakeOver( object );
		return reference;
	}
	
	/**
	 * \brief Returns reference to protect against problems.
	 */
	static deTObjectReference New( const deTObjectReference &reference ){
		return reference;
	}
	
	/**
	 * \brief Returns reference to protect against problems.
	 */
	static deTObjectReference New( deTObjectReference &reference ){
		return reference;
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
	 * \brief C to object.
	 * \throws deeNullPointer if object is nullptr.
	 */
	inline operator T&() const{
		DEASSERT_NOTNULL( pObject )
		return *pObject;
	}
	
	/**
	 * \brief Pointer to object.
	 * \throws deeNullPointer if object is nullptr.
	 */
	inline T* operator->() const{
		DEASSERT_NOTNULL( pObject )
		return pObject;
	}
	
	/**
	 * \brief Store object.
	 * 
	 * If an object is already held its reference is release and the new object
	 * stored. If the new object is not nullptr a reference is added.
	 */
	deTObjectReference &operator=( T *object ){
		if( object == pObject ){
			return *this;
		}
		
		if( pObject ){
			pObject->FreeReference();
		}
		
		pObject = object;
		
		if( object ){
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
	inline deTObjectReference &operator=( const deTObjectReference &reference ){
		return operator=( reference.pObject );
	}
	
	/** \brief Test if object is held by this holder. */
	inline bool operator==( T *object ) const{
		return pObject == object;
	}
	
	/** \brief Test if object is held by this holder. */
	inline bool operator==( const deTObjectReference &reference ) const{
		return pObject == reference.pObject;
	}
	
	/** \brief Test if object is not held by this holder. */
	inline bool operator!=( T *object ) const{
		return pObject != object;
	}
	
	/** \brief Test if object is not held by this holder. */
	inline bool operator!=( const deTObjectReference &reference ) const{
		return pObject != reference.pObject;
	}
	/*@}*/
};

#endif
