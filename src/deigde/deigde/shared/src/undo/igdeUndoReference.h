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

#ifndef _IGDEUNDOREFERENCE_H_
#define _IGDEUNDOREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeUndo;


/**
 * \brief Undo reference.
 * 
 * Safe way to hold an undo reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member undo. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeUndoReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty undo reference holder. */
	igdeUndoReference();
	
	/**
	 * \brief Create undo reference holder with undo.
	 * 
	 * Reference is added if undo is not NULL.
	 */
	igdeUndoReference( igdeUndo *undo );
	
	/**
	 * \brief Create undo reference holder with undo from another holder.
	 * 
	 * Reference is added if undo in holder is not NULL.
	 */
	igdeUndoReference( const igdeUndoReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set undo without adding reference.
	 * 
	 * Use this method if the undo to hold has been added a reference already. This is
	 * the case with created undos as well as certain methods returning newly created
	 * undos. In all these cases the undo has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em undo to be a NULL undo.
	 */
	void TakeOver( igdeUndo *undo );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if undo is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to undo. */
	operator igdeUndo*() const;
	
	/**
	 * \brief Reference to undo.
	 * 
	 * \throws deeNullPointer if undo is NULL.
	 */
	operator igdeUndo&() const;
	
	/**
	 * \brief Pointer to undo.
	 * 
	 * \throws deeNullPointer if undo is NULL.
	 */
	igdeUndo* operator->() const;
	
	/**
	 * \brief Store undo.
	 * 
	 * If an undo is already held its reference is release and the new undo
	 * stored. If the new undo is not NULL a reference is added.
	 */
	igdeUndoReference &operator=( igdeUndo *undo );
	
	/**
	 * \brief Store undo.
	 * 
	 * If an undo is already held its reference is release and the new undo
	 * stored. If the new undo is not NULL a reference is added.
	 */
	igdeUndoReference &operator=( const igdeUndoReference &reference );
	
	/** \brief Test if undo is held by this holder. */
	bool operator==( igdeUndo *undo ) const;
	bool operator!=( igdeUndo *undo ) const;
	
	/** \brief Test if undo is held by this holder. */
	bool operator==( const igdeUndoReference &reference ) const;
	bool operator!=( const igdeUndoReference &reference ) const;
	/*@}*/
};

#endif
