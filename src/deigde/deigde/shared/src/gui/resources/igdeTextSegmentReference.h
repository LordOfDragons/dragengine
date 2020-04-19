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

#ifndef _IGDETEXTSTYLEREFERENCE_H_
#define _IGDETEXTSTYLEREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeTextSegment;


/**
 * \brief TextSegment reference.
 * 
 * Safe way to hold an text segment reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member text segment. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class igdeTextSegmentReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty text segment reference holder. */
	igdeTextSegmentReference();
	
	/**
	 * \brief Create text segment reference holder with text segment.
	 * 
	 * Reference is added if text segment is not NULL.
	 */
	igdeTextSegmentReference( igdeTextSegment *textSegment );
	
	/**
	 * \brief Create text segment reference holder with text segment from another holder.
	 * 
	 * Reference is added if text segment in holder is not NULL.
	 */
	igdeTextSegmentReference( const igdeTextSegmentReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set text segment without adding reference.
	 * 
	 * Use this method if the text segment to hold has been added a reference already. This is
	 * the case with created text segments as well as certain methods returning newly created
	 * text segments. In all these cases the text segment has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em text segment to be a NULL text segment.
	 */
	void TakeOver( igdeTextSegment *textSegment );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if text segment is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to text segment. */
	operator igdeTextSegment*() const;
	
	/**
	 * \brief Reference to text segment.
	 * 
	 * \throws deeNullPointer if text segment is NULL.
	 */
	operator igdeTextSegment&() const;
	
	/**
	 * \brief Pointer to text segment.
	 * 
	 * \throws deeNullPointer if text segment is NULL.
	 */
	igdeTextSegment* operator->() const;
	
	/**
	 * \brief Store text segment.
	 * 
	 * If an text segment is already held its reference is release and the new text segment
	 * stored. If the new text segment is not NULL a reference is added.
	 */
	igdeTextSegmentReference &operator=( igdeTextSegment *textSegment );
	
	/**
	 * \brief Store text segment.
	 * 
	 * If an text segment is already held its reference is release and the new text segment
	 * stored. If the new text segment is not NULL a reference is added.
	 */
	igdeTextSegmentReference &operator=( const igdeTextSegmentReference &reference );
	
	/** \brief Test if text segment is held by this holder. */
	bool operator==( igdeTextSegment *textSegment ) const;
	bool operator!=( igdeTextSegment *textSegment ) const;
	
	/** \brief Test if text segment is held by this holder. */
	bool operator==( const igdeTextSegmentReference &reference ) const;
	bool operator!=( const igdeTextSegmentReference &reference ) const;
	/*@}*/
};

#endif
