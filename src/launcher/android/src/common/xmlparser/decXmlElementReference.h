/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DECXMLELEMENTREFERENCE_H_
#define _DECXMLELEMENTREFERENCE_H_

#include "../../deObjectReference.h"

class decXmlElement;


/**
 * \brief XML element reference.
 * 
 * Safe way to hold an object reference with correct reference handling.
 * Storing \em NULL is allowed. Initial value is \em NULL.
 * 
 * \note This class is designed to be used as stack or member object. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class decXmlElementReference : protected deObjectReference {
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty object reference holder. */
	decXmlElementReference();
	
	/**
	 * \brief Create object reference holder with object.
	 * 
	 * Reference is added if object is not \em NULL.
	 */
	decXmlElementReference( decXmlElement *object );
	
	/**
	 * \brief Create object reference holder with object from another holder.
	 * 
	 * Reference is added if object in holder is not \em NULL.
	 */
	decXmlElementReference( const decXmlElementReference &reference );
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
	void TakeOver( decXmlElement *object );
	
	/** \brief Object is \em NULL. */
	bool operator!() const;
	
	/** \brief Test if object is not \em NULL. */
	operator bool() const;
	
	/** \brief Pointer to object. */
	operator decXmlElement*() const;
	
	/**
	 * \brief Reference to object.
	 * 
	 * \throws deeNullPointer if object is \em NULL.
	 */
	operator decXmlElement&() const;
	
	/**
	 * \brief Pointer to object.
	 * 
	 * \throws deeNullPointer if object is \em NULL.
	 */
	decXmlElement* operator->() const;
	
	/**
	 * \brief Store object.
	 * 
	 * If an object is already held its reference is release and the new object
	 * stored. If the new object is not \em NULL a reference is added.
	 */
	decXmlElementReference &operator=( decXmlElement *object );
	
	/**
	 * \brief Store object.
	 * 
	 * If an object is already held its reference is release and the new object
	 * stored. If the new object is not \em NULL a reference is added.
	 */
	decXmlElementReference &operator=( const decXmlElementReference &reference );
	
	/** \brief Test if object is held by this holder. */
	bool operator==( decXmlElement *object ) const;
	bool operator!=( decXmlElement *object ) const;
	
	/** \brief Test if object is held by this holder. */
	bool operator==( const decXmlElementReference &reference ) const;
	bool operator!=( const decXmlElementReference &reference ) const;
	/*@}*/
};

#endif
