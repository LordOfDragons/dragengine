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

#ifndef _IGDEACTIONSELECTDIRECTORYREFERENCE_H_
#define _IGDEACTIONSELECTDIRECTORYREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeActionSelectDirectory;


/**
 * \brief ActionSelectDirectory reference.
 * 
 * Safe way to hold an action reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member action. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeActionSelectDirectoryReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty action reference holder. */
	igdeActionSelectDirectoryReference();
	
	/**
	 * \brief Create action reference holder with action.
	 * 
	 * Reference is added if action is not NULL.
	 */
	igdeActionSelectDirectoryReference( igdeActionSelectDirectory *action );
	
	/**
	 * \brief Create action reference holder with action from another holder.
	 * 
	 * Reference is added if action in holder is not NULL.
	 */
	igdeActionSelectDirectoryReference( const igdeActionSelectDirectoryReference &reference );
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
	void TakeOver( igdeActionSelectDirectory *action );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if action is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to action. */
	operator igdeActionSelectDirectory*() const;
	
	/**
	 * \brief Reference to action.
	 * 
	 * \throws deeNullPointer if action is NULL.
	 */
	operator igdeActionSelectDirectory&() const;
	
	/**
	 * \brief Pointer to action.
	 * 
	 * \throws deeNullPointer if action is NULL.
	 */
	igdeActionSelectDirectory* operator->() const;
	
	/**
	 * \brief Store action.
	 * 
	 * If an action is already held its reference is release and the new action
	 * stored. If the new action is not NULL a reference is added.
	 */
	igdeActionSelectDirectoryReference &operator=( igdeActionSelectDirectory *action );
	
	/**
	 * \brief Store action.
	 * 
	 * If an action is already held its reference is release and the new action
	 * stored. If the new action is not NULL a reference is added.
	 */
	igdeActionSelectDirectoryReference &operator=( const igdeActionSelectDirectoryReference &reference );
	
	/** \brief Test if action is held by this holder. */
	bool operator==( igdeActionSelectDirectory *action ) const;
	bool operator!=( igdeActionSelectDirectory *action ) const;
	
	/** \brief Test if action is held by this holder. */
	bool operator==( const igdeActionSelectDirectoryReference &reference ) const;
	bool operator!=( const igdeActionSelectDirectoryReference &reference ) const;
	/*@}*/
};

#endif
