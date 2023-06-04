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

#ifndef _IGDEDIALOGMULTILINEVALUEREFERENCE_H_
#define _IGDEDIALOGMULTILINEVALUEREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeDialogMultilineValue;


/**
 * \brief DialogMultilineValue reference.
 * 
 * Safe way to hold an dialog reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member dialog. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeDialogMultilineValueReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty dialog reference holder. */
	igdeDialogMultilineValueReference();
	
	/**
	 * \brief Create dialog reference holder with dialog.
	 * 
	 * Reference is added if dialog is not NULL.
	 */
	igdeDialogMultilineValueReference( igdeDialogMultilineValue *dialog );
	
	/**
	 * \brief Create dialog reference holder with dialog from another holder.
	 * 
	 * Reference is added if dialog in holder is not NULL.
	 */
	igdeDialogMultilineValueReference( const igdeDialogMultilineValueReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set dialog without adding reference.
	 * 
	 * Use this method if the dialog to hold has been added a reference already. This is
	 * the case with created dialogs as well as certain methods returning newly created
	 * dialogs. In all these cases the dialog has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em dialog to be a NULL dialog.
	 */
	void TakeOver( igdeDialogMultilineValue *dialog );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if dialog is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to dialog. */
	operator igdeDialogMultilineValue*() const;
	
	/**
	 * \brief Reference to dialog.
	 * 
	 * \throws deeNullPointer if dialog is NULL.
	 */
	operator igdeDialogMultilineValue&() const;
	
	/**
	 * \brief Pointer to dialog.
	 * 
	 * \throws deeNullPointer if dialog is NULL.
	 */
	igdeDialogMultilineValue* operator->() const;
	
	/**
	 * \brief Store dialog.
	 * 
	 * If an dialog is already held its reference is release and the new dialog
	 * stored. If the new dialog is not NULL a reference is added.
	 */
	igdeDialogMultilineValueReference &operator=( igdeDialogMultilineValue *dialog );
	
	/**
	 * \brief Store dialog.
	 * 
	 * If an dialog is already held its reference is release and the new dialog
	 * stored. If the new dialog is not NULL a reference is added.
	 */
	igdeDialogMultilineValueReference &operator=( const igdeDialogMultilineValueReference &reference );
	
	/** \brief Test if dialog is held by this holder. */
	bool operator==( igdeDialogMultilineValue *dialog ) const;
	bool operator!=( igdeDialogMultilineValue *dialog ) const;
	
	/** \brief Test if dialog is held by this holder. */
	bool operator==( const igdeDialogMultilineValueReference &reference ) const;
	bool operator!=( const igdeDialogMultilineValueReference &reference ) const;
	/*@}*/
};

#endif
