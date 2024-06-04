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

#ifndef _IGDEEDITVECTOR2REFERENCE_H_
#define _IGDEEDITVECTOR2REFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeEditVector2;


/**
 * \brief Edit vector reference.
 * 
 * Safe way to hold an edit vector reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member edit vector. Do not use
 *       it as vectorer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeEditVector2Reference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty edit vector reference holder. */
	igdeEditVector2Reference();
	
	/**
	 * \brief Create edit vector reference holder with edit vector.
	 * 
	 * Reference is added if edit vector is not NULL.
	 */
	igdeEditVector2Reference( igdeEditVector2 *editVector2 );
	
	/**
	 * \brief Create edit vector reference holder with edit vector from another holder.
	 * 
	 * Reference is added if edit vector in holder is not NULL.
	 */
	igdeEditVector2Reference( const igdeEditVector2Reference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set edit vector without adding reference.
	 * 
	 * Use this method if the edit vector to hold has been added a reference already. This is
	 * the case with created edit vectors as well as certain methods returning newly created
	 * edit vectors. In all these cases the edit vector has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em edit vector to be a NULL edit vector.
	 */
	void TakeOver( igdeEditVector2 *editVector2 );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if edit vector is not NULL. */
	operator bool() const;
	
	/** \brief Vector2er to edit vector. */
	operator igdeEditVector2*() const;
	
	/**
	 * \brief Reference to edit vector.
	 * 
	 * \throws deeNullVector2er if edit vector is NULL.
	 */
	operator igdeEditVector2&() const;
	
	/**
	 * \brief Vector2er to edit vector.
	 * 
	 * \throws deeNullVector2er if edit vector is NULL.
	 */
	igdeEditVector2* operator->() const;
	
	/**
	 * \brief Store edit vector.
	 * 
	 * If an edit vector is already held its reference is release and the new edit vector
	 * stored. If the new edit vector is not NULL a reference is added.
	 */
	igdeEditVector2Reference &operator=( igdeEditVector2 *editVector2 );
	
	/**
	 * \brief Store edit vector.
	 * 
	 * If an edit vector is already held its reference is release and the new edit vector
	 * stored. If the new edit vector is not NULL a reference is added.
	 */
	igdeEditVector2Reference &operator=( const igdeEditVector2Reference &reference );
	
	/** \brief Test if edit vector is held by this holder. */
	bool operator==( igdeEditVector2 *editVector2 ) const;
	bool operator!=( igdeEditVector2 *editVector2 ) const;
	
	/** \brief Test if edit vector is held by this holder. */
	bool operator==( const igdeEditVector2Reference &reference ) const;
	bool operator!=( const igdeEditVector2Reference &reference ) const;
	/*@}*/
};

#endif
