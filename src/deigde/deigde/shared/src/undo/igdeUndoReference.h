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
