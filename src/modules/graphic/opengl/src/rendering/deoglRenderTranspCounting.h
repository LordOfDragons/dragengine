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

#ifndef _DEOGLRENDERTRANSPCOUNTING_H_
#define _DEOGLRENDERTRANSPCOUNTING_H_

#include "deoglRenderBase.h"

class deoglRenderPlan;
class deoglRenderPlanMasked;
class deoglOcclusionQuery;


/**
 * Transparency counting renderer.
 */
class deoglRenderTranspCounting : public deoglRenderBase{
private:
	deoglOcclusionQuery *pOccQuery;
	
	const deoglPipeline *pPipelineTraCountMaxCount;
	const deoglPipeline *pPipelineTraCountMaxCountStereo;
	
	const deoglPipeline *pPipelineTraCountGetCount;
	const deoglPipeline *pPipelineTraCountGetCountStereo;
	
	bool pHasCount;
	int pCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderer. */
	deoglRenderTranspCounting( deoglRenderThread &renderThread );
	
	/** Clean up renderer. */
	virtual ~deoglRenderTranspCounting();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/**
	 * Count transparency.
	 * 
	 * Using FBO Def-Ren Diffuse+Depth. Clears diffuse buffer.
	 * - render transparent geometry writing 1/255 for each fragment
	 * Invalidates no attachments
	 * 
	 * Using ping-pong between FBO Def-Ren Diffuse and Reflectivity. Clear respective
	 * texture each turn. Invalidates no attachments each turn.
	 * 
	 * Using last used Diffuse/Reflectivity buffer. Clears nothing. Invalidates buffer
	 * since result is written to render plan.
	 */
	void CountTransparency( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask );
	
	/**
	 * Last found transparency count.
	 * 
	 * Count is cached until the next time CountTransparency() is called.
	 */
	int GetCount();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
