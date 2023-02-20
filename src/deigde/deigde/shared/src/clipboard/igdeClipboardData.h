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

#ifndef _IGDECLIPBOARDDATA_H_
#define _IGDECLIPBOARDDATA_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Clipboard data superclass for use with igdeClipboard.
 * 
 * Subclass to create data to be stored in an igdeClipboard. Each clipboard data
 * has a clipboard type name indicating what kind of data it contains. Multiple
 * clipboard data objects can be stored in a clipboard as long as they have unique
 * clipboard type names. The application uses the clipboard type name to locate
 * data it can paste.
 */
class DE_DLL_EXPORT igdeClipboardData : public deObject{
private:
	decString pTypeName;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create clipboard data. */
	igdeClipboardData( const char *typeName );
	
protected:
	/**
	 * \brief Clean up object.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeClipboardData();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Type name of clipboard data. */
	inline const decString &GetTypeName() const{ return pTypeName; }
	/*@}*/
};

#endif
