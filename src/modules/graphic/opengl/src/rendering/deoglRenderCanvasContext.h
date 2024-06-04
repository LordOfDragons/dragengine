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

#ifndef _DEOGLRENDERCANVASCONTEXT_H_
#define _DEOGLRENDERCANVASCONTEXT_H_

#include <dragengine/common/math/decMath.h>

class deoglFramebuffer;
class deoglRCanvas;
class deoglRCanvasView;
class deoglRenderTarget;
class deoglTexture;
class deoglRenderPlanMasked;
class deoglSkinStateConstructedNode;



/**
 * Render canvas context.
 */
class deoglRenderCanvasContext{
private:
	deoglFramebuffer *pFBO;
	decPoint pViewportOffset;
	decPoint pViewportSize;
	const deoglRenderPlanMasked *pRenderPlanMask;
	
	decVector2 pClipFactor;
	decVector2 pClipMin;
	decVector2 pClipMax;
	
	decTexMatrix2 pTransform;
	decVector2 pTCClampMin;
	decVector2 pTCClampMax;
	decColorMatrix pColorTransform;
	float pTransparency;
	deoglTexture *pMask;
	decTexMatrix2 pTransformMask;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create copy of a render canvas context. */
	deoglRenderCanvasContext( const deoglRenderCanvasContext &copy );
	
	/** Create render canvas context for an initial canvas view. */
	deoglRenderCanvasContext( const deoglRCanvas &canvas, deoglFramebuffer *fbo,
		const decPoint &viewportOffset, const decPoint &viewportSize, bool upsideDown,
		const deoglRenderPlanMasked *renderPlanMask );
	
	/** Create render canvas context for a child canvas. */
	deoglRenderCanvasContext( const deoglRenderCanvasContext &parentContext,
		const deoglRCanvas &childCanvas );
	
	/** Create render canvas context for an initial constructed node. */
	deoglRenderCanvasContext( const deoglSkinStateConstructedNode &node, deoglFramebuffer *fbo,
		const decPoint &viewportOffset, const decPoint &viewportSize, bool upsideDown,
		const deoglRenderPlanMasked *renderPlanMask );
	
	/** Create render canvas context for a child constructed node. */
	deoglRenderCanvasContext( const deoglRenderCanvasContext &parentContext,
		const deoglSkinStateConstructedNode &childNode );
	
	/** Clean up render canvas context. */
	~deoglRenderCanvasContext();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Framebuffer if offscreen rendering or \em NULL to use the primary framebuffer. */
	inline deoglFramebuffer *GetFBO() const{ return pFBO; }
	
	/** Viewport offset. */
	inline const decPoint &GetViewportOffset() const{ return pViewportOffset; }
	
	/** Viewport size. */
	inline const decPoint &GetViewportSize() const{ return pViewportSize; }
	
	/** Render plan mask or NULL. */
	inline const deoglRenderPlanMasked *GetRenderPlanMask() const{ return pRenderPlanMask; }
	
	
	/** Factor to scale clipping to proper opengl coordinates. */
	inline const decVector2 &GetClipFactor() const{ return pClipFactor; }
	
	/** Rectangular clipping minimum. Shader version of glViewport/glScissor. */
	inline const decVector2 &GetClipMin() const{ return pClipMin; }
	
	/** Set rectangular clipping minimum. Shader version of glViewport/glScissor. */
	void SetClipMin( const decVector2 &clipMin );
	
	/** Rectangular clipping maximum. Shader version of glViewport/glScissor. */
	inline const decVector2 &GetClipMax() const{ return pClipMax; }
	
	/** Set rectangular clipping maximum. Shader version of glViewport/glScissor. */
	void SetClipMax( const decVector2 &clipMax );
	
	/** Clip max is less than or equal to clip min for at least one coordinate. */
	bool IsZeroClip() const;
	
	
	
	/** Transformation relative to render target. */
	inline const decTexMatrix2 &GetTransform() const{ return pTransform; }
	
	/** Set transformation relative to render target. */
	void SetTransform( const decTexMatrix2 &transform );
	
	/** Texture coordinates clamp minimum. */
	inline const decVector2 &GetTCClampMinimum() const{ return pTCClampMin; }
	
	/** Set texture coordinates clamp minimum. */
	void SetTCClampMinimum( const decVector2 &clamp );
	
	/** Texture coordinates clamp maximum. */
	inline const decVector2 &GetTCClampMaximum() const{ return pTCClampMax; }
	
	/** Set texture coordinates clamp maximum. */
	void SetTCClampMaximum( const decVector2 &clamp );
	
	/** Color transformation relative to render target. */
	inline const decColorMatrix &GetColorTransform() const{ return pColorTransform; }
	
	/** Set color transformation relative to render target. */
	void SetColorTransform( const decColorMatrix &transform );
	
	/** Transparency relative to render target. */
	inline float GetTransparency() const{ return pTransparency; }
	
	/** Set transparency relative to render target. */
	void SetTransparency( float transparency );
	
	/** Mask or NULL. */
	inline deoglTexture *GetMask() const{ return pMask; }
	
	/** Set mask or NULL. */
	void SetMask( deoglTexture *mask );
	
	/** Transformation relative to mask render target. */
	inline const decTexMatrix2 &GetTransformMask() const{ return pTransformMask; }
	
	/** Set transformation relative to mask render target. */
	void SetTransformMask( const decTexMatrix2 &transform );
	
	/** Update transformation mask from transformation. */
	void UpdateTransformMask();
	/*@}*/
	
	
	
private:
	void pCalculateClipping( const decVector2 &canvasSize );
};

#endif
