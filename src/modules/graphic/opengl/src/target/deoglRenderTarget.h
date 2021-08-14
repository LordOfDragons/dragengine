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

class deoglRenderThread;
class deoglFramebuffer;
class deoglTexture;
class deoglRenderbuffer;



/**
 * \brief Render target.
 */
class deoglRenderTarget : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglRenderTarget> Ref;
	
	
	
private:
	deoglRenderThread &pRenderThread;
	
	int pWidth;
	int pHeight;
	int pTextureWidth;
	int pTextureHeight;
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
	/** \brief Create render render target. */
	deoglRenderTarget( deoglRenderThread &renderThread, int width, int height, int componentCount, int bitCount );
	
	/** \brief Clean up render render target. */
	virtual ~deoglRenderTarget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Width. */
	inline int GetWidth() const{ return pWidth; }
	
	/** \brief Height. */
	inline int GetHeight() const{ return pHeight; }
	
	/** \brief Aspect ratio. */
	inline float GetAspectRatio() const{ return pAspectRatio; }
	
	/** \brief Bit count. */
	inline int GetBitCount() const{ return pBitCount; }
	
	/** \brief Set size. */
	void SetSize( int width, int height );
	
	
	
	/** \brief Framebuffer or \em NULL if not existing. */
	inline deoglFramebuffer *GetFBO() const{ return pFBO; }
	
	/** \brief Prepare framebuffer for rendering. */
	void PrepareFramebuffer();
	
	/** \brief Release framebuffer after rendering. */
	void ReleaseFramebuffer();
	
	
	
	/** \brief Texture is dirty. */
	inline bool GetTextureDirty() const{ return pDirtyTexture; }
	
	/** \brief Set texture dirty. */
	void SetTextureDirty( bool dirty );
	
	// texture management
	inline deoglTexture *GetTexture() const{ return pTexture; }
	inline int GetTextureWidth() const{ return pTextureWidth; }
	inline int GetTextureHeight() const{ return pTextureHeight; }
	/*@}*/
};

#endif
