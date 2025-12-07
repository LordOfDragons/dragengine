/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEOGLFRAMEBUFFERARRAY_H_
#define _DEOGLFRAMEBUFFERARRAY_H_

#include "deoglFramebuffer.h"
#include <dragengine/common/collection/decObjectList.h>


/**
 * Framebuffer object for use with array textures.
 */
class deoglFramebufferArray : public deObject{
public:
	typedef deTObjectReference<deoglFramebufferArray> Ref;
	
	
private:
	deoglRenderThread &pRenderThread;
	const int pLayerCount;
	deoglFramebuffer::Ref pFBO;
	decObjectList pLayerFBOs;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create framebuffer object. */
	deoglFramebufferArray(deoglRenderThread &renderThread, int layerCount);
	
protected:
	/** Cleans up the framebuffer object. */
	~deoglFramebufferArray() override = default;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Layer count. */
	inline int GetLayerCount() const{ return pLayerCount; }
	
	/** Framebuffer or nullptr if layered. */
	inline const deoglFramebuffer::Ref &GetFBO() const{ return pFBO; }
	
	/** Layer framebuffer or nullptr if not layered. */
	deoglFramebuffer *GetLayerFBO(int layer) const;
	
	/** GetFBO() or GetLayerFBO(0) if layered. */
	deoglFramebuffer *GetBaseFBO() const;
	
	/** Verify framebuffer. */
	void Verify();
	
	
	/** Color target is attached. */
	bool IsColorAttached(int index) const;
	
	/** Color target is not attached. */
	bool IsColorDetached(int index) const;
	
	/** Attach color array texture. */
	void AttachColorArrayTexture(int index, deoglArrayTexture *texture);
	
	/** Attach a color array texture level. */
	void AttachColorArrayTextureLevel(int index, deoglArrayTexture *texture, int level);
	
	/** Detach a color image if attached. */
	void DetachColorImage(int index);
	
	/** Detaches all color images in this and all higher attachment points if they are attached. */
	void DetachColorImages(int startIndex);
	
	/** Invalidate color attachment. */
	void InvalidateColor(int index);
	
	
	/** Depth target is attached. */
	bool IsDepthAttached() const;
	
	/** No depth target is attached. */
	bool IsDepthDetached() const;
	
	/** Attach depth array texture. */
	void AttachDepthArrayTexture(deoglArrayTexture *texture);
	
	/** Attach depth array texture level. */
	void AttachDepthArrayTextureLevel(deoglArrayTexture *texture, int level);
	
	/** Detach depth image if attached. */
	void DetachDepthImage();
	
	/** Invalidate depth attachment. */
	void InvalidateDepth();
	
	
	/** Stencil target is attached. */
	bool IsStencilAttached() const;
	
	/** No stencil target is attached. */
	bool IsStencilDetached() const;
	
	/** Attach stencil texture. */
	void AttachStencilArrayTexture(deoglArrayTexture *texture);
	
	/** Attach stencil texture level. */
	void AttachStencilArrayTextureLevel(deoglArrayTexture *texture, int level);
	
	/** Detach stencil image if attached. */
	void DetachStencilImage();
	
	/** Invalidate stencil attachment. */
	void InvalidateStencil();
	
	/** Invalidate depth and stencil attachments. */
	void InvalidateDepthStencil();
	
	
	/** Attach color array textures. */
	void AttachColorArrayTextures(deoglArrayTexture *texture1 = nullptr,
		deoglArrayTexture *texture2 = nullptr, deoglArrayTexture *texture3 = nullptr,
		deoglArrayTexture *texture4 = nullptr, deoglArrayTexture *texture5 = nullptr,
		deoglArrayTexture *texture6 = nullptr, deoglArrayTexture *texture7 = nullptr);
	
	/** Attach a color array textures level. */
	void AttachColorArrayTexturesLevel(int level, deoglArrayTexture *texture1 = nullptr,
		deoglArrayTexture *texture2 = nullptr, deoglArrayTexture *texture3 = nullptr,
		deoglArrayTexture *texture4 = nullptr, deoglArrayTexture *texture5 = nullptr,
		deoglArrayTexture *texture6 = nullptr, deoglArrayTexture *texture7 = nullptr);
	
	/** Attach depth/stencil array texture. */
	void AttachDepthStencilArrayTexture(deoglArrayTexture *texture);
	
	/** Attach depth/stencil array texture level. */
	void AttachDepthStencilArrayTextureLevel(deoglArrayTexture *texture, int level);
	
	
	/** Detach all images if they are attached. */
	void DetachAllImages();
	
	/** Invalidate all images that are attached. */
	void InvalidateAllImages();
	
	
	/** Adds a configuration trace point with the configuration of this framebuffer object. */
	void AddConfigToTrace(deErrorTracePoint &tracePoint);
	
	/** Debug print framebuffer configuration. */
	void DebugPrint(const char *prefix);
	
	/** Set debug object label. */
	void SetDebugObjectLabel(const char *name);
	/*@}*/
};

#endif
