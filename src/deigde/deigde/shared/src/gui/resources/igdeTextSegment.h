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

#ifndef _IGDETEXTSEGMENT_H_
#define _IGDETEXTSEGMENT_H_

#include "../event/igdeActionReference.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>


/**
 * \brief IGDE UI Text Segment.
 * 
 * Text segments define a style and optionally an action for a text segment defined
 * by a  start and end offset. The end offset is inclusive hence the length of the
 * segment is "end - start + 1". Thus text segments can not be empty.
 */
class igdeTextSegment : public deObject{
private:
	int pBegin;
	int pEnd;
	decString pStyle;
	igdeActionReference pAction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create text segment. */
	igdeTextSegment( int begin, int end );
	
	igdeTextSegment( int begin, int end, const char *style );
	
	igdeTextSegment( int begin, int end, const char *style, igdeAction *action );
	
	
	
protected:
	/**
	 * \brief Clean up text segment.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeTextSegment();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Offset to begin of text segment. */
	inline int GetBegin() const{ return pBegin; }
	
	/** \brief Offset to end of text segment. */
	inline int GetEnd() const{ return pEnd; }
	
	/** \brief Length of text segment. */
	inline int GetLength() const{ return pEnd - pBegin + 1; }
	
	/** \brief Style. */
	inline const decString &GetStyle() const{ return pStyle; }
	
	/** \brief Action or NULL if not used. */
	inline igdeAction *GetAction() const{ return pAction; }
	/*@}*/
};

#endif
