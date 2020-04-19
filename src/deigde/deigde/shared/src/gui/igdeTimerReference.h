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
class igdeTimerReference : protected deObjectReference{
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
