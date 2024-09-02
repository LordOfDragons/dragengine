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

#ifndef _IGDEDIALOGREFERENCE_H_
#define _IGDEDIALOGREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeDialog;


/**
 * \brief Dialog reference.
 * 
 * Safe way to hold an dialog reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member dialog. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeDialogReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty dialog reference holder. */
	igdeDialogReference();
	
	/**
	 * \brief Create dialog reference holder with dialog.
	 * 
	 * Reference is added if dialog is not NULL.
	 */
	igdeDialogReference( igdeDialog *dialog );
	
	/**
	 * \brief Create dialog reference holder with dialog from another holder.
	 * 
	 * Reference is added if dialog in holder is not NULL.
	 */
	igdeDialogReference( const igdeDialogReference &reference );
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
	void TakeOver( igdeDialog *dialog );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if dialog is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to dialog. */
	operator igdeDialog*() const;
	
	/**
	 * \brief Reference to dialog.
	 * 
	 * \throws deeNullPointer if dialog is NULL.
	 */
	operator igdeDialog&() const;
	
	/**
	 * \brief Pointer to dialog.
	 * 
	 * \throws deeNullPointer if dialog is NULL.
	 */
	igdeDialog* operator->() const;
	
	/**
	 * \brief Store dialog.
	 * 
	 * If an dialog is already held its reference is release and the new dialog
	 * stored. If the new dialog is not NULL a reference is added.
	 */
	igdeDialogReference &operator=( igdeDialog *dialog );
	
	/**
	 * \brief Store dialog.
	 * 
	 * If an dialog is already held its reference is release and the new dialog
	 * stored. If the new dialog is not NULL a reference is added.
	 */
	igdeDialogReference &operator=( const igdeDialogReference &reference );
	
	/** \brief Test if dialog is held by this holder. */
	bool operator==( igdeDialog *dialog ) const;
	bool operator!=( igdeDialog *dialog ) const;
	
	/** \brief Test if dialog is held by this holder. */
	bool operator==( const igdeDialogReference &reference ) const;
	bool operator!=( const igdeDialogReference &reference ) const;
	/*@}*/
};

#endif
