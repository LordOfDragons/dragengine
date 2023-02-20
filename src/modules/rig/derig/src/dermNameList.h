/* 
 * Drag[en]gine Rig Module
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

#ifndef _DERMNAMELIST_H_
#define _DERMNAMELIST_H_

#include <dragengine/common/string/decString.h>


/**
 * List of names.
 */
class dermNameList{
private:
	struct sName{
		decString name;
		int number;
	};
	
	sName *pNames;
	int pNameCount;
	int pNameSize;
	


public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new name list. */
	dermNameList();

	/** Cleans up the name list. */
	~dermNameList();
	/*@}*/
	


	/** @name Management */
	/*@{*/
	/** Retrieves the number of names. */
	inline int GetNameCount() const{ return pNameCount; }

	/** Retrieves the name at the given index. */
	const decString &GetNameAt( int index ) const;

	/** Retrieves the number of the name at the given index. */
	int GetNameNumberAt( int index ) const;

	/** Sets the number of the name at the given index. */
	void SetNameNumberAt( int index, int number );

	/** Retrieves the index of the given name or -1 if not found. */
	int IndexOfName( const char *name ) const;

	/** Determines if the given name exists. */
	bool HasName( const char *name ) const;

	/** Adds a new name. */
	int AddName( const char *name );
	/*@}*/
};

#endif
