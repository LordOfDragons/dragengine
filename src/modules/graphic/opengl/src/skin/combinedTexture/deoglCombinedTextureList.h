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

#ifndef _DEOGLCOMBINEDTEXTURELIST_H_
#define _DEOGLCOMBINEDTEXTURELIST_H_

#include <dragengine/common/math/decMath.h>

class deoglCombinedTexture;
class deoglRenderThread;
class deoglRImage;



/**
 * Combined Texture List.
 */
class deoglCombinedTextureList{
private:
	deoglRenderThread &pRenderThread;
	
	deoglCombinedTexture *pRoot;
	deoglCombinedTexture *pTail;
	int pCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create list. */
	deoglCombinedTextureList( deoglRenderThread &pRenderThread );
	
	/** Clean up list. */
	~deoglCombinedTextureList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** Number of combined textures in the list. */
	inline int GetCount() const{ return pCount; }
	
	/** Root combined texture. */
	inline deoglCombinedTexture *GetRoot() const{ return pRoot; }
	
	/**
	 * Combined texture with parameters.
	 * \details Creates combined texture if not existing. This adds a usage to the combined
	 *          texture. To remove a usage use the RemoveUsage function on the returned
	 *          combined texture.
	 */
	deoglCombinedTexture *GetWith( const decColor &color, deoglRImage *images[ 4 ] );
	
	/**
	 * Remove combined texture.
	 * \details For use by deoglCombinedTexture only.
	 */
	void Remove( deoglCombinedTexture *combinedTexture );
	/*@}*/
};

#endif
