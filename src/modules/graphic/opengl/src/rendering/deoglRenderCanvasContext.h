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



/**
 * \brief Render canvas context.
 */
class deoglRenderCanvasContext{
private:
	deoglFramebuffer *pFBO;
	decPoint pViewportOffset;
	decPoint pViewportSize;
	
	decVector2 pClipFactor;
	decVector2 pClipMin;
	decVector2 pClipMax;
	
	decTexMatrix2 pTransform;
	decVector2 pTCClampMin;
	decVector2 pTCClampMax;
	decColorMatrix pColorTransform;
	float pTransparency;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create copy of a render canvas context. */
	deoglRenderCanvasContext( const deoglRenderCanvasContext &copy );
	
	/** \brief Create render canvas context for an initial canvas view. */
	deoglRenderCanvasContext( const deoglRCanvasView &canvas, deoglFramebuffer *fbo,
		const decPoint &viewportOffset, const decPoint &viewportSize, bool upsideDown );
	
	/** \brief Create render canvas context for a child canvas. */
	deoglRenderCanvasContext( const deoglRenderCanvasContext &parentContext, const deoglRCanvas &childCanvas );
	
	/** \brief Clean up render canvas context. */
	~deoglRenderCanvasContext();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Framebuffer if offscreen rendering or \em NULL to use the primary framebuffer. */
	inline deoglFramebuffer *GetFBO() const{ return pFBO; }
	
	/** \brief Viewport offset. */
	inline const decPoint &GetViewportOffset() const{ return pViewportOffset; }
	
	/** \brief Viewport size. */
	inline const decPoint &GetViewportSize() const{ return pViewportSize; }
	
	
	
	/** \brief Factor to scale clipping to proper opengl coordinates. */
	inline const decVector2 &GetClipFactor() const{ return pClipFactor; }
	
	/** \brief Rectangular clipping minimum. Shader version of glViewport/glScissor. */
	inline const decVector2 &GetClipMin() const{ return pClipMin; }
	
	/** \brief Set rectangular clipping minimum. Shader version of glViewport/glScissor. */
	void SetClipMin( const decVector2 &clipMin );
	
	/** \brief Rectangular clipping maximum. Shader version of glViewport/glScissor. */
	inline const decVector2 &GetClipMax() const{ return pClipMax; }
	
	/** \brief Set rectangular clipping maximum. Shader version of glViewport/glScissor. */
	void SetClipMax( const decVector2 &clipMax );
	
	/** \brief Clip max is less than or equal to clip min for at least one coordinate. */
	bool IsZeroClip() const;
	
	
	
	/** \brief Transformation relative to render target. */
	inline const decTexMatrix2 &GetTransform() const{ return pTransform; }
	
	/** \brief Set transformation relative to render target. */
	void SetTransform( const decTexMatrix2 &transform );
	
	/** \brief Texture coordinates clamp minimum. */
	inline const decVector2 &GetTCClampMinimum() const{ return pTCClampMin; }
	
	/** \brief Set texture coordinates clamp minimum. */
	void SetTCClampMinimum( const decVector2 &clamp );
	
	/** \brief Texture coordinates clamp maximum. */
	inline const decVector2 &GetTCClampMaximum() const{ return pTCClampMax; }
	
	/** \brief Set texture coordinates clamp maximum. */
	void SetTCClampMaximum( const decVector2 &clamp );
	
	/** \brief Color transformation relative to render target. */
	inline const decColorMatrix &GetColorTransform() const{ return pColorTransform; }
	
	/** \brief Set color transformation relative to render target. */
	void SetColorTransform( const decColorMatrix &transform );
	
	/** \brief Transparency relative to render target. */
	inline float GetTransparency() const{ return pTransparency; }
	
	/** \brief Set transparency relative to render target. */
	void SetTransparency( float transparency );
	/*@}*/
	
private:
	void pCalculateClipping( const decVector2 &canvasSize );
};

#endif
