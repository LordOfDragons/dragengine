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

#ifndef _IGDEGUITHEMEPROPERTYREFERENCE_H_
#define _IGDEGUITHEMEPROPERTYREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeGuiThemeProperty;


/**
 * \brief GuiThemeProperty reference.
 * 
 * Safe way to hold an property reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member property. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeGuiThemePropertyReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty property reference holder. */
	igdeGuiThemePropertyReference();
	
	/**
	 * \brief Create property reference holder with property.
	 * 
	 * Reference is added if property is not NULL.
	 */
	igdeGuiThemePropertyReference( igdeGuiThemeProperty *property );
	
	/**
	 * \brief Create property reference holder with property from another holder.
	 * 
	 * Reference is added if property in holder is not NULL.
	 */
	igdeGuiThemePropertyReference( const igdeGuiThemePropertyReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set property without adding reference.
	 * 
	 * Use this method if the property to hold has been added a reference already. This is
	 * the case with created propertys as well as certain methods returning newly created
	 * propertys. In all these cases the property has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em property to be a NULL property.
	 */
	void TakeOver( igdeGuiThemeProperty *property );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if property is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to property. */
	operator igdeGuiThemeProperty*() const;
	
	/**
	 * \brief Reference to property.
	 * 
	 * \throws deeNullPointer if property is NULL.
	 */
	operator igdeGuiThemeProperty&() const;
	
	/**
	 * \brief Pointer to property.
	 * 
	 * \throws deeNullPointer if property is NULL.
	 */
	igdeGuiThemeProperty* operator->() const;
	
	/**
	 * \brief Store property.
	 * 
	 * If an property is already held its reference is release and the new property
	 * stored. If the new property is not NULL a reference is added.
	 */
	igdeGuiThemePropertyReference &operator=( igdeGuiThemeProperty *property );
	
	/**
	 * \brief Store property.
	 * 
	 * If an property is already held its reference is release and the new property
	 * stored. If the new property is not NULL a reference is added.
	 */
	igdeGuiThemePropertyReference &operator=( const igdeGuiThemePropertyReference &reference );
	
	/** \brief Test if property is held by this holder. */
	bool operator==( igdeGuiThemeProperty *property ) const;
	bool operator!=( igdeGuiThemeProperty *property ) const;
	
	/** \brief Test if property is held by this holder. */
	bool operator==( const igdeGuiThemePropertyReference &reference ) const;
	bool operator!=( const igdeGuiThemePropertyReference &reference ) const;
	/*@}*/
};

#endif
