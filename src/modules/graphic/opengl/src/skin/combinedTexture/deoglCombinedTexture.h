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

#ifndef _DEOGLCOMBINEDTEXTURE_H_
#define _DEOGLCOMBINEDTEXTURE_H_

#include "../../texture/deoglRImage.h"

#include <dragengine/common/math/decMath.h>

class deoglPixelBuffer;
class deoglRenderThread;
class deoglTexture;



/**
 * Combined texture.
 * 
 * Stores a texture composed of 2 or more static colors or images.
 */
class deoglCombinedTexture{
private:
	deoglRenderThread &pRenderThread;
	
	decColor pColor;
	deoglRImage::Ref pImages[ 4 ];
	
	deoglTexture *pTexture;
	
	int pUsageCount;
	unsigned int pHashCode;
	deoglCombinedTexture *pLLPrev;
	deoglCombinedTexture *pLLNext;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create combined texture. */
	deoglCombinedTexture( deoglRenderThread &renderThread, const decColor &color, deoglRImage *images[ 4 ] );
	
	/** Clean up combined texture. */
	~deoglCombinedTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** Static color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** Image for component or nullptr to use the static color component. */
	const deoglRImage::Ref &GetImageAt( int component ) const;
	
	
	
	/** Texture or nullptr if not existing. */
	inline deoglTexture *GetTexture() const{ return pTexture; }
	
	/** Set texture or nullptr if not existing. */
	void SetTexture( deoglTexture *texture );
	
	
	
	/** Combined texture matches another combined texture. */
	bool Equals( const deoglCombinedTexture &combinedTexture ) const;
	
	/** Combined texture matches another combined texture. */
	bool Equals( const decColor &color, deoglRImage *images[ 4 ] ) const;
	
	
	
	/** Usage count. */
	inline int GetUsageCount() const{ return pUsageCount; }
	
	/** Increment usage count by one. */
	void AddUsage();
	
	/**
	 * Decrement usage count by one.
	 * \details If usage count reaches 0 the combined texture is removed from the
	 *          combined texture manager and deleted.
	 */
	void RemoveUsage();
	
	
	
	/** Hash code. */
	inline unsigned int GetHashCode() const{ return pHashCode; }
	
	/** Calculate hash code. */
	void CalcHashCode();
	
	/** Calculate hash code for combination. */
	static unsigned int CalcHashCodeFor( const decColor &color, const deoglRImage::Ref *images );
	
	
	
	/** Previous entry in the linked list. */
	inline deoglCombinedTexture *GetLLPrev() const{ return pLLPrev; }
	
	/** Set previous entry in the linked list. */
	void SetLLPrev( deoglCombinedTexture *entry );
	
	/** Next entry in the linked list. */
	inline deoglCombinedTexture *GetLLNext() const{ return pLLNext; }
	
	/** Set next entry in the linked list. */
	void SetLLNext( deoglCombinedTexture *entry );
	/*@}*/
};

#endif
