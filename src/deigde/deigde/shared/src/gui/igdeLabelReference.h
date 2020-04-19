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
class igdeLabelReference : protected deObjectReference{
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
