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
#ifndef _DEOGLRENDERCACHELIGHTSHADOW_H_
#define _DEOGLRENDERCACHELIGHTSHADOW_H_

// predefinitions
class deoglRenderThread;
class deoglTexture;
class deoglCubeMap;
class deoglRenderableColorTexture;
class deoglRenderableDepthTexture;
class deoglRenderableColorCubeMap;
class deoglRenderableDepthCubeMap;



/**
 * @brief Render Cache Light Shadow.
 *
 * Shadow for a cached light.
 */
class deoglRenderCacheLightShadow{
private:
	deoglRenderThread &pRenderThread;
	
	int pSize;
	int pMemoryConsumption;
	
	deoglRenderableDepthTexture *pMapDepth;
	deoglRenderableColorTexture *pMapDepthEncoded;
	deoglRenderableDepthCubeMap *pCubeDepth;
	deoglRenderableColorCubeMap *pCubeDepthEncoded;
	
	deoglRenderableColorTexture *pMapColor;
	deoglRenderableColorCubeMap *pCubeColor;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new render cache light shadow. */
	deoglRenderCacheLightShadow( deoglRenderThread &renderThread );
	/** Cleans up the render cache light shadow. */
	~deoglRenderCacheLightShadow();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the size or 0 if not used. */
	inline int GetSize() const{ return pSize; }
	/** Sets the size or 0 if not used. */
	void SetSize( int size );
	
	/** Retrieves the memory consumption in bytes. */
	inline int GetMemoryConsumption() const{ return pMemoryConsumption; }
	/** Sets the memory consumption in bytes. */
	void SetMemoryConsumption( int consumption );
	
	/** Retrieves the depth texture. */
	deoglTexture *GetMapDepth();
	/** Retrieves the encoded depth texture. */
	deoglTexture *GetMapDepthEncoded();
	/** Retrieves the depth cube map. */
	deoglCubeMap *GetCubeDepth();
	/** Retrieves the encoded depth cube map. */
	deoglCubeMap *GetCubeDepthEncoded();
	
	/** Retrieves the color texture. */
	deoglTexture *GetMapColor();
	/** Retrieves the color cube map. */
	deoglCubeMap *GetCubeColor();
	/*@}*/
	
private:
	void pDropTextures();
};

// end of include only once
#endif
