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

#ifndef _IGDEFILEPATTERN_H_
#define _IGDEFILEPATTERN_H_

#include <dragengine/common/string/decString.h>



/**
 * \brief File pattern defining a display name and file pattern.
 */
class igdeFilePattern{
private:
	decString pName;
	decString pPattern;
	decString pDefExtension;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file pattern. */
	igdeFilePattern( const char *name, const char *pattern, const char *defaultExtension );
	
	/** \brief Create copy of a file pattern. */
	igdeFilePattern( const igdeFilePattern &filePattern );
	
	/** \brief Clean up file pattern. */
	~igdeFilePattern();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief File pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** \brief Default extension. */
	inline const decString &GetDefaultExtension() const{ return pDefExtension; }
	/*@}*/
};

#endif
