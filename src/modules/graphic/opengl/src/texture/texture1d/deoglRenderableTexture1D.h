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

#ifndef _DEOGLRENDERABLETEXTURE1D_H_
#define _DEOGLRENDERABLETEXTURE1D_H_

class deoglRenderThread;
class deoglTexture1D;



/**
 * Renderable 1D Texture.
 * \details Stores a renderable 1d texture of any kind. Renderable 1d textures can not
 *          be used by more than one entity at the same time. They can also not change
 *          the size or format once created. They can though be reused if the entity
 *          holding the texture gives it up.
 */
class deoglRenderableTexture1D{
private:
	int pSize;
	int pFormat;
	bool pInUse;
	
	deoglTexture1D *pTexture;
	
	int pMemoryUsageGPU;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new renderable 1d texture. */
	deoglRenderableTexture1D( deoglRenderThread &renderThread, int size, int format );
	/** Cleans up the renderable 1d texture. */
	~deoglRenderableTexture1D();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the size. */
	inline int GetSize() const{ return pSize; }
	/** Retrieves the format. */
	inline int GetFormat() const{ return pFormat; }
	
	/** Determines if this array texture has the matching parameters. */
	bool Matches( int size, int format ) const;
	
	/** Determines if the shadow map is in use. */
	inline bool GetInUse() const{ return pInUse; }
	/** Sets if the shadow map is in use. */
	void SetInUse( bool inUse );
	
	/** Retrieves the 1d texture. */
	inline deoglTexture1D *GetTexture() const{ return pTexture; }
	
	/** Retrieves the GPU memory usage. */
	inline int GetMemoryUsageGPU() const{ return pMemoryUsageGPU; }
	/** Updates the GPU memory consumption. */
	void UpdateMemoryUsage();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
