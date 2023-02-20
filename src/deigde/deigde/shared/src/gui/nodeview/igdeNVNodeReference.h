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

#ifndef _IGDENVNODEREFERENCE_H_
#define _IGDENVNODEREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeNVNode;


/**
 * \brief NVNode reference.
 * 
 * Safe way to hold an widget reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member widget. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeNVNodeReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty widget reference holder. */
	igdeNVNodeReference();
	
	/**
	 * \brief Create widget reference holder with widget.
	 * 
	 * Reference is added if widget is not NULL.
	 */
	igdeNVNodeReference( igdeNVNode *widget );
	
	/**
	 * \brief Create widget reference holder with widget from another holder.
	 * 
	 * Reference is added if widget in holder is not NULL.
	 */
	igdeNVNodeReference( const igdeNVNodeReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set widget without adding reference.
	 * 
	 * Use this method if the widget to hold has been added a reference already. This is
	 * the case with created widgets as well as certain methods returning newly created
	 * widgets. In all these cases the widget has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em widget to be a NULL widget.
	 */
	void TakeOver( igdeNVNode *widget );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if widget is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to widget. */
	operator igdeNVNode*() const;
	
	/**
	 * \brief Reference to widget.
	 * 
	 * \throws deeNullPointer if widget is NULL.
	 */
	operator igdeNVNode&() const;
	
	/**
	 * \brief Pointer to widget.
	 * 
	 * \throws deeNullPointer if widget is NULL.
	 */
	igdeNVNode* operator->() const;
	
	/**
	 * \brief Store widget.
	 * 
	 * If an widget is already held its reference is release and the new widget
	 * stored. If the new widget is not NULL a reference is added.
	 */
	igdeNVNodeReference &operator=( igdeNVNode *widget );
	
	/**
	 * \brief Store widget.
	 * 
	 * If an widget is already held its reference is release and the new widget
	 * stored. If the new widget is not NULL a reference is added.
	 */
	igdeNVNodeReference &operator=( const igdeNVNodeReference &reference );
	
	/** \brief Test if widget is held by this holder. */
	bool operator==( igdeNVNode *widget ) const;
	bool operator!=( igdeNVNode *widget ) const;
	
	/** \brief Test if widget is held by this holder. */
	bool operator==( const igdeNVNodeReference &reference ) const;
	bool operator!=( const igdeNVNodeReference &reference ) const;
	/*@}*/
};

#endif
