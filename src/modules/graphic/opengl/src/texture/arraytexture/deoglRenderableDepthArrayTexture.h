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

#ifndef _DEOGLRENDERABLEDEPTHARRAYTEXTURE_H_
#define _DEOGLRENDERABLEDEPTHARRAYTEXTURE_H_

class deoglRenderThread;
class deoglArrayTexture;



/**
 * Stores a renderable texture of any kind. Renderable textures can not be
 * used by more than one entity at the same time. They can also not change
 * the size of format once created. They can though be reused if the entity
 * holding the texture gives it up.
 */
class deoglRenderableDepthArrayTexture{
private:
	int pWidth;
	int pHeight;
	int pLayerCount;
	bool pWithStencil;
	bool pUseFloat;
	bool pInUse;
	
	deoglArrayTexture *pArrayTexture;
	
	int pMemoryUsageGPU;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderable texture. */
	deoglRenderableDepthArrayTexture( deoglRenderThread &renderThread,
		int width, int height, int layerCount, bool withStencil, bool useFloat );
	
	/** Clean up renderable texture. */
	~deoglRenderableDepthArrayTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Width. */
	inline int GetWidth() const{ return pWidth; }
	
	/** Height. */
	inline int GetHeight() const{ return pHeight; }
	
	/** Layer count. */
	inline int GetLayerCount() const{ return pLayerCount; }
	
	/** With stencil buffer. */
	inline bool GetWithStencil() const{ return pWithStencil; }
	
	/** Use float depth. */
	inline bool GetUseFloat() const{ return pUseFloat; }
	
	
	
	/** Format matches. */
	bool Matches( int width, int height, int layers, bool withStencil, bool useFloat ) const;
	
	/** ArrayTexture is in use. */
	inline bool GetInUse() const{ return pInUse; }
	
	/** Set if texture is in use. */
	void SetInUse( bool inUse );
	
	
	
	/** ArrayTexture. */
	inline deoglArrayTexture *GetArrayTexture() const{ return pArrayTexture; }
	
	
	
	/** GPU memory usage. */
	inline int GetMemoryUsageGPU() const{ return pMemoryUsageGPU; }
	
	/** Update GPU memory consumption. */
	void UpdateMemoryUsage();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
