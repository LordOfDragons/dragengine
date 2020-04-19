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

#include <dragengine/common/math/decMath.h>

class deoglPixelBuffer;
class deoglRenderThread;
class deoglRImage;
class deoglTexture;



/**
 * \brief Combined texture.
 * 
 * Stores a texture composed of 2 or more static colors or images.
 */
class deoglCombinedTexture{
private:
	deoglRenderThread &pRenderThread;
	
	decColor pColor;
	deoglRImage *pImages[ 4 ];
	
	deoglTexture *pTexture;
	
	int pUsageCount;
	unsigned int pHashCode;
	deoglCombinedTexture *pLLPrev;
	deoglCombinedTexture *pLLNext;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create combined texture. */
	deoglCombinedTexture( deoglRenderThread &renderThread, const decColor &color, deoglRImage *images[ 4 ] );
	
	/** \brief Clean up combined texture. */
	~deoglCombinedTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** \brief Static color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Image for component or \em NULL to use the static color component. */
	deoglRImage *GetImageAt( int component ) const;
	
	
	
	/** \brief Texture or \em NULL if not existing. */
	inline deoglTexture *GetTexture() const{ return pTexture; }
	
	/** \brief Set texture or \em NULL if not existing. */
	void SetTexture( deoglTexture *texture );
	
	
	
	/** \brief Combined texture matches another combined texture. */
	bool Equals( const deoglCombinedTexture &combinedTexture ) const;
	
	/** \brief Combined texture matches another combined texture. */
	bool Equals( const decColor &color, deoglRImage *images[ 4 ] ) const;
	
	
	
	/** \brief Usage count. */
	inline int GetUsageCount() const{ return pUsageCount; }
	
	/** \brief Increment usage count by one. */
	void AddUsage();
	
	/**
	 * \brief Decrement usage count by one.
	 * \details If usage count reaches 0 the combined texture is removed from the
	 *          combined texture manager and deleted.
	 */
	void RemoveUsage();
	
	
	
	/** \brief Hash code. */
	inline unsigned int GetHashCode() const{ return pHashCode; }
	
	/** \brief Calculate hash code. */
	void CalcHashCode();
	
	/** \brief Calculate hash code for combination. */
	static unsigned int CalcHashCodeFor( const decColor &color, deoglRImage *images[ 4 ] );
	
	
	
	/** \brief Previous entry in the linked list. */
	inline deoglCombinedTexture *GetLLPrev() const{ return pLLPrev; }
	
	/** \brief Set previous entry in the linked list. */
	void SetLLPrev( deoglCombinedTexture *entry );
	
	/** \brief Next entry in the linked list. */
	inline deoglCombinedTexture *GetLLNext() const{ return pLLNext; }
	
	/** \brief Set next entry in the linked list. */
	void SetLLNext( deoglCombinedTexture *entry );
	/*@}*/
};

#endif
