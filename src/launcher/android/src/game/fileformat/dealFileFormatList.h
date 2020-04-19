/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DEALFILEFORMATLIST_H_
#define _DEALFILEFORMATLIST_H_

#include "../../common/collection/decObjectList.h"

class dealFileFormat;



/**
 * \brief File Format List.
 */
class dealFileFormatList{
private:
	decObjectList pFormats;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file format list. */
	dealFileFormatList();
	
	/** \brief Clean up file format list. */
	~dealFileFormatList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of file formats. */
	int GetFormatCount() const;
	
	/** \brief File format at the given position. */
	dealFileFormat *GetFormatAt( int index ) const;
	
	/** \brief File format exists. */
	bool HasFormat( dealFileFormat *format ) const;
	
	/** \brief Index of a file format or -1 if not found. */
	int IndexOfFormat( dealFileFormat *format ) const;
	
	/** \brief Add file format. */
	void AddFormat( dealFileFormat *format );
	
	/** \brief Remove file format. */
	void RemoveFormat( dealFileFormat *format );
	
	/** \brief Remove all file formats. */
	void RemoveAllFormats();
	/*@}*/
};

#endif
