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

#ifndef _DEOGLRCANVASVIEW_H_
#define _DEOGLRCANVASVIEW_H_

#include "deoglRCanvas.h"

#include <dragengine/common/collection/decObjectList.h>

class deoglRenderTarget;
class deoglRenderPlanMasked;


/**
 * Render canvas view.
 */
class deoglRCanvasView : public deoglRCanvas{
private:
	decObjectList pChildren;
	unsigned int pPaintTracker;
	deoglRenderTarget *pRenderTarget;
	bool pResizeRenderTarget;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglRCanvasView( deoglRenderThread &renderThread );
	
	/** Clean up peer. */
	virtual ~deoglRCanvasView();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Add children with the correct sorting by order. */
	void AddChild( deoglRCanvas *canvas );
	
	/** Remove all children. */
	void RemoveAllChildren();
	
	/** Has no children. */
	bool HasNoChildren() const;
	
	
	
	/** Paint tracker number. */
	inline unsigned int GetPaintTracker() const{ return pPaintTracker; }
	
	/** Increment paint tracker number by one. */
	void IncrementPaintTracker();
	
	/** Render target or \em NULL if not ready. */
	inline deoglRenderTarget *GetRenderTarget() const{ return pRenderTarget; }
	
	/** Render target has to be resized. */
	void SetResizeRenderTarget();
	
	/**
	 * Prepare render target. If not existing the render target is created. If dirty the render
	 * target texture is rendered. After this call returns GetRenderTarget() returns a render
	 * target with a valid texture object suitable for rendering unless an error occurred.
	 */
	void PrepareRenderTarget( const deoglRenderPlanMasked *renderPlanMask,
		int componentCount, int bitCount );
	
	/** Render render target if dirty. */
	void RenderRenderTarget( const deoglRenderPlanMasked *renderPlanMask );
	
	
	
	/** Prepare for rendering. */
	virtual void PrepareForRender( const deoglRenderPlanMasked *renderPlanMask );
	
	/** Prepare for rendering render. */
	virtual void PrepareForRenderRender( const deoglRenderPlanMasked *renderPlanMask );
	
	/**
	 * Render.
	 * Resets paint dirty flag. Paint tracker is kept unchanged.
	 */
	virtual void Render( const deoglRenderCanvasContext &context );
	/*@}*/
};

#endif
