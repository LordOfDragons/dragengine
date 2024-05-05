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

#ifndef _IGDEEDITPOINT3REFERENCE_H_
#define _IGDEEDITPOINT3REFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeEditPoint3;


/**
 * \brief Edit point reference.
 * 
 * Safe way to hold an edit point reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member edit point. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeEditPoint3Reference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty edit point reference holder. */
	igdeEditPoint3Reference();
	
	/**
	 * \brief Create edit point reference holder with edit point.
	 * 
	 * Reference is added if edit point is not NULL.
	 */
	igdeEditPoint3Reference( igdeEditPoint3 *editPoint3 );
	
	/**
	 * \brief Create edit point reference holder with edit point from another holder.
	 * 
	 * Reference is added if edit point in holder is not NULL.
	 */
	igdeEditPoint3Reference( const igdeEditPoint3Reference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set edit point without adding reference.
	 * 
	 * Use this method if the edit point to hold has been added a reference already. This is
	 * the case with created edit points as well as certain methods returning newly created
	 * edit points. In all these cases the edit point has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em edit point to be a NULL edit point.
	 */
	void TakeOver( igdeEditPoint3 *editPoint3 );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if edit point is not NULL. */
	operator bool() const;
	
	/** \brief Point3er to edit point. */
	operator igdeEditPoint3*() const;
	
	/**
	 * \brief Reference to edit point.
	 * 
	 * \throws deeNullPoint3er if edit point is NULL.
	 */
	operator igdeEditPoint3&() const;
	
	/**
	 * \brief Point3er to edit point.
	 * 
	 * \throws deeNullPoint3er if edit point is NULL.
	 */
	igdeEditPoint3* operator->() const;
	
	/**
	 * \brief Store edit point.
	 * 
	 * If an edit point is already held its reference is release and the new edit point
	 * stored. If the new edit point is not NULL a reference is added.
	 */
	igdeEditPoint3Reference &operator=( igdeEditPoint3 *editPoint3 );
	
	/**
	 * \brief Store edit point.
	 * 
	 * If an edit point is already held its reference is release and the new edit point
	 * stored. If the new edit point is not NULL a reference is added.
	 */
	igdeEditPoint3Reference &operator=( const igdeEditPoint3Reference &reference );
	
	/** \brief Test if edit point is held by this holder. */
	bool operator==( igdeEditPoint3 *editPoint3 ) const;
	bool operator!=( igdeEditPoint3 *editPoint3 ) const;
	
	/** \brief Test if edit point is held by this holder. */
	bool operator==( const igdeEditPoint3Reference &reference ) const;
	bool operator!=( const igdeEditPoint3Reference &reference ) const;
	/*@}*/
};

#endif
