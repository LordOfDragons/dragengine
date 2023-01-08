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

#ifndef _DEOGLRENDERABLEDEPTHCUBEMAP_H_
#define _DEOGLRENDERABLEDEPTHCUBEMAP_H_

class deoglRenderThread;
class deoglCubeMap;



/**
 * @brief Renderable Depth CubeMap.
 * Stores a renderable cubemap of any kind. Renderable cubemaps can not be
 * used by more than one entity at the same time. They can also not change
 * the size of format once created. They can though be reused if the entity
 * holding the cubemap gives it up.
 */
class deoglRenderableDepthCubeMap{
private:
	int pSize;
	bool pUseFloat;
	bool pInUse;
	
	deoglCubeMap *pCubeMap;
	
	int pMemoryUsageGPU;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new renderable cubemap. */
	deoglRenderableDepthCubeMap( deoglRenderThread &renderThread, int size, bool useFloat );
	/** Cleans up the renderable cubemap. */
	~deoglRenderableDepthCubeMap();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the size. */
	inline int GetSize() const{ return pSize; }
	
	/** Use float depth. */
	inline bool GetUseFloat() const{ return pUseFloat; }
	
	/** Determines if the format matches the given format. */
	bool Matches( int size, bool useFloat ) const;
	
	/** Determines if the shadow map is in use. */
	inline bool GetInUse() const{ return pInUse; }
	/** Sets if the shadow map is in use. */
	void SetInUse( bool inUse );
	
	/** Retrieves the cubemap. */
	inline deoglCubeMap *GetCubeMap() const{ return pCubeMap; }
	
	/** Retrieves the GPU memory usage. */
	inline int GetMemoryUsageGPU() const{ return pMemoryUsageGPU; }
	/** Updates the GPU memory consumption. */
	void UpdateMemoryUsage();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
