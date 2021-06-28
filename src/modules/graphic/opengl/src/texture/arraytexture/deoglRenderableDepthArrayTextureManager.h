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
#ifndef _DEOGLRENDERABLEDEPTHARRAYTEXTUREMANAGER_H_
#define _DEOGLRENDERABLEDEPTHARRAYTEXTUREMANAGER_H_

// predefinitions
class deoglRenderThread;
class deoglRenderableDepthArrayTexture;



/**
 * Manages renderable textures. Maintains a list of renderable textures build
 * upon demand. Renderable textures once created stay around and can be reused.
 * A renderable texture is only handed out if the format matches and it is not
 * in use by somebody else already. To give up use of a renderable texture use
 * the SetIsUse call with false as the parameter on the returned object.
 */
class deoglRenderableDepthArrayTextureManager{
private:
	deoglRenderThread &pRenderThread;
	
	deoglRenderableDepthArrayTexture **pArrayTextures;
	int pArrayTextureCount;
	int pArrayTextureSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderable texture manager. */
	deoglRenderableDepthArrayTextureManager( deoglRenderThread &renderThread );
	
	/** Clean up renderable texture manager. */
	~deoglRenderableDepthArrayTextureManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Count of renderable textures. */
	inline int GetCount() const{ return pArrayTextureCount; }
	
	/** Renderable texture at index for reading purpose only. */
	const deoglRenderableDepthArrayTexture *GetAt( int index ) const;
	
	/** Renderable texture matching format. */
	deoglRenderableDepthArrayTexture *GetWith( int width, int height, int layerCount,
		bool withStencil, bool useFloat );
	/*@}*/
};

#endif
