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

#ifndef _DEOGLRENDERABLECOLORARRAYTEXTURE_H_
#define _DEOGLRENDERABLECOLORARRAYTEXTURE_H_

class deoglRenderThread;
class deoglArrayTexture;



/**
 * Stores a renderable texture of any kind. Renderable textures can not be
 * used by more than one entity at the same time. They can also not change
 * the size of format once created. They can though be reused if the entity
 * holding the texture gives it up.
 */
class deoglRenderableColorArrayTexture{
private:
	int pWidth;
	int pHeight;
	int pLayerCount;
	int pComponentCount;
	bool pIsFloat;
	bool pInUse;
	
	deoglArrayTexture *pArrayTexture;
	
	int pMemoryUsageGPU;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderable texture. */
	deoglRenderableColorArrayTexture( deoglRenderThread &renderThread, int width,
		int height, int layerCount, int componentCount, bool isFloat );
	
	/** Clean up renderable texture. */
	~deoglRenderableColorArrayTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Width. */
	inline int GetWidth() const{ return pWidth; }
	
	/** Height. */
	inline int GetHeight() const{ return pHeight; }
	
	/** Layer count. */
	inline int GetLayerCount() const{ return pLayerCount; }
	
	/** Count of components. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** Use float format. */
	inline bool GetIsFloat() const{ return pIsFloat; }
	
	/** Format matches. */
	bool Matches( int width, int height, int layers, int componentCount, bool isFloat ) const;
	
	/** Texture is in use. */
	inline bool GetInUse() const{ return pInUse; }
	
	/** Set if texture is in use. */
	void SetInUse( bool inUse );
	
	/** Texture. */
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
