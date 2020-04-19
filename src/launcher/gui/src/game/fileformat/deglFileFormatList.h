/* 
 * Drag[en]gine GUI Launcher
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

#ifndef _DEGLFILEFORMATLIST_H_
#define _DEGLFILEFORMATLIST_H_

#include <dragengine/common/collection/decObjectList.h>

class deglFileFormat;



/**
 * @brief File Format List.
 */
class deglFileFormatList{
private:
	decObjectList pFormats;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new file format list. */
	deglFileFormatList();
	/** Cleans up the file format list. */
	~deglFileFormatList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of file formats. */
	int GetFormatCount() const;
	/** Retrieves the file format at the given position. */
	deglFileFormat *GetFormatAt( int index ) const;
	/** Determines if a file format exists. */
	bool HasFormat( deglFileFormat *format ) const;
	/** Retrieves the index of a file format or -1 if not found. */
	int IndexOfFormat( deglFileFormat *format ) const;
	/** Adds a file format. */
	void AddFormat( deglFileFormat *format );
	/** Removes a file format. */
	void RemoveFormat( deglFileFormat *format );
	/** Removes all file formats. */
	void RemoveAllFormats();
	/*@}*/
};

#endif // _DEGLGAME_H_
