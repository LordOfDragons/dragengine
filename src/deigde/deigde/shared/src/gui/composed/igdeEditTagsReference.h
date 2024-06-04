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

#ifndef _IGDEEDITTAGSREFERENCE_H_
#define _IGDEEDITTAGSREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeEditTags;


/**
 * \brief Edit tags reference.
 * 
 * Safe way to hold an edit tags reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member edit tags. Do not use
 *       it as tagser and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeEditTagsReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty edit tags reference holder. */
	igdeEditTagsReference();
	
	/**
	 * \brief Create edit tags reference holder with edit tags.
	 * 
	 * Reference is added if edit tags is not NULL.
	 */
	igdeEditTagsReference( igdeEditTags *widget );
	
	/**
	 * \brief Create edit tags reference holder with edit tags from another holder.
	 * 
	 * Reference is added if edit tags in holder is not NULL.
	 */
	igdeEditTagsReference( const igdeEditTagsReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set edit tags without adding reference.
	 * 
	 * Use this method if the edit tags to hold has been added a reference already. This is
	 * the case with created edit tagss as well as certain methods returning newly created
	 * edit tagss. In all these cases the edit tags has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em edit tags to be a NULL edit tags.
	 */
	void TakeOver( igdeEditTags *widget );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if edit tags is not NULL. */
	operator bool() const;
	
	/** \brief TagLister to edit tags. */
	operator igdeEditTags*() const;
	
	/**
	 * \brief Reference to edit tags.
	 * 
	 * \throws deeNullTagLister if edit tags is NULL.
	 */
	operator igdeEditTags&() const;
	
	/**
	 * \brief TagLister to edit tags.
	 * 
	 * \throws deeNullTagLister if edit tags is NULL.
	 */
	igdeEditTags* operator->() const;
	
	/**
	 * \brief Store edit tags.
	 * 
	 * If an edit tags is already held its reference is release and the new edit tags
	 * stored. If the new edit tags is not NULL a reference is added.
	 */
	igdeEditTagsReference &operator=( igdeEditTags *widget );
	
	/**
	 * \brief Store edit tags.
	 * 
	 * If an edit tags is already held its reference is release and the new edit tags
	 * stored. If the new edit tags is not NULL a reference is added.
	 */
	igdeEditTagsReference &operator=( const igdeEditTagsReference &reference );
	
	/** \brief Test if edit tags is held by this holder. */
	bool operator==( igdeEditTags *widget ) const;
	bool operator!=( igdeEditTags *widget ) const;
	
	/** \brief Test if edit tags is held by this holder. */
	bool operator==( const igdeEditTagsReference &reference ) const;
	bool operator!=( const igdeEditTagsReference &reference ) const;
	/*@}*/
};

#endif
