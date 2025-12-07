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

#ifndef _DEOGLRENDERTRANSPARENTPASSES_H_
#define _DEOGLRENDERTRANSPARENTPASSES_H_

#include "deoglRenderBase.h"

class deoglRenderPlan;
class deoglRenderPlanMasked;


/**
 * Transparent passes renderer.
 */
class deoglRenderTransparentPasses : public deoglRenderBase{
private:
	const deoglPipeline *pPipelineCopyDepthLimit;
	const deoglPipeline *pPipelineCopyDepthLimitStereo;
	const deoglPipeline *pPipelineCopyDepthColor;
	const deoglPipeline *pPipelineCopyDepthColorStereo;
	const deoglPipeline *pPipelineCopyColor;
	const deoglPipeline *pPipelineCopyColorStereo;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderer. */
	deoglRenderTransparentPasses(deoglRenderThread &renderThread);
	
	/** Clean up renderer. */
	virtual ~deoglRenderTransparentPasses();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/** Render Transparent Passes. */
	void RenderTransparentPasses(deoglRenderPlan &plan, const deoglRenderPlanMasked *mask, bool xray);
	
	/** Render transparent geometry pass. */
	void RenderTransparentGeometryPass(deoglRenderPlan &plan, const deoglRenderPlanMasked *mask, bool xray);
	
	/** Render transparent depth limite pass. */
	void RenderTransparentLimitDepth(deoglRenderPlan &plan, const deoglRenderPlanMasked *mask, bool xray);
	
	/** Render volumetric pass. */
	void RenderVolumetricPass(deoglRenderPlan &plan, const deoglRenderPlanMasked *mask, bool inbetween);
	
	/** Copy color texture to temporary texture. */
	void CopyColorToTemporary(deoglRenderPlan &plan);
	/*@}*/
};

#endif
