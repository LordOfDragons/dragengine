/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEEDITPROPERTYVALUESREFERENCE_H_
#define _IGDEEDITPROPERTYVALUESREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeEditPropertyValue;


/**
 * \brief Edit property value reference.
 * 
 * Safe way to hold an edit property value reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member edit property value. Do not use
 *       it as tagser and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeEditPropertyValueReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty edit property value reference holder. */
	igdeEditPropertyValueReference();
	
	/**
	 * \brief Create edit property value reference holder with edit property value.
	 * 
	 * Reference is added if edit property value is not NULL.
	 */
	igdeEditPropertyValueReference( igdeEditPropertyValue *widget );
	
	/**
	 * \brief Create edit property value reference holder with edit property value from another holder.
	 * 
	 * Reference is added if edit property value in holder is not NULL.
	 */
	igdeEditPropertyValueReference( const igdeEditPropertyValueReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set edit property value without adding reference.
	 * 
	 * Use this method if the edit property value to hold has been added a reference already. This is
	 * the case with created edit property values as well as certain methods returning newly created
	 * edit property values. In all these cases the edit property value has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em edit property value to be a NULL edit property value.
	 */
	void TakeOver( igdeEditPropertyValue *widget );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if edit property value is not NULL. */
	operator bool() const;
	
	/** \brief TagLister to edit property value. */
	operator igdeEditPropertyValue*() const;
	
	/**
	 * \brief Reference to edit property value.
	 * 
	 * \throws deeNullTagLister if edit property value is NULL.
	 */
	operator igdeEditPropertyValue&() const;
	
	/**
	 * \brief TagLister to edit property value.
	 * 
	 * \throws deeNullTagLister if edit property value is NULL.
	 */
	igdeEditPropertyValue* operator->() const;
	
	/**
	 * \brief Store edit property value.
	 * 
	 * If an edit property value is already held its reference is release and the new edit property value
	 * stored. If the new edit property value is not NULL a reference is added.
	 */
	igdeEditPropertyValueReference &operator=( igdeEditPropertyValue *widget );
	
	/**
	 * \brief Store edit property value.
	 * 
	 * If an edit property value is already held its reference is release and the new edit property value
	 * stored. If the new edit property value is not NULL a reference is added.
	 */
	igdeEditPropertyValueReference &operator=( const igdeEditPropertyValueReference &reference );
	
	/** \brief Test if edit property value is held by this holder. */
	bool operator==( igdeEditPropertyValue *widget ) const;
	bool operator!=( igdeEditPropertyValue *widget ) const;
	
	/** \brief Test if edit property value is held by this holder. */
	bool operator==( const igdeEditPropertyValueReference &reference ) const;
	bool operator!=( const igdeEditPropertyValueReference &reference ) const;
	/*@}*/
};

#endif
