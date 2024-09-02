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

#ifndef _IGDELABELREFERENCE_H_
#define _IGDELABELREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeLabel;


/**
 * \brief Label reference.
 * 
 * Safe way to hold an label reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member label. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeLabelReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty label reference holder. */
	igdeLabelReference();
	
	/**
	 * \brief Create label reference holder with label.
	 * 
	 * Reference is added if label is not NULL.
	 */
	igdeLabelReference( igdeLabel *label );
	
	/**
	 * \brief Create label reference holder with label from another holder.
	 * 
	 * Reference is added if label in holder is not NULL.
	 */
	igdeLabelReference( const igdeLabelReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set label without adding reference.
	 * 
	 * Use this method if the label to hold has been added a reference already. This is
	 * the case with created labels as well as certain methods returning newly created
	 * labels. In all these cases the label has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em label to be a NULL label.
	 */
	void TakeOver( igdeLabel *label );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if label is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to label. */
	operator igdeLabel*() const;
	
	/**
	 * \brief Reference to label.
	 * 
	 * \throws deeNullPointer if label is NULL.
	 */
	operator igdeLabel&() const;
	
	/**
	 * \brief Pointer to label.
	 * 
	 * \throws deeNullPointer if label is NULL.
	 */
	igdeLabel* operator->() const;
	
	/**
	 * \brief Store label.
	 * 
	 * If an label is already held its reference is release and the new label
	 * stored. If the new label is not NULL a reference is added.
	 */
	igdeLabelReference &operator=( igdeLabel *label );
	
	/**
	 * \brief Store label.
	 * 
	 * If an label is already held its reference is release and the new label
	 * stored. If the new label is not NULL a reference is added.
	 */
	igdeLabelReference &operator=( const igdeLabelReference &reference );
	
	/** \brief Test if label is held by this holder. */
	bool operator==( igdeLabel *label ) const;
	bool operator!=( igdeLabel *label ) const;
	
	/** \brief Test if label is held by this holder. */
	bool operator==( const igdeLabelReference &reference ) const;
	bool operator!=( const igdeLabelReference &reference ) const;
	/*@}*/
};

#endif
