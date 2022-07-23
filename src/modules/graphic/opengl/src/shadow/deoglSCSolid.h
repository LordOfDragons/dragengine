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

#ifndef _DEOGLSCSOLID_H_
#define _DEOGLSCSOLID_H_

#include "../memory/consumption/deoglMemoryConsumptionGPUUse.h"

class deoglRenderThread;
class deoglArrayTexture;
class deoglTexture;
class deoglCubeMap;
class deoglRenderableDepthTexture;
class deoglRenderableDepthCubeMap;
class deoglRenderableDepthArrayTexture;



/**
 * Shadow caster solid faces.
 */
class deoglSCSolid{
private:
	deoglRenderThread &pRenderThread;
	
	deoglTexture *pStaticMap;
	deoglCubeMap *pStaticCubeMap;
	deoglArrayTexture *pStaticArrayMap;
	int pLastUseStatic;
	bool pHasStatic;
	
	deoglTexture *pDynamicMap;
	deoglCubeMap *pDynamicCubeMap;
	deoglArrayTexture *pDynamicArrayMap;
	int pLastUseDynamic;
	bool pHasDynamic;
	bool pDirtyDynamic;
	
	deoglRenderableDepthTexture *pTemporaryMap;
	deoglRenderableDepthCubeMap *pTemporaryCubeMap;
	deoglRenderableDepthArrayTexture *pTemporaryArrayMap;
	
	int pLastSizeStatic;
	int pNextSizeStatic;
	int pLastSizeDynamic;
	int pNextSizeDynamic;
	
	deoglMemoryConsumptionGPUUse pMemUseStaMap;
	deoglMemoryConsumptionGPUUse pMemUseStaCube;
	deoglMemoryConsumptionGPUUse pMemUseStaArray;
	deoglMemoryConsumptionGPUUse pMemUseDynMap;
	deoglMemoryConsumptionGPUUse pMemUseDynCube;
	deoglMemoryConsumptionGPUUse pMemUseDynArray;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create solid shadow caster. */
	deoglSCSolid( deoglRenderThread &renderThread );
	
	/** Clean up solid shadow caster. */
	~deoglSCSolid();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Static shadow map if present or NULL otherwise. */
	inline deoglTexture *GetStaticMap() const{ return pStaticMap; }
	
	/** Request static shadow map with size if absent. */
	deoglTexture *ObtainStaticMapWithSize( int size, bool withStencil, bool useFloat );
	
	/** Static shadow cube map if present or NULL otherwise. */
	inline deoglCubeMap *GetStaticCubeMap() const{ return pStaticCubeMap; }
	
	/** Request static cube map with size if absent. */
	deoglCubeMap *ObtainStaticCubeMapWithSize( int size );
	
	/** Static array shadow map if present or NULL otherwise. */
	inline deoglArrayTexture *GetStaticArrayMap() const{ return pStaticArrayMap; }
	
	/** Request static array shadow map with size if absent. */
	deoglArrayTexture *ObtainStaticArrayMapWithSize( int size, int layers, bool useFloat );
	
	/** Drop static shadow map if present. */
	void DropStatic();
	
	/** Number of frames elapsed since the last time static shadow map has been used. */
	inline int GetLastUseStatic() const{ return pLastUseStatic; }
	
	/** Increment last use static shadow map counter by one. */
	void IncrementLastUseStatic();
	
	/** Reset last use static shadow map counter. */
	void ResetLastUseStatic();
	
	
	
	/** Dynamic shadow map if present or NULL otherwise. */
	inline deoglTexture *GetDynamicMap() const{ return pDynamicMap; }
	
	/** Obtain dynamic shadow map with size if absent. */
	deoglTexture *ObtainDynamicMapWithSize( int size, bool withStencil, bool useFloat );
	
	/** Dynamic shadow cube map if present or NULL otherwise. */
	inline deoglCubeMap *GetDynamicCubeMap() const{ return pDynamicCubeMap; }
	
	/** Obtain dynamic shadow cube map with size if absent. */
	deoglCubeMap *ObtainDynamicCubeMapWithSize( int size );
	
	/** Dynamic array shadow map if present or NULL otherwise. */
	inline deoglArrayTexture *GetDynamicArrayMap() const{ return pDynamicArrayMap; }
	
	/** Obtain dynamic array shadow map with size if absent. */
	deoglArrayTexture *ObtainDynamicArrayMapWithSize( int size, int layers, bool useFloat );
	
	/** Drop dynamic shadow map if present. */
	void DropDynamic();
	
	/** Number of frames elapsed since the last time dynamic shadow map has been used. */
	inline int GetLastUseDynamic() const{ return pLastUseDynamic; }
	
	/** Increment last use dynamic shadow map counter by one. */
	void IncrementLastUseDynamic();
	
	/** Reset last use dynamic shadow map counter. */
	void ResetLastUseDynamic();
	
	/** Dynamic shadow map is dirty. */
	inline bool GetDirtyDynamic() const{ return pDirtyDynamic; }
	
	/** Set dynamic shadow map dirty. */
	void SetDirtyDynamic( bool dirty );
	
	
	
	/** Temporary shadow map if present or NULL otherwise. */
	inline deoglRenderableDepthTexture *GetTemporaryMap() const{ return pTemporaryMap; }
	
	/** Obtain temporary shadow map with size if absent. */
	deoglRenderableDepthTexture *ObtainTemporaryMapWithSize( int size, bool withStencil, bool useFloat );
	
	/** Temporary shadow cube map if present or NULL otherwise. */
	inline deoglRenderableDepthCubeMap *GetTemporaryCubeMap() const{ return pTemporaryCubeMap; }
	
	/** Obtain temporary shadow cube map with size if absent. */
	deoglRenderableDepthCubeMap *ObtainTemporaryCubeMapWithSize( int size );
	
	/** Temporary array shadow map if present or NULL otherwise. */
	inline deoglRenderableDepthArrayTexture *GetTemporaryArrayMap() const{ return pTemporaryArrayMap; }
	
	/** Obtain temporary array shadow map with size if absent. */
	deoglRenderableDepthArrayTexture *ObtainTemporaryArrayMapWithSize( int size, int layers, bool useFloat );
	
	/** Drop temporary shadow map if present. */
	void DropTemporary();
	
	
	
	/** Last frame static size or 0. */
	inline int GetLastSizeStatic() const{ return pLastSizeStatic; }
	
	/** Next frame static size or 0. */
	inline int GetNextSizeStatic() const{ return pNextSizeStatic; }
	
	/** Set next frame static size to largest value. */
	void SetLargestNextSizeStatic( int size );
	
	/** Last frame dynamic size or 0. */
	inline int GetLastSizeDynamic() const{ return pLastSizeDynamic; }
	
	/** Next frame dynamic size or 0. */
	inline int GetNextSizeDynamic() const{ return pNextSizeDynamic; }
	
	/** Set next frame dynamic size to largest value. */
	void SetLargestNextSizeDynamic( int size );
	
	
	
	/** Check if static maps have not been used recently removing them. */
	void Update();
	
	/** Shadow caster requires update. True if timers are running to drop textures. */
	bool RequiresUpdate() const;
	
	/** Drop all maps. */
	void Clear();
	/*@}*/
};

#endif
