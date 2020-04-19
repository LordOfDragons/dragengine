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
#ifndef _DEOGLRENDERCACHELIGHT_H_
#define _DEOGLRENDERCACHELIGHT_H_

// includes
#include <dragengine/common/math/decMath.h>

// predefinitions
class deoglRenderThread;
class deoglRLight;
class deoglRenderCacheLightShadow;



/**
 * @brief Render Cache Light.
 *
 * Stores a light for use in by render plans. Stores the light as well
 * as the shadow parameters including the shadow textures.
 */
class deoglRenderCacheLight{
private:
	deoglRLight *pLight;
	
	bool pPrepared;
	
	bool pUseCubeMaps;
	bool pUseEncodedDepth;
	bool pUseTransparency;
	
	int pMemoryConsumption;
	
	deoglRenderCacheLightShadow *pShadowSolid;
	deoglRenderCacheLightShadow *pShadowTransparent;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new render cache light. */
	deoglRenderCacheLight( deoglRenderThread &renderThread );
	/** Cleans up the render cache light. */
	~deoglRenderCacheLight();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the light. */
	inline deoglRLight *GetLight() const{ return pLight; }
	/** Sets the light resetting the cache. */
	void SetLight( deoglRLight *light );
	
	/** Determines if light has been already prepared. */
	inline bool GetPrepared() const{ return pPrepared; }
	/** Sets if the light has been prepared. */
	void SetPrepared( bool prepared );
	
	/** Determines if cube maps are used. */
	inline bool GetUseCubeMaps() const{ return pUseCubeMaps; }
	/** Sets if cube maps are used. */
	void SetUseCubeMaps( bool useCubeMaps );
	/** Determines if encoded depth is used. */
	inline bool GetUseEncodedDepth() const{ return pUseEncodedDepth; }
	/** Sets if encoded depth is used. */
	void SetUseEncodedDepth( bool useEncodedDepth );
	/** Determines if transparency is used. */
	inline bool GetUseTransparency() const{ return pUseTransparency; }
	/** Sets if transparency is used. */
	void SetUseTransparency( bool useTransparency );
	
	/** Retrieves the memory consumption in bytes. */
	inline int GetMemoryConsumption() const{ return pMemoryConsumption; }
	/** Sets the memory consumption in bytes. */
	void SetMemoryConsumption( int consumption );
	
	/** Retrieves the solid shadow. */
	inline deoglRenderCacheLightShadow &GetShadowSolid(){ return *pShadowSolid; }
	/** Retrieves the transparent shadow. */
	inline deoglRenderCacheLightShadow &GetShadowTransparent(){ return *pShadowTransparent; }
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
