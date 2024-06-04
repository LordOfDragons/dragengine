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

#ifndef _IGDENVBOARDREFERENCE_H_
#define _IGDENVBOARDREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeNVBoard;


/**
 * \brief NVBoard reference.
 * 
 * Safe way to hold an widget reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member widget. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeNVBoardReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty widget reference holder. */
	igdeNVBoardReference();
	
	/**
	 * \brief Create widget reference holder with widget.
	 * 
	 * Reference is added if widget is not NULL.
	 */
	igdeNVBoardReference( igdeNVBoard *widget );
	
	/**
	 * \brief Create widget reference holder with widget from another holder.
	 * 
	 * Reference is added if widget in holder is not NULL.
	 */
	igdeNVBoardReference( const igdeNVBoardReference &reference );
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
	void TakeOver( igdeNVBoard *widget );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if widget is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to widget. */
	operator igdeNVBoard*() const;
	
	/**
	 * \brief Reference to widget.
	 * 
	 * \throws deeNullPointer if widget is NULL.
	 */
	operator igdeNVBoard&() const;
	
	/**
	 * \brief Pointer to widget.
	 * 
	 * \throws deeNullPointer if widget is NULL.
	 */
	igdeNVBoard* operator->() const;
	
	/**
	 * \brief Store widget.
	 * 
	 * If an widget is already held its reference is release and the new widget
	 * stored. If the new widget is not NULL a reference is added.
	 */
	igdeNVBoardReference &operator=( igdeNVBoard *widget );
	
	/**
	 * \brief Store widget.
	 * 
	 * If an widget is already held its reference is release and the new widget
	 * stored. If the new widget is not NULL a reference is added.
	 */
	igdeNVBoardReference &operator=( const igdeNVBoardReference &reference );
	
	/** \brief Test if widget is held by this holder. */
	bool operator==( igdeNVBoard *widget ) const;
	bool operator!=( igdeNVBoard *widget ) const;
	
	/** \brief Test if widget is held by this holder. */
	bool operator==( const igdeNVBoardReference &reference ) const;
	bool operator!=( const igdeNVBoardReference &reference ) const;
	/*@}*/
};

#endif
