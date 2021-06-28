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

#ifndef _DEOGLRENDERCANVASCONTEXT_H_
#define _DEOGLRENDERCANVASCONTEXT_H_

#include <dragengine/common/math/decMath.h>

class deoglFramebuffer;
class deoglRCanvas;
class deoglRCanvasView;
class deoglRenderTarget;
class deoglTexture;
class deoglRenderPlanMasked;



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
