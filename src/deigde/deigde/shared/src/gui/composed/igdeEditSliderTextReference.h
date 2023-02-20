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
