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

#ifndef _IGDETEXTAREAREFERENCE_H_
#define _IGDETEXTAREAREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeTextArea;


/**
 * \brief TextArea reference.
 * 
 * Safe way to hold an textarea reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member textarea. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeTextAreaReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty textarea reference holder. */
	igdeTextAreaReference();
	
	/**
	 * \brief Create textarea reference holder with textarea.
	 * 
	 * Reference is added if textarea is not NULL.
	 */
	igdeTextAreaReference( igdeTextArea *textarea );
	
	/**
	 * \brief Create textarea reference holder with textarea from another holder.
	 * 
	 * Reference is added if textarea in holder is not NULL.
	 */
	igdeTextAreaReference( const igdeTextAreaReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set textarea without adding reference.
	 * 
	 * Use this method if the textarea to hold has been added a reference already. This is
	 * the case with created textareas as well as certain methods returning newly created
	 * textareas. In all these cases the textarea has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em textarea to be a NULL textarea.
	 */
	void TakeOver( igdeTextArea *textarea );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if textarea is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to textarea. */
	operator igdeTextArea*() const;
	
	/**
	 * \brief Reference to textarea.
	 * 
	 * \throws deeNullPointer if textarea is NULL.
	 */
	operator igdeTextArea&() const;
	
	/**
	 * \brief Pointer to textarea.
	 * 
	 * \throws deeNullPointer if textarea is NULL.
	 */
	igdeTextArea* operator->() const;
	
	/**
	 * \brief Store textarea.
	 * 
	 * If an textarea is already held its reference is release and the new textarea
	 * stored. If the new textarea is not NULL a reference is added.
	 */
	igdeTextAreaReference &operator=( igdeTextArea *textarea );
	
	/**
	 * \brief Store textarea.
	 * 
	 * If an textarea is already held its reference is release and the new textarea
	 * stored. If the new textarea is not NULL a reference is added.
	 */
	igdeTextAreaReference &operator=( const igdeTextAreaReference &reference );
	
	/** \brief Test if textarea is held by this holder. */
	bool operator==( igdeTextArea *textarea ) const;
	bool operator!=( igdeTextArea *textarea ) const;
	
	/** \brief Test if textarea is held by this holder. */
	bool operator==( const igdeTextAreaReference &reference ) const;
	bool operator!=( const igdeTextAreaReference &reference ) const;
	/*@}*/
};

#endif
