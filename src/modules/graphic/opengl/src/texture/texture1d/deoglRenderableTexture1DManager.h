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

#ifndef _DEOGLRENDERABLETEXTURE1DMANAGER_H_
#define _DEOGLRENDERABLETEXTURE1DMANAGER_H_
#include <dragengine/common/collection/decPointerList.h>

class deoglRenderThread;
class deoglRenderableTexture1D;



/**
 * \brief Renderable 1D Texture Manager.
 * \details Manages renderable 1d textures. Maintains a list of renderable 1d textures
 *          build upon demand. Renderable 1d textures once created stay around and can
 *          be reused. A renderable 1d texture is only handed out if the format matches
 *          and it is not in use by somebody else already. To give up use of a renderable
 *          1d texture use the SetIsUse call with false as the parameter on the returned object.
 */
class deoglRenderableTexture1DManager{
private:
	deoglRenderThread &pRenderThread;
	decPointerList pTextures;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new renderable texture manager. */
	deoglRenderableTexture1DManager( deoglRenderThread &renderThread );
	/** \brief Cleans up the renderable texture manager. */
	~deoglRenderableTexture1DManager();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the number of renderable textures. */
	int GetTextureCount() const;
	/** \brief Retrieves the renderable texture for reading only at the given location. */
	const deoglRenderableTexture1D *GetTextureAt( int index ) const;
	
	/** \brief Retrieves the renderable texture for the given format. */
	deoglRenderableTexture1D *GetTextureWith( int size, int format );
	/*@}*/
};

#endif
