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
