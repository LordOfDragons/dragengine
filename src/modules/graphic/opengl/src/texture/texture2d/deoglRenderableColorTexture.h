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

#ifndef _DEOGLRENDERABLECOLORTEXTURE_H_
#define _DEOGLRENDERABLECOLORTEXTURE_H_

class deoglRenderThread;
class deoglTexture;



/**
 * @brief Renderable Color Texture.
 * Stores a renderable texture of any kind. Renderable textures can not be
 * used by more than one entity at the same time. They can also not change
 * the size of format once created. They can though be reused if the entity
 * holding the texture gives it up.
 */
class deoglRenderableColorTexture{
private:
	int pWidth;
	int pHeight;
	int pComponentCount;
	bool pIsFloat;
	bool pInUse;
	
	deoglTexture *pTexture;
	
	int pMemoryUsageGPU;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new renderable texture. */
	deoglRenderableColorTexture( deoglRenderThread &renderThread, int width, int height, int componentCount, bool isFloat );
	/** Cleans up the renderable texture. */
	~deoglRenderableColorTexture();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the width. */
	inline int GetWidth() const{ return pWidth; }
	/** Retrieves the height. */
	inline int GetHeight() const{ return pHeight; }
	/** Retrieves the number of components. */
	inline int GetComponentCount() const{ return pComponentCount; }
	/** Retrieves the texture is of float format. */
	inline bool GetIsFloat() const{ return pIsFloat; }
	
	/** Determines if the format matches the given format. */
	bool Matches( int width, int height, int componentCount, bool isFloat ) const;
	
	/** Determines if the shadow map is in use. */
	inline bool GetInUse() const{ return pInUse; }
	/** Sets if the shadow map is in use. */
	void SetInUse( bool inUse );
	
	/** Retrieves the texture. */
	inline deoglTexture *GetTexture() const{ return pTexture; }
	
	/** Retrieves the GPU memory usage. */
	inline int GetMemoryUsageGPU() const{ return pMemoryUsageGPU; }
	/** Updates the GPU memory consumption. */
	void UpdateMemoryUsage();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
