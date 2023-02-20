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

#ifndef _IGDEFILEPATTERNLIST_H_
#define _IGDEFILEPATTERNLIST_H_

#include <dragengine/dragengine_export.h>

class igdeFilePattern;



/**
 * \brief File Pattern List.
 */
class DE_DLL_EXPORT igdeFilePatternList{
private:
	igdeFilePattern **pFilePatterns;
	int pFilePatternCount;
	int pFilePatternSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file pattern list. */
	igdeFilePatternList();
	
	/** \brief Create copy of a file pattern list. */
	igdeFilePatternList( const igdeFilePatternList &list );
	
	/** \brief Clean up file pattern list. */
	~igdeFilePatternList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of file patterns. */
	inline int GetFilePatternCount() const{ return pFilePatternCount; }
	
	/** \brief File pattern at the given position. */
	igdeFilePattern *GetFilePatternAt( int index ) const;
	
	/** \brief Index of the file pattern or -1 if not found. */
	int IndexOfFilePattern( igdeFilePattern *filePattern ) const;
	
	/** \brief Determines if a file pattern exists. */
	bool HasFilePattern( igdeFilePattern *filePattern ) const;
	
	/** \brief Adds a file pattern. */
	void AddFilePattern( igdeFilePattern *filePattern );
	
	/** \brief Removes a file pattern. */
	void RemoveFilePattern( igdeFilePattern *filePattern );
	
	/** \brief Removes all file patterns. */
	void RemoveAllFilePatterns();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set list from another list. */
	igdeFilePatternList &operator=( const igdeFilePatternList &list );
	/*@}*/
};

#endif
