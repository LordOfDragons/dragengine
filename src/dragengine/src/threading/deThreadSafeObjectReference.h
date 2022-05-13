/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#ifndef _DETHREADSAFEOBJECTREFERENCE_H_
#define _DETHREADSAFEOBJECTREFERENCE_H_


class deThreadSafeObject;


/**
 * \brief Object reference holder.
 * 
 * Safe way to hold an object reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member object. Do not use
 * it as pointer and memory allocate it. For this reason the destructor
 * is on purpose not virtual.
 */
class DE_DLL_EXPORT deThreadSafeObjectReference{
private:
	deThreadSafeObject *pObject;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty object reference holder. */
	deThreadSafeObjectReference();
	
	/**
	 * \brief Create object reference holder with object.
	 * 
	 * deThreadSafeObjectReference is added if object is not NULL.
	 */
	deThreadSafeObjectReference( deThreadSafeObject *object );
	
	/**
	 * \brief Create object reference holder with object from another holder.
	 * 
	 * deThreadSafeObjectReference is added if object in holder is not NULL.
	 */
	deThreadSafeObjectReference( const deThreadSafeObjectReference &reference );
	
	/**
	 * \brief Clean up object reference holder.
	 * 
	 * Releases reference if object is not NULL.
	 */
	~deThreadSafeObjectReference();
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
	void TakeOver( deThreadSafeObject *object );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if object is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to object. */
	inline operator deThreadSafeObject*() const{ return pObject; }
	
	/**
	 * \brief deThreadSafeObjectReference to object.
	 * \throws deeNullPointer if object is NULL.
	 */
	operator deThreadSafeObject&() const;
	
	/**
	 * \brief Pointer to object.
	 * \throws deeNullPointer if object is NULL.
	 */
	deThreadSafeObject* operator->() const;
	
	/**
	 * \brief Store object.
	 * 
	 * If an object is already held its reference is release and the new object
	 * stored. If the new object is not NULL a reference is added.
	 */
	deThreadSafeObjectReference &operator=( deThreadSafeObject *object );
	
	/**
	 * \brief Store object.
	 * 
	 * If an object is already held its reference is release and the new object
	 * stored. If the new object is not NULL a reference is added.
	 */
	deThreadSafeObjectReference &operator=( const deThreadSafeObjectReference &reference );
	
	/** \brief Test if object is held by this holder. */
	inline bool operator==( deThreadSafeObject *object ) const{ return pObject == object; }
	
	/** \brief Test if object is held by this holder. */
	inline bool operator==( const deThreadSafeObjectReference &reference ) const{
		return pObject == reference.pObject;
	}
	
	/** \brief Test if object is not held by this holder. */
	inline bool operator!=( deThreadSafeObject *object ) const{ return pObject != object; }
	
	/** \brief Test if object is not held by this holder. */
	inline bool operator!=( const deThreadSafeObjectReference &reference ) const{
		return pObject != reference.pObject;
	}
	/*@}*/
};

#endif
