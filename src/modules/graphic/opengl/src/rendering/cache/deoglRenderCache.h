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

// include only once
#ifndef _DEOGLRENDERERCACHE_H_
#define _DEOGLRENDERERCACHE_H_

// includes
#include <dragengine/common/math/decMath.h>

// predefinitions
class deoglRenderThread;
class deoglRenderCacheLight;
class deoglRLight;



/**
 * @brief Render Cache.
 *
 * Caches dynamic render objects across an entire frame update. This allows
 * to reuse dynamic render objects for multiple render world calls for the
 * current frame. At each FrameEnd call this cache is cleared.
 */
class deoglRenderCache{
private:
	deoglRenderThread &pRenderThread;
	
	deoglRenderCacheLight **pLights;
	int pLightCount;
	int pLightSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new render cache. */
	deoglRenderCache( deoglRenderThread &renderThread );
	/** Cleans up the render cache. */
	~deoglRenderCache();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Clears the cache. */
	void Clear();
	/*@}*/
	
	/** @name Lights */
	/*@{*/
	/** Retrieves the number of cache lights. */
	inline int GetLightCount() const{ return pLightCount; }
	/** Retrieves the cache light at the given index. */
	deoglRenderCacheLight *GetLightAt( int index ) const;
	/**
	 * Retrieves the cache light for the given light. If no cache exists yet a new
	 * one is created prio to returning it.
	 */
	deoglRenderCacheLight *GetLightFor( deoglRLight *light );
	/** Removes all cache lights. */
	void RemoveAllLights();
	/*@}*/
	
private:
	int pIndexOfLightWith( deoglRLight *light ) const;
};

// end of include only once
#endif
