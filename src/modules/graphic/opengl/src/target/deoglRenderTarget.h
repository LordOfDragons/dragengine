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

#ifndef _DEOGLRRENDERTARGET_H_
#define _DEOGLRRENDERTARGET_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglFramebuffer;
class deoglTexture;



/**
 * Render target.
 */
class deoglRenderTarget : public deObject{
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<deoglRenderTarget> Ref;
	
	
	
private:
	deoglRenderThread &pRenderThread;
	
	decPoint pSize;
	decPoint pTextureSize;
	float pAspectRatio;
	int pBitCount;
	int pComponentCount;
	float pFloatTexture;
	
	bool pDirtyTexture;
	
	deoglTexture *pTexture;
	deoglFramebuffer *pFBO;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render render target. */
	deoglRenderTarget( deoglRenderThread &renderThread, const decPoint &size, int componentCount, int bitCount );
	
	/** Clean up render render target. */
	virtual ~deoglRenderTarget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Size. */
	inline const decPoint &GetSize() const{ return pSize; }
	
	/** Aspect ratio. */
	inline float GetAspectRatio() const{ return pAspectRatio; }
	
	/** Bit count. */
	inline int GetBitCount() const{ return pBitCount; }
	
	/** Component count. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** Set size. */
	void SetSize( const decPoint &size );
	
	
	
	/** Framebuffer or \em NULL if not existing. */
	inline deoglFramebuffer *GetFBO() const{ return pFBO; }
	
	/** Prepare texture. */
	void PrepareTexture();
	
	/** Prepare framebuffer for rendering. */
	void PrepareFramebuffer();
	
	/** Release framebuffer after rendering. */
	void ReleaseFramebuffer();
	
	
	
	/** Texture is dirty. */
	inline bool GetTextureDirty() const{ return pDirtyTexture; }
	
	/** Set texture dirty. */
	void SetTextureDirty( bool dirty );
	
	// texture management
	inline deoglTexture *GetTexture() const{ return pTexture; }
	inline const decPoint &GetTextureSize() const{ return pTextureSize; }
	/*@}*/
};

#endif
