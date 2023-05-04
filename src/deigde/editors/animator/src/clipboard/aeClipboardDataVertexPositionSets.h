/* 
 * Drag[en]gine IGDE Animator Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _AECLIPBOARDDATAVERTEXPOSITIONSETS_H_
#define _AECLIPBOARDDATAVERTEXPOSITIONSETS_H_

#include <deigde/clipboard/igdeClipboardData.h>

#include <dragengine/common/string/decStringSet.h>



/**
 * VertexPositionSets clipboard data.
 */
class aeClipboardDataVertexPositionSets : public igdeClipboardData{
public:
	/** Type name. */
	static const char * const TYPE_NAME;
	
	
	
private:
	decStringSet pVertexPositionSets;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new clipboard data. */
	aeClipboardDataVertexPositionSets( const decStringSet &sets );
	
protected:
	/**
	 * Clean up object.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~aeClipboardDataVertexPositionSets();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** VertexPositionSetss. */
	inline const decStringSet &GetVertexPositionSets() const{ return pVertexPositionSets; }
	/*@}*/
};

#endif
