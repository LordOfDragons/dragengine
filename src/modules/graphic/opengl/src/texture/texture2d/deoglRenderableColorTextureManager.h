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

#ifndef _DEOGLRENDERABLECOLORTEXTUREMANAGER_H_
#define _DEOGLRENDERABLECOLORTEXTUREMANAGER_H_

class deoglRenderThread;
class deoglRenderableColorTexture;



/**
 * @brief Renderable Color Texture Manager.
 * Manages renderable textures. Maintains a list of renderable textures build
 * upon demand. Renderable textures once created stay around and can be reused.
 * A renderable texture is only handed out if the format matches and it is not
 * in use by somebody else already. To give up use of a renderable texture use
 * the SetIsUse call with false as the parameter on the returned object.
 */
class deoglRenderableColorTextureManager{
private:
	deoglRenderThread &pRenderThread;
	
	deoglRenderableColorTexture **pTextures;
	int pTextureCount;
	int pTextureSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new renderable texture manager. */
	deoglRenderableColorTextureManager( deoglRenderThread &renderThread );
	/** Cleans up the renderable texture manager. */
	~deoglRenderableColorTextureManager();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of renderable textures. */
	inline int GetTextureCount() const{ return pTextureCount; }
	/** Retrieves the renderable texture for reading only at the given location. */
	const deoglRenderableColorTexture *GetTextureAt( int index ) const;
	
	/** Retrieves the renderable texture for the given format. */
	deoglRenderableColorTexture *GetTextureWith( int width, int height, int componentCount, bool isFloat );
	/*@}*/
};

#endif
