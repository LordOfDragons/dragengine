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

#ifndef _DEOGLOCCLUSIONMAP_H_
#define _DEOGLOCCLUSIONMAP_H_

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglTexture;
class deoglFramebuffer;



/**
 * @brief Occlusion Map.
 * Occlusion map used for occlusion testing. The occlusion map stores a Z-Pyramid
 * of the depth in the scene used for testing. For each lod level in the occlusion
 * map an own framebuffer is used.
 */
class deoglOcclusionMap{
private:
	deoglRenderThread &pRenderThread;
	
	deoglTexture *pTexture;
	deoglFramebuffer **pFBOs;
	
	int pWidth;
	int pHeight;
	int pLevelCount;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new occlusion map. */
	deoglOcclusionMap( deoglRenderThread &renderThread, int width, int height );
	/** Cleans up the occlusion map. */
	~deoglOcclusionMap();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the width of the base level. */
	inline int GetWidth() const{ return pWidth; }
	/** Retrieves the height of the base level. */
	inline int GetHeight() const{ return pHeight; }
	/** Retrieves the level count. */
	inline int GetLevelCount() const{ return pLevelCount; }
	
	/** Retrieves the texture. */
	inline deoglTexture *GetTexture() const{ return pTexture; }
	/** Retrieves the fbo for a level. */
	deoglFramebuffer *GetFBOAt( int level );
	/*@}*/
	
private:
	void pCleanUp();
	
	void pCreateTextures();
	void pCreateFBOs();
};

#endif
