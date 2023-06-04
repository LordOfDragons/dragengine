/* 
 * Drag[en]gine IGDE Animator Editor
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

#ifndef _AECLIPBOARDDATALINK_H_
#define _AECLIPBOARDDATALINK_H_

#include "../animator/link/aeLinkList.h"

#include <deigde/clipboard/igdeClipboardData.h>



/**
 * Link clipboard data.
 */
class aeClipboardDataLink : public igdeClipboardData{
public:
	/** Type name. */
	static const char * const TYPE_NAME;
	
	
	
private:
	aeLinkList pLinks;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new clipboard data. */
	aeClipboardDataLink( aeLink *link );
	
	/** Create a new clipboard data. */
	aeClipboardDataLink( const aeLinkList &links );
	
protected:
	/**
	 * Clean up object.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~aeClipboardDataLink();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Links. */
	inline const aeLinkList &GetLinks() const{ return pLinks; }
	/*@}*/
};

#endif
