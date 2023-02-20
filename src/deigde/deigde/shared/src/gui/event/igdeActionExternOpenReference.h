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

#ifndef _IGDEACTIONEXTERNOPENREFERENCE_H_
#define _IGDEACTIONEXTERNOPENREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeActionExternOpen;


/**
 * \brief ActionExternOpen reference.
 * 
 * Safe way to hold an action reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member action. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeActionExternOpenReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty action reference holder. */
	igdeActionExternOpenReference();
	
	/**
	 * \brief Create action reference holder with action.
	 * 
	 * Reference is added if action is not NULL.
	 */
	igdeActionExternOpenReference( igdeActionExternOpen *action );
	
	/**
	 * \brief Create action reference holder with action from another holder.
	 * 
	 * Reference is added if action in holder is not NULL.
	 */
	igdeActionExternOpenReference( const igdeActionExternOpenReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set action without adding reference.
	 * 
	 * Use this method if the action to hold has been added a reference already. This is
	 * the case with created actions as well as certain methods returning newly created
	 * actions. In all these cases the action has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em action to be a NULL action.
	 */
	void TakeOver( igdeActionExternOpen *action );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if action is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to action. */
	operator igdeActionExternOpen*() const;
	
	/**
	 * \brief Reference to action.
	 * 
	 * \throws deeNullPointer if action is NULL.
	 */
	operator igdeActionExternOpen&() const;
	
	/**
	 * \brief Pointer to action.
	 * 
	 * \throws deeNullPointer if action is NULL.
	 */
	igdeActionExternOpen* operator->() const;
	
	/**
	 * \brief Store action.
	 * 
	 * If an action is already held its reference is release and the new action
	 * stored. If the new action is not NULL a reference is added.
	 */
	igdeActionExternOpenReference &operator=( igdeActionExternOpen *action );
	
	/**
	 * \brief Store action.
	 * 
	 * If an action is already held its reference is release and the new action
	 * stored. If the new action is not NULL a reference is added.
	 */
	igdeActionExternOpenReference &operator=( const igdeActionExternOpenReference &reference );
	
	/** \brief Test if action is held by this holder. */
	bool operator==( igdeActionExternOpen *action ) const;
	bool operator!=( igdeActionExternOpen *action ) const;
	
	/** \brief Test if action is held by this holder. */
	bool operator==( const igdeActionExternOpenReference &reference ) const;
	bool operator!=( const igdeActionExternOpenReference &reference ) const;
	/*@}*/
};

#endif
