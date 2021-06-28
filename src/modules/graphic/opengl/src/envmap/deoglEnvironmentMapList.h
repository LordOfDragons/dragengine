/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLENVIRONMENTMAPLIST_H_
#define _DEOGLENVIRONMENTMAPLIST_H_

#include <dragengine/common/collection/decPointerList.h>

class deoglEnvironmentMap;



/**
 * List of pointers to environment maps. The ordering of the environment map in the list is
 * not defined and can change when environment maps are removed. This allows to remove
 * elements with constant time requirements.
 */
class deoglEnvironmentMapList{
private:
	decPointerList pEnvMaps;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create environment map list. */
	deoglEnvironmentMapList();
	
	/** Clean up environment map list. */
	~deoglEnvironmentMapList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Number of environment maps. */
	int GetCount() const;
	
	/** Index of environment map or -1 if absent. */
	int IndexOf( deoglEnvironmentMap *envmap ) const;
	
	/** Environment map is present. */
	bool Has( deoglEnvironmentMap *envmap ) const;
	
	/** Environment map at index. */
	deoglEnvironmentMap *GetAt( int index ) const;
	
	/** Add environment map. */
	void Add( deoglEnvironmentMap *envmap );
	
	/**
	 * Add environment map if absent.
	 * \returns true if added.
	 */
	bool AddIfMissing( deoglEnvironmentMap *envmap );
	
	/** Remove environment map. */
	void Remove( deoglEnvironmentMap *envmap );
	
	/**
	 * Remove environment map if present.
	 * \returns true if removed.
	 */
	bool RemoveIfExisting( deoglEnvironmentMap *envmap );
	
	/** Remove environment map from the index. */
	void RemoveFrom( int index );
	
	/** Removes all environment maps. */
	void RemoveAll();
	/*@}*/
};

#endif
