/* 
 * Drag[en]gine IGDE Skin Editor
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

#ifndef _SECLIPBOARDDATAMAPPED_H_
#define _SECLIPBOARDDATAMAPPED_H_

#include "../skin/mapped/seMappedList.h"

#include <deigde/clipboard/igdeClipboardData.h>

#include <dragengine/common/collection/decObjectList.h>


/** Clipboard data mapped. */
class seClipboardDataMapped : public igdeClipboardData{
public:
	/** Type name. */
	static const char * const TYPE_NAME;
	
	
	
private:
	seMappedList pMapped;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create clipboard data from selected mapped. */
	seClipboardDataMapped( const seMappedList &list );
	
protected:
	/**
	 * Clean up object.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~seClipboardDataMapped();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Count of mapped. */
	int GetCount() const;
	
	/** \brief Mapped at index. */
	const seMapped &GetAt( int index ) const;
	/*@}*/
};

#endif
