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

#ifndef _DEOGLLIGHTBOUNDARYMAP_H_
#define _DEOGLLIGHTBOUNDARYMAP_H_

#include <dragengine/common/math/decMath.h>

class deoglFramebuffer;
class deoglRenderThread;
class deoglTexture;
class deoglPixelBuffer;



/**
 * @brief Light Boundary Map.
 * Boundary map used for light boundary box calculation from a shadow map. The boundary map
 * stores a Z-Pyramid of the minimum and maximum boundary of a shadow map. For each lod level
 * in the boundary map an own framebuffer is used.
 */
class deoglLightBoundaryMap{
private:
	deoglRenderThread &pRenderThread;
	
	deoglTexture *pTextureMin;
	deoglTexture *pTextureMax;
	deoglFramebuffer **pFBOs;
	
	deoglPixelBuffer *pPixBufBoundaryMin;
	deoglPixelBuffer *pPixBufBoundaryMax;
	
	int pSize;
	int pLevelCount;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new boundary map. */
	deoglLightBoundaryMap( deoglRenderThread &renderThread, int size );
	/** Cleans up the boundary map. */
	~deoglLightBoundaryMap();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the size of the base level. */
	inline int GetSize() const{ return pSize; }
	/** Retrieves the level count. */
	inline int GetLevelCount() const{ return pLevelCount; }
	/** Retrieves the base level for the given size. */
	int GetBaseLevel( int baseSize );
	
	/** Retrieves the minimum texture. */
	inline deoglTexture *GetTextureMin() const{ return pTextureMin; }
	/** Retrieves the maximum texture. */
	inline deoglTexture *GetTextureMax() const{ return pTextureMax; }
	/** Retrieves the fbo for a level. */
	deoglFramebuffer *GetFBOAt( int level );
	
	/** Retrieve the result. */
	void GetResult( decVector &boundaryMin, decVector &boundaryMax );
	/*@}*/
	
private:
	void pCleanUp();
	
	void pCreateTextures();
	void pCreateFBOs();
};

#endif
