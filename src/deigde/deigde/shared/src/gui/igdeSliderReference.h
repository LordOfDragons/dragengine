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

#ifndef _IGDESLIDERREFERENCE_H_
#define _IGDESLIDERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeSlider;


/**
 * \brief Slider reference.
 * 
 * Safe way to hold an slider reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member slider. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeSliderReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty slider reference holder. */
	igdeSliderReference();
	
	/**
	 * \brief Create slider reference holder with slider.
	 * 
	 * Reference is added if slider is not NULL.
	 */
	igdeSliderReference( igdeSlider *slider );
	
	/**
	 * \brief Create slider reference holder with slider from another holder.
	 * 
	 * Reference is added if slider in holder is not NULL.
	 */
	igdeSliderReference( const igdeSliderReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set slider without adding reference.
	 * 
	 * Use this method if the slider to hold has been added a reference already. This is
	 * the case with created sliders as well as certain methods returning newly created
	 * sliders. In all these cases the slider has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em slider to be a NULL slider.
	 */
	void TakeOver( igdeSlider *slider );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if slider is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to slider. */
	operator igdeSlider*() const;
	
	/**
	 * \brief Reference to slider.
	 * 
	 * \throws deeNullPointer if slider is NULL.
	 */
	operator igdeSlider&() const;
	
	/**
	 * \brief Pointer to slider.
	 * 
	 * \throws deeNullPointer if slider is NULL.
	 */
	igdeSlider* operator->() const;
	
	/**
	 * \brief Store slider.
	 * 
	 * If an slider is already held its reference is release and the new slider
	 * stored. If the new slider is not NULL a reference is added.
	 */
	igdeSliderReference &operator=( igdeSlider *slider );
	
	/**
	 * \brief Store slider.
	 * 
	 * If an slider is already held its reference is release and the new slider
	 * stored. If the new slider is not NULL a reference is added.
	 */
	igdeSliderReference &operator=( const igdeSliderReference &reference );
	
	/** \brief Test if slider is held by this holder. */
	bool operator==( igdeSlider *slider ) const;
	bool operator!=( igdeSlider *slider ) const;
	
	/** \brief Test if slider is held by this holder. */
	bool operator==( const igdeSliderReference &reference ) const;
	bool operator!=( const igdeSliderReference &reference ) const;
	/*@}*/
};

#endif
