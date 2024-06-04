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

#ifndef _IGDESPACERREFERENCE_H_
#define _IGDESPACERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeSpacer;


/**
 * \brief Spacer reference.
 * 
 * Safe way to hold an spacer reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member spacer. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeSpacerReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty spacer reference holder. */
	igdeSpacerReference();
	
	/**
	 * \brief Create spacer reference holder with spacer.
	 * 
	 * Reference is added if spacer is not NULL.
	 */
	igdeSpacerReference( igdeSpacer *spacer );
	
	/**
	 * \brief Create spacer reference holder with spacer from another holder.
	 * 
	 * Reference is added if spacer in holder is not NULL.
	 */
	igdeSpacerReference( const igdeSpacerReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set spacer without adding reference.
	 * 
	 * Use this method if the spacer to hold has been added a reference already. This is
	 * the case with created spacers as well as certain methods returning newly created
	 * spacers. In all these cases the spacer has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em spacer to be a NULL spacer.
	 */
	void TakeOver( igdeSpacer *spacer );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if spacer is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to spacer. */
	operator igdeSpacer*() const;
	
	/**
	 * \brief Reference to spacer.
	 * 
	 * \throws deeNullPointer if spacer is NULL.
	 */
	operator igdeSpacer&() const;
	
	/**
	 * \brief Pointer to spacer.
	 * 
	 * \throws deeNullPointer if spacer is NULL.
	 */
	igdeSpacer* operator->() const;
	
	/**
	 * \brief Store spacer.
	 * 
	 * If an spacer is already held its reference is release and the new spacer
	 * stored. If the new spacer is not NULL a reference is added.
	 */
	igdeSpacerReference &operator=( igdeSpacer *spacer );
	
	/**
	 * \brief Store spacer.
	 * 
	 * If an spacer is already held its reference is release and the new spacer
	 * stored. If the new spacer is not NULL a reference is added.
	 */
	igdeSpacerReference &operator=( const igdeSpacerReference &reference );
	
	/** \brief Test if spacer is held by this holder. */
	bool operator==( igdeSpacer *spacer ) const;
	bool operator!=( igdeSpacer *spacer ) const;
	
	/** \brief Test if spacer is held by this holder. */
	bool operator==( const igdeSpacerReference &reference ) const;
	bool operator!=( const igdeSpacerReference &reference ) const;
	/*@}*/
};

#endif
