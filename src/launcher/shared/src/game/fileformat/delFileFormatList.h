/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DELFILEFORMATLIST_H_
#define _DELFILEFORMATLIST_H_

#include <dragengine/common/collection/decObjectList.h>

class delFileFormat;


/**
 * \brief File Format List.
 */
class delFileFormatList{
private:
	decObjectList pFormats;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**  \brief Create file format list. */
	delFileFormatList();
	
	/** \brief Clean up file format list. */
	~delFileFormatList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of file formats. */
	int GetCount() const;
	
	/** \brief File format at index. */
	delFileFormat *GetAt( int index ) const;
	
	/** \brief File format is present. */
	bool Has( delFileFormat *format ) const;
	
	/** \brief Index of file format or -1 if absent. */
	int IndexOf( delFileFormat *format ) const;
	
	/** \brief Add file format. */
	void Add( delFileFormat *format );
	
	/** \brief Remove file format. */
	void Remove( delFileFormat *format );
	
	/** \brief Remove all file formats. */
	void RemoveAll();
	/*@}*/
};

#endif
