/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEFILEPATTERNLIST_H_
#define _GDEFILEPATTERNLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeFilePattern;



/**
 * \brief File pattern list.
 */
class gdeFilePatternList{
private:
	decObjectOrderedSet pPatterns;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file pattern list. */
	gdeFilePatternList();
	
	/** \brief Create copy of file pattern list. */
	gdeFilePatternList( const gdeFilePatternList &list );
	
	/** \brief Clean up file pattern list. */
	~gdeFilePatternList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of file patterns. */
	int GetCount() const;
	
	/** \brief Object class at position. */
	gdeFilePattern *GetAt( int index ) const;
	
	/** \brief Index of named file pattern or -1 if absent. */
	int IndexOf( gdeFilePattern *pattern ) const;
	
	/** \brief Object class is present. */
	bool Has( gdeFilePattern *pattern ) const;
	
	/** \brief Add file pattern. */
	void Add( gdeFilePattern *pattern );
	
	/** \brief Remove file pattern. */
	void Remove( gdeFilePattern *pattern );
	
	/** \brief Remove all file patterns. */
	void RemoveAll();
	
	/** \brief Set from another file pattern list. */
	gdeFilePatternList &operator=( const gdeFilePatternList &list );
	/*@}*/
};

#endif
 
