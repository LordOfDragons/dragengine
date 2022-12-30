/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLDEBUGNAMESENUM_H_
#define _DEOGLDEBUGNAMESENUM_H_

#include <dragengine/common/string/decString.h>


/**
 * Debug names enumeration.
 */
class deoglDebugNamesEnum{
public:
	struct sEntry{
		int value;
		const char *name;
	};
	
	static const sEntry EndOfList;
	
	
	
protected:
	const char *pName;
	const sEntry *pEntries;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create debug names enumeration. */
	deoglDebugNamesEnum( const char *name, const sEntry *entries );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Name of enumeration. */
	inline const char *Name() const{ return pName; }
	
	/** Name of enumeration constant. */
	decString EntryName( int value, bool full = false ) const;
	/*@}*/
};

#endif
