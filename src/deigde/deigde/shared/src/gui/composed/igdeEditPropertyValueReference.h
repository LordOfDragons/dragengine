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
