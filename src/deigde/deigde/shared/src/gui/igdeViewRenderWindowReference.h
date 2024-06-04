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
class DE_DLL_EXPORT igdeViewRenderWindowReference : protected deObjectReference{
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
