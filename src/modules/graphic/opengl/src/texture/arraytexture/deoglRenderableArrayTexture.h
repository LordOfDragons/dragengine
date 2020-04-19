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

#ifndef _DEOGLRENDERABLEARRAYTEXTURE_H_
#define _DEOGLRENDERABLEARRAYTEXTURE_H_

class deoglRenderThread;
class deoglArrayTexture;



/**
 * @brief Renderable Array Texture.
 * Stores a renderable array texture of any kind. Renderable array textures
 * can not be used by more than one entity at the same time. They can also
 * not change the size or format once created. They can though be reused if
 * the entity holding the texture gives it up.
 */
class deoglRenderableArrayTexture{
private:
	int pWidth;
	int pHeight;
	int pLayerCount;
	int pFormat;
	bool pInUse;
	
	deoglArrayTexture *pTexture;
	
	int pMemoryUsageGPU;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new renderable array texture. */
	deoglRenderableArrayTexture( deoglRenderThread &renderThread, int width, int height, int layerCount, int format );
	/** Cleans up the renderable array texture. */
	~deoglRenderableArrayTexture();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the width. */
	inline int GetWidth() const{ return pWidth; }
	/** Retrieves the height. */
	inline int GetHeight() const{ return pHeight; }
	/** Retrieves the number of layers. */
	inline int GetLayerCount() const{ return pLayerCount; }
	/** Retrieves the format. */
	inline int GetFormat() const{ return pFormat; }
	
	/** Determines if this array texture has the matching parameters. */
	bool Matches( int width, int height, int layerCount, int format ) const;
	
	/** Determines if the shadow map is in use. */
	inline bool GetInUse() const{ return pInUse; }
	/** Sets if the shadow map is in use. */
	void SetInUse( bool inUse );
	
	/** Retrieves the array texture. */
	inline deoglArrayTexture *GetTexture() const{ return pTexture; }
	
	/** Retrieves the GPU memory usage. */
	inline int GetMemoryUsageGPU() const{ return pMemoryUsageGPU; }
	/** Updates the GPU memory consumption. */
	void UpdateMemoryUsage();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
