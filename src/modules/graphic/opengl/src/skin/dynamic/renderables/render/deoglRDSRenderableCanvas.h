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

#ifndef _DEOGLRRDSRENDERABLECANVAS_H_
#define _DEOGLRRDSRENDERABLECANVAS_H_

#include <dragengine/deObject.h>

#include "deoglRDSRenderable.h"

class deoglRCanvasView;
class deDSRenderableCanvas;



/**
 * Render dynamic skin canvas renderable.
 */
class deoglRDSRenderableCanvas : public deoglRDSRenderable{
private:
	deoglRCanvasView::Ref pCanvas;
	int pComponentCount;
	int pBitCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render dynamic skin canvas renderable. */
	deoglRDSRenderableCanvas(deoglRDynamicSkin &dynamicSkin, const deDSRenderableCanvas &renderable);
	
	/** Clean up render dynamic skin canvas  renderable. */
	~deoglRDSRenderableCanvas() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Set canvas. */
	void SetCanvas(deoglRCanvasView *canvas);
	
	/** Set component count. */
	void SetComponentCount(int componentCount);
	
	/** Set bit count. */
	void SetBitCount(int bitCount);
	
	/** Prepare for render. */
	void PrepareForRender(const deoglRenderPlanMasked *renderPlanMask) override;
	
	/** Render. */
	void Render(const deoglRenderPlanMasked *renderPlanMask) override;
	
	/**
	 * Get texture to use for rendering or \em NULL if not applicable.
	 * \details Default implementation returns \em NULL.
	 */
	deoglTexture *GetRenderTexture() override;
	/*@}*/
};

#endif
