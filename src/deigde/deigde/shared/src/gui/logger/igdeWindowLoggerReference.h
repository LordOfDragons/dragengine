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

#ifndef _IGDEWINDOWLOGGERREFERENCE_H_
#define _IGDEWINDOWLOGGERREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeWindowLogger;


/**
 * \brief Window logger reference.
 * 
 * Safe way to hold an window reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member window. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeWindowLoggerReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty window reference holder. */
	igdeWindowLoggerReference();
	
	/**
	 * \brief Create window reference holder with window.
	 * 
	 * Reference is added if window is not NULL.
	 */
	igdeWindowLoggerReference( igdeWindowLogger *window );
	
	/**
	 * \brief Create window reference holder with window from another holder.
	 * 
	 * Reference is added if window in holder is not NULL.
	 */
	igdeWindowLoggerReference( const igdeWindowLoggerReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set window without adding reference.
	 * 
	 * Use this method if the window to hold has been added a reference already. This is
	 * the case with created windows as well as certain methods returning newly created
	 * windows. In all these cases the window has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em window to be a NULL window.
	 */
	void TakeOver( igdeWindowLogger *window );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if window is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to window. */
	operator igdeWindowLogger*() const;
	
	/**
	 * \brief Reference to window.
	 * 
	 * \throws deeNullPointer if window is NULL.
	 */
	operator igdeWindowLogger&() const;
	
	/**
	 * \brief Pointer to window.
	 * 
	 * \throws deeNullPointer if window is NULL.
	 */
	igdeWindowLogger* operator->() const;
	
	/**
	 * \brief Store window.
	 * 
	 * If an window is already held its reference is release and the new window
	 * stored. If the new window is not NULL a reference is added.
	 */
	igdeWindowLoggerReference &operator=( igdeWindowLogger *window );
	
	/**
	 * \brief Store window.
	 * 
	 * If an window is already held its reference is release and the new window
	 * stored. If the new window is not NULL a reference is added.
	 */
	igdeWindowLoggerReference &operator=( const igdeWindowLoggerReference &reference );
	
	/** \brief Test if window is held by this holder. */
	bool operator==( igdeWindowLogger *window ) const;
	bool operator!=( igdeWindowLogger *window ) const;
	
	/** \brief Test if window is held by this holder. */
	bool operator==( const igdeWindowLoggerReference &reference ) const;
	bool operator!=( const igdeWindowLoggerReference &reference ) const;
	/*@}*/
};

#endif
