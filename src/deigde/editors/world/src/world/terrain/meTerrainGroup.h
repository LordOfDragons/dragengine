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
#ifndef _METERRAINGROUP_H_
#define _METERRAINGROUP_H_

#include <dragengine/common/string/decString.h>

// predefinitions
class meObject;


// class meTerrainGroup
class meTerrainGroup{
private:
	decString pName;
	meObject **pObjects;
	int pObjectCount, pObjectSize;
public:
	// constructor, destructor
	meTerrainGroup( const char *name );
	~meTerrainGroup();
	// management
	inline const decString &GetName() const{ return pName; }
	void SetName( const char *name );
	// object management
	inline int GetObjectCount() const{ return pObjectCount; }
	meObject *GetObject( int index ) const;
	int FindObject( meObject *object ) const;
	bool HasObject( meObject *object ) const;
	void AddObject( meObject *object );
	void RemoveObject( meObject *object );
	void RemoveAllObjects();
private:
	void pCleanUp();
};

// end of include only once
#endif
