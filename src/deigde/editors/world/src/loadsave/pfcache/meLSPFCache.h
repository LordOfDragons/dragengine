/* 
 * Drag[en]gine IGDE World Editor
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

// include only once
#ifndef _MELSPFCACHE_H_
#define _MELSPFCACHE_H_

// includes
#include <dragengine/common/string/decString.h>

// predefinitions
class meHeightTerrainSector;
class decBaseFileReader;
class decBaseFileWriter;



/**
 * @brief Load/Save Prop Field Cache.
 *
 * Load/Save Prop Field Cache.
 */
class meLSPFCache{
private:
	decString pName;
	decString pPattern;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a object. */
	meLSPFCache();
	/** Cleans up the object. */
	~meLSPFCache();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Retrieves the pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** Load prop field cache from file. */
	void LoadFromFile( meHeightTerrainSector &sector, decBaseFileReader &file );
	/** Save prop field cache from file. */
	void SaveToFile( meHeightTerrainSector &sector, decBaseFileWriter &file );
	/*@}*/
};

// end of include only once
#endif
