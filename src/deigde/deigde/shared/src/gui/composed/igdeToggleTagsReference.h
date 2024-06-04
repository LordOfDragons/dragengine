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

#ifndef _IGDETOGGLETAGSREFERENCE_H_
#define _IGDETOGGLETAGSREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeToggleTags;


/**
 * \brief Toggle tags reference.
 * 
 * Safe way to hold an toggle tags reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member toggle tags. Do not use
 *       it as tagser and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeToggleTagsReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty toggle tags reference holder. */
	igdeToggleTagsReference();
	
	/**
	 * \brief Create toggle tags reference holder with toggle tags.
	 * 
	 * Reference is added if toggle tags is not NULL.
	 */
	igdeToggleTagsReference( igdeToggleTags *widget );
	
	/**
	 * \brief Create toggle tags reference holder with toggle tags from another holder.
	 * 
	 * Reference is added if toggle tags in holder is not NULL.
	 */
	igdeToggleTagsReference( const igdeToggleTagsReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set toggle tags without adding reference.
	 * 
	 * Use this method if the toggle tags to hold has been added a reference already. This is
	 * the case with created toggle tagss as well as certain methods returning newly created
	 * toggle tagss. In all these cases the toggle tags has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em toggle tags to be a NULL toggle tags.
	 */
	void TakeOver( igdeToggleTags *widget );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if toggle tags is not NULL. */
	operator bool() const;
	
	/** \brief TagLister to toggle tags. */
	operator igdeToggleTags*() const;
	
	/**
	 * \brief Reference to toggle tags.
	 * 
	 * \throws deeNullTagLister if toggle tags is NULL.
	 */
	operator igdeToggleTags&() const;
	
	/**
	 * \brief TagLister to toggle tags.
	 * 
	 * \throws deeNullTagLister if toggle tags is NULL.
	 */
	igdeToggleTags* operator->() const;
	
	/**
	 * \brief Store toggle tags.
	 * 
	 * If an toggle tags is already held its reference is release and the new toggle tags
	 * stored. If the new toggle tags is not NULL a reference is added.
	 */
	igdeToggleTagsReference &operator=( igdeToggleTags *widget );
	
	/**
	 * \brief Store toggle tags.
	 * 
	 * If an toggle tags is already held its reference is release and the new toggle tags
	 * stored. If the new toggle tags is not NULL a reference is added.
	 */
	igdeToggleTagsReference &operator=( const igdeToggleTagsReference &reference );
	
	/** \brief Test if toggle tags is held by this holder. */
	bool operator==( igdeToggleTags *widget ) const;
	bool operator!=( igdeToggleTags *widget ) const;
	
	/** \brief Test if toggle tags is held by this holder. */
	bool operator==( const igdeToggleTagsReference &reference ) const;
	bool operator!=( const igdeToggleTagsReference &reference ) const;
	/*@}*/
};

#endif
