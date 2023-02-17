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

#ifndef _DEOGLRWDEPTHMINMAX_H_
#define _DEOGLRWDEPTHMINMAX_H_

class deoglRenderThread;
class deoglArrayTexture;
class deoglFramebuffer;



/**
 * Deferred Rendering Depth Min-Max.
 * Stores a mip-mapped view of the minimum and maximum depth values inside a depth texture.
 * The maximum number of mipmap levels is limited. For each lod level an own framebuffer
 * is used.
 */
class deoglDRDepthMinMax{
public:
	static int USAGE_VERSION;
	
private:
	deoglRenderThread &pRenderThread;
	
	deoglArrayTexture *pTexture;
	deoglFramebuffer **pFBOs;
	
	deoglArrayTexture *pTextureMin;
	deoglFramebuffer **pFBOMin;
	deoglArrayTexture *pTextureMax;
	deoglFramebuffer **pFBOMax;
	
	int pWidth;
	int pHeight;
	int pLayerCount;
	int pLevelCount;
	int pMaxLevelCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create depth min-max. */
	deoglDRDepthMinMax( deoglRenderThread &renderThread,
		int width, int height, int layerCount, int maxLevelCount );
	
	/** Clean up depth min-max. */
	~deoglDRDepthMinMax();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Width of base level. */
	inline int GetWidth() const{ return pWidth; }
	
	/** Height of base level. */
	inline int GetHeight() const{ return pHeight; }
	
	/** Layer count. */
	inline int GetLayerCount() const{ return pLayerCount; }
	
	/** Level count. */
	inline int GetLevelCount() const{ return pLevelCount; }
	
	/** Maximum level count. */
	inline int GetMaxLevelCount() const{ return pMaxLevelCount; }
	
	/** Retrieves the texture. */
	inline deoglArrayTexture *GetTexture() const{ return pTexture; }
	/** Retrieves the fbo for a level. */
	deoglFramebuffer *GetFBOAt( int level );
	
	/** Retrieves the min texture. */
	inline deoglArrayTexture *GetTextureMin() const{ return pTextureMin; }
	/** Retrieves the fbo for a level of the min texture. */
	deoglFramebuffer *GetFBOMinAt( int level );
	/** Retrieves the max texture. */
	inline deoglArrayTexture *GetTextureMax() const{ return pTextureMax; }
	/** Retrieves the fbo for a level of the max texture. */
	deoglFramebuffer *GetFBOMaxAt( int level );
	/*@}*/
	
private:
	void pCleanUp();
	
	void pCreateTextures();
	void pCreateFBOs();
};

#endif
