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

#ifndef _IGDEVIEWCURVEBEZIERREFERENCE_H_
#define _IGDEVIEWCURVEBEZIERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeViewCurveBezier;


/**
 * \brief ViewCurveBezier reference.
 * 
 * Safe way to hold an viewCurveBezier reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member viewCurveBezier. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeViewCurveBezierReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty viewCurveBezier reference holder. */
	igdeViewCurveBezierReference();
	
	/**
	 * \brief Create viewCurveBezier reference holder with viewCurveBezier.
	 * 
	 * Reference is added if viewCurveBezier is not NULL.
	 */
	igdeViewCurveBezierReference( igdeViewCurveBezier *viewCurveBezier );
	
	/**
	 * \brief Create viewCurveBezier reference holder with viewCurveBezier from another holder.
	 * 
	 * Reference is added if viewCurveBezier in holder is not NULL.
	 */
	igdeViewCurveBezierReference( const igdeViewCurveBezierReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set viewCurveBezier without adding reference.
	 * 
	 * Use this method if the viewCurveBezier to hold has been added a reference already. This is
	 * the case with created viewCurveBeziers as well as certain methods returning newly created
	 * viewCurveBeziers. In all these cases the viewCurveBezier has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em viewCurveBezier to be a NULL viewCurveBezier.
	 */
	void TakeOver( igdeViewCurveBezier *viewCurveBezier );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if viewCurveBezier is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to viewCurveBezier. */
	operator igdeViewCurveBezier*() const;
	
	/**
	 * \brief Reference to viewCurveBezier.
	 * 
	 * \throws deeNullPointer if viewCurveBezier is NULL.
	 */
	operator igdeViewCurveBezier&() const;
	
	/**
	 * \brief Pointer to viewCurveBezier.
	 * 
	 * \throws deeNullPointer if viewCurveBezier is NULL.
	 */
	igdeViewCurveBezier* operator->() const;
	
	/**
	 * \brief Store viewCurveBezier.
	 * 
	 * If an viewCurveBezier is already held its reference is release and the new viewCurveBezier
	 * stored. If the new viewCurveBezier is not NULL a reference is added.
	 */
	igdeViewCurveBezierReference &operator=( igdeViewCurveBezier *viewCurveBezier );
	
	/**
	 * \brief Store viewCurveBezier.
	 * 
	 * If an viewCurveBezier is already held its reference is release and the new viewCurveBezier
	 * stored. If the new viewCurveBezier is not NULL a reference is added.
	 */
	igdeViewCurveBezierReference &operator=( const igdeViewCurveBezierReference &reference );
	
	/** \brief Test if viewCurveBezier is held by this holder. */
	bool operator==( igdeViewCurveBezier *viewCurveBezier ) const;
	bool operator!=( igdeViewCurveBezier *viewCurveBezier ) const;
	
	/** \brief Test if viewCurveBezier is held by this holder. */
	bool operator==( const igdeViewCurveBezierReference &reference ) const;
	bool operator!=( const igdeViewCurveBezierReference &reference ) const;
	/*@}*/
};

#endif
