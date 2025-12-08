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

#ifndef _DEOGLRCANVASRENDERWORLD_H_
#define _DEOGLRCANVASRENDERWORLD_H_

#include "deoglRCanvas.h"

class deoglRWorld;
#include "../../world/deoglRCamera.h"


/**
 * Render canvas render world.
 */
class deoglRCanvasRenderWorld : public deoglRCanvas{
private:
	deoglRCamera::Ref pCamera;
	bool pForceToneMapAdaption;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglRCanvasRenderWorld(deoglRenderThread &renderThread);
	
	/** Clean up peer. */
	~deoglRCanvasRenderWorld() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Camera or \em NULL if not set. */
	inline const deoglRCamera::Ref &GetCamera() const{ return pCamera; }
	
	/** Set camera or \em NULL if not set. */
	void SetCamera(deoglRCamera *camera);
	
	
	
	/** Prepare for rendering. */
	void PrepareForRender(const deoglRenderPlanMasked *renderPlanMask) override;
	
	/** Render. */
	void Render(const deoglRenderCanvasContext &context) override;
	/*@}*/
};

#endif
