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

#ifndef _IGDETIMERREFERENCE_H_
#define _IGDETIMERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeTimer;


/**
 * \brief Timer reference.
 * 
 * Safe way to hold a timer reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member timer. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeTimerReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty timer reference holder. */
	igdeTimerReference();
	
	/**
	 * \brief Create timer reference holder with timer.
	 * 
	 * Reference is added if timer is not NULL.
	 */
	igdeTimerReference( igdeTimer *timer );
	
	/**
	 * \brief Create timer reference holder with timer from another holder.
	 * 
	 * Reference is added if timer in holder is not NULL.
	 */
	igdeTimerReference( const igdeTimerReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set timer without adding reference.
	 * 
	 * Use this method if the timer to hold has been added a reference already. This is
	 * the case with created timers as well as certain methods returning newly created
	 * timers. In all these cases the timer has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em timer to be a NULL timer.
	 */
	void TakeOver( igdeTimer *timer );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if timer is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to timer. */
	operator igdeTimer*() const;
	
	/**
	 * \brief Reference to timer.
	 * 
	 * \throws deeNullPointer if timer is NULL.
	 */
	operator igdeTimer&() const;
	
	/**
	 * \brief Pointer to timer.
	 * 
	 * \throws deeNullPointer if timer is NULL.
	 */
	igdeTimer* operator->() const;
	
	/**
	 * \brief Store timer.
	 * 
	 * If an timer is already held its reference is release and the new timer
	 * stored. If the new timer is not NULL a reference is added.
	 */
	igdeTimerReference &operator=( igdeTimer *timer );
	
	/**
	 * \brief Store timer.
	 * 
	 * If an timer is already held its reference is release and the new timer
	 * stored. If the new timer is not NULL a reference is added.
	 */
	igdeTimerReference &operator=( const igdeTimerReference &reference );
	
	/** \brief Test if timer is held by this holder. */
	bool operator==( igdeTimer *timer ) const;
	bool operator!=( igdeTimer *timer ) const;
	
	/** \brief Test if timer is held by this holder. */
	bool operator==( const igdeTimerReference &reference ) const;
	bool operator!=( const igdeTimerReference &reference ) const;
	/*@}*/
};

#endif
