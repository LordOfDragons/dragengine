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

#ifndef _DEOGLFRAMEBUFFERCUBE_H_
#define _DEOGLFRAMEBUFFERCUBE_H_

#include "deoglFramebuffer.h"
#include <dragengine/common/collection/decObjectList.h>


/**
 * Framebuffer object for use with cube map textures.
 */
class deoglFramebufferCube : public deObject{
public:
	typedef deTObjectReference<deoglFramebufferCube> Ref;
	
	
private:
	deoglRenderThread &pRenderThread;
	deoglFramebuffer::Ref pFBO;
	decObjectList pFaceFBOs;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create framebuffer object. */
	deoglFramebufferCube(deoglRenderThread &renderThread);
	
protected:
	/** Cleans up the framebuffer object. */
	~deoglFramebufferCube() override = default;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Framebuffer or nullptr if layered. */
	inline const deoglFramebuffer::Ref &GetFBO() const{ return pFBO; }
	
	/** Face framebuffer or nullptr if not layered. */
	deoglFramebuffer *GetFaceFBO(int face) const;
	
	/** GetFBO() or GetFaceFBO(0) if layered. */
	deoglFramebuffer *GetBaseFBO() const;
	
	/** Verify framebuffer. */
	void Verify();
	
	
	/** Color target is attached. */
	bool IsColorAttached(int index) const;
	
	/** Color target is not attached. */
	bool IsColorDetached(int index) const;
	
	/** Attach color cube texture. */
	void AttachColorCubeMap(int index, deoglCubeMap *texture);
	
	/** Attach a color cube texture level. */
	void AttachColorCubeMapLevel(int index, deoglCubeMap *texture, int level);
	
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
	
	/** Attach depth cube texture. */
	void AttachDepthCubeMap(deoglCubeMap *texture);
	
	/** Attach depth cube texture level. */
	void AttachDepthCubeMapLevel(deoglCubeMap *texture, int level);
	
	/** Detach depth image if attached. */
	void DetachDepthImage();
	
	/** Invalidate depth attachment. */
	void InvalidateDepth();
	
	
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
