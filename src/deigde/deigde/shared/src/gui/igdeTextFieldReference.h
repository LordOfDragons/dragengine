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

#ifndef _IGDETEXTFIELDREFERENCE_H_
#define _IGDETEXTFIELDREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeTextField;


/**
 * \brief TextField reference.
 * 
 * Safe way to hold an textfield reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member textfield. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeTextFieldReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty textfield reference holder. */
	igdeTextFieldReference();
	
	/**
	 * \brief Create textfield reference holder with textfield.
	 * 
	 * Reference is added if textfield is not NULL.
	 */
	igdeTextFieldReference( igdeTextField *textfield );
	
	/**
	 * \brief Create textfield reference holder with textfield from another holder.
	 * 
	 * Reference is added if textfield in holder is not NULL.
	 */
	igdeTextFieldReference( const igdeTextFieldReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set textfield without adding reference.
	 * 
	 * Use this method if the textfield to hold has been added a reference already. This is
	 * the case with created textfields as well as certain methods returning newly created
	 * textfields. In all these cases the textfield has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em textfield to be a NULL textfield.
	 */
	void TakeOver( igdeTextField *textfield );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if textfield is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to textfield. */
	operator igdeTextField*() const;
	
	/**
	 * \brief Reference to textfield.
	 * 
	 * \throws deeNullPointer if textfield is NULL.
	 */
	operator igdeTextField&() const;
	
	/**
	 * \brief Pointer to textfield.
	 * 
	 * \throws deeNullPointer if textfield is NULL.
	 */
	igdeTextField* operator->() const;
	
	/**
	 * \brief Store textfield.
	 * 
	 * If an textfield is already held its reference is release and the new textfield
	 * stored. If the new textfield is not NULL a reference is added.
	 */
	igdeTextFieldReference &operator=( igdeTextField *textfield );
	
	/**
	 * \brief Store textfield.
	 * 
	 * If an textfield is already held its reference is release and the new textfield
	 * stored. If the new textfield is not NULL a reference is added.
	 */
	igdeTextFieldReference &operator=( const igdeTextFieldReference &reference );
	
	/** \brief Test if textfield is held by this holder. */
	bool operator==( igdeTextField *textfield ) const;
	bool operator!=( igdeTextField *textfield ) const;
	
	/** \brief Test if textfield is held by this holder. */
	bool operator==( const igdeTextFieldReference &reference ) const;
	bool operator!=( const igdeTextFieldReference &reference ) const;
	/*@}*/
};

#endif
