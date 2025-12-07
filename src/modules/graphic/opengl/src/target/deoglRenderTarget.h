/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DEOGLRRENDERTARGET_H_
#define _DEOGLRRENDERTARGET_H_

#include "../framebuffer/deoglFramebuffer.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglTexture;


/**
 * Render target.
 */
class deoglRenderTarget : public deObject{
public:
	typedef deTObjectReference<deoglRenderTarget> Ref;
	
	
private:
	deoglRenderThread &pRenderThread;
	
	decPoint pSize, pTextureSize;
	float pAspectRatio;
	int pBitCount, pComponentCount;
	float pFloatTexture;
	
	bool pDirtyTexture;
	
	deoglTexture *pTexture;
	deoglFramebuffer::Ref pFBO;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render render target. */
	deoglRenderTarget(deoglRenderThread &renderThread, const decPoint &size, int componentCount, int bitCount);
	
	/** Clean up render render target. */
	virtual ~deoglRenderTarget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Size. */
	inline const decPoint &GetSize() const{return pSize;}
	
	/** Aspect ratio. */
	inline float GetAspectRatio() const{return pAspectRatio;}
	
	/** Bit count. */
	inline int GetBitCount() const{return pBitCount;}
	
	/** Component count. */
	inline int GetComponentCount() const{return pComponentCount;}
	
	/** Set size. */
	void SetSize(const decPoint &size);
	
	
	
	/** Framebuffer or \em NULL if not existing. */
	inline const deoglFramebuffer::Ref &GetFBO() const{return pFBO;}
	
	/** Prepare texture. */
	void PrepareTexture();
	
	/** Prepare framebuffer for rendering. */
	void PrepareFramebuffer();
	
	/** Release framebuffer after rendering. */
	void ReleaseFramebuffer();
	
	
	
	/** Texture is dirty. */
	inline bool GetTextureDirty() const{return pDirtyTexture;}
	
	/** Set texture dirty. */
	void SetTextureDirty(bool dirty);
	
	// texture management
	inline deoglTexture *GetTexture() const{return pTexture;}
	inline const decPoint &GetTextureSize() const{return pTextureSize;}
	/*@}*/
};

#endif
