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

#ifndef _IGDEEDITSLIDERTEXTREFERENCE_H_
#define _IGDEEDITSLIDERTEXTREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeEditSliderText;


/**
 * \brief EditSliderText reference.
 * 
 * Safe way to hold an EditSliderText reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member EditSliderText. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeEditSliderTextReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty EditSliderText reference holder. */
	igdeEditSliderTextReference();
	
	/**
	 * \brief Create EditSliderText reference holder with EditSliderText.
	 * 
	 * Reference is added if EditSliderText is not NULL.
	 */
	igdeEditSliderTextReference( igdeEditSliderText *editSliderText );
	
	/**
	 * \brief Create EditSliderText reference holder with EditSliderText from another holder.
	 * 
	 * Reference is added if EditSliderText in holder is not NULL.
	 */
	igdeEditSliderTextReference( const igdeEditSliderTextReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set EditSliderText without adding reference.
	 * 
	 * Use this method if the EditSliderText to hold has been added a reference already. This is
	 * the case with created EditSliderTexts as well as certain methods returning newly created
	 * EditSliderTexts. In all these cases the EditSliderText has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em EditSliderText to be a NULL EditSliderText.
	 */
	void TakeOver( igdeEditSliderText *editSliderText );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if EditSliderText is not NULL. */
	operator bool() const;
	
	/** \brief SliderTexter to EditSliderText. */
	operator igdeEditSliderText*() const;
	
	/**
	 * \brief Reference to EditSliderText.
	 * 
	 * \throws deeNullSliderTexter if EditSliderText is NULL.
	 */
	operator igdeEditSliderText&() const;
	
	/**
	 * \brief SliderTexter to EditSliderText.
	 * 
	 * \throws deeNullSliderTexter if EditSliderText is NULL.
	 */
	igdeEditSliderText* operator->() const;
	
	/**
	 * \brief Store EditSliderText.
	 * 
	 * If an EditSliderText is already held its reference is release and the new EditSliderText
	 * stored. If the new EditSliderText is not NULL a reference is added.
	 */
	igdeEditSliderTextReference &operator=( igdeEditSliderText *editSliderText );
	
	/**
	 * \brief Store EditSliderText.
	 * 
	 * If an EditSliderText is already held its reference is release and the new EditSliderText
	 * stored. If the new EditSliderText is not NULL a reference is added.
	 */
	igdeEditSliderTextReference &operator=( const igdeEditSliderTextReference &reference );
	
	/** \brief Test if EditSliderText is held by this holder. */
	bool operator==( igdeEditSliderText *editSliderText ) const;
	bool operator!=( igdeEditSliderText *editSliderText ) const;
	
	/** \brief Test if EditSliderText is held by this holder. */
	bool operator==( const igdeEditSliderTextReference &reference ) const;
	bool operator!=( const igdeEditSliderTextReference &reference ) const;
	/*@}*/
};

#endif
