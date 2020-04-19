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

#ifndef _IGDEVIEWRENDERWINDOWREFERENCE_H_
#define _IGDEVIEWRENDERWINDOWREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeViewRenderWindow;


/**
 * \brief ViewRenderWindow reference.
 * 
 * Safe way to hold an view reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member view. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeViewRenderWindowReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty view reference holder. */
	igdeViewRenderWindowReference();
	
	/**
	 * \brief Create view reference holder with view.
	 * 
	 * Reference is added if view is not NULL.
	 */
	igdeViewRenderWindowReference( igdeViewRenderWindow *view );
	
	/**
	 * \brief Create view reference holder with view from another holder.
	 * 
	 * Reference is added if view in holder is not NULL.
	 */
	igdeViewRenderWindowReference( const igdeViewRenderWindowReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set view without adding reference.
	 * 
	 * Use this method if the view to hold has been added a reference already. This is
	 * the case with created views as well as certain methods returning newly created
	 * views. In all these cases the view has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em view to be a NULL view.
	 */
	void TakeOver( igdeViewRenderWindow *view );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if view is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to view. */
	operator igdeViewRenderWindow*() const;
	
	/**
	 * \brief Reference to view.
	 * 
	 * \throws deeNullPointer if view is NULL.
	 */
	operator igdeViewRenderWindow&() const;
	
	/**
	 * \brief Pointer to view.
	 * 
	 * \throws deeNullPointer if view is NULL.
	 */
	igdeViewRenderWindow* operator->() const;
	
	/**
	 * \brief Store view.
	 * 
	 * If an view is already held its reference is release and the new view
	 * stored. If the new view is not NULL a reference is added.
	 */
	igdeViewRenderWindowReference &operator=( igdeViewRenderWindow *view );
	
	/**
	 * \brief Store view.
	 * 
	 * If an view is already held its reference is release and the new view
	 * stored. If the new view is not NULL a reference is added.
	 */
	igdeViewRenderWindowReference &operator=( const igdeViewRenderWindowReference &reference );
	
	/** \brief Test if view is held by this holder. */
	bool operator==( igdeViewRenderWindow *view ) const;
	bool operator!=( igdeViewRenderWindow *view ) const;
	
	/** \brief Test if view is held by this holder. */
	bool operator==( const igdeViewRenderWindowReference &reference ) const;
	bool operator!=( const igdeViewRenderWindowReference &reference ) const;
	/*@}*/
};

#endif
