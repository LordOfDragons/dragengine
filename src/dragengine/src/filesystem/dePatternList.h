/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEPATTERNLIST_H_
#define _DEPATTERNLIST_H_

#include "../common/string/decStringList.h"


/**
 * \brief List of file patterns.
 *
 * Manages a list of patterns used for searching files in virtual file
 * system containers. The patterns in the list are of the form '.X' where
 * X is any kind of extension including additional periods. There is no
 * '*' in front of the pattern.
 */
class DE_DLL_EXPORT dePatternList{
private:
	decStringList pPatterns;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new pattern list. */
	dePatternList();
	
	/** \brief Clean up pattern list. */
	~dePatternList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of patterns. */
	int GetPatternCount() const;
	
	/** \brief Pattern at the given position. */
	const decString &GetPatternAt( int index ) const;
	
	/** \brief Index of the pattern or -1 if not found. */
	int IndexOfPattern( const char *pattern ) const;
	
	/** \brief Determines if a pattern exists. */
	bool HasPattern( const char *pattern ) const;
	
	/** \brief Adds a pattern. */
	void AddPattern( const char *pattern );
	
	/** \brief Removes a pattern. */
	void RemovePattern( const char *pattern );
	
	/** \brief Removes all pattern. */
	void RemoveAllPatterns();
	
	/** \brief Sort patterns by length from the longest pattern to the shortest one. */
	void SortPatternByLength();
	
	/** \brief Given path matches a pattern. */
	bool PathMatches( const char *path ) const;
	
	/** \brief Index of the pattern matching the given path or -1 if none matches. */
	int IndexOfPatternMatchingPath( const char *path ) const;
	/*@}*/
};

#endif
