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

#ifndef _DEOGLRENDERWORLD_H_
#define _DEOGLRENDERWORLD_H_

#include "deoglRenderBase.h"
#include "deoglRenderWorldInfo.h"
#include "../pipeline/deoglPipeline.h"

class deoglAddToRenderTask;
class deoglAddToRenderTaskParticles;
class deoglParticleSorter;
class deoglRenderTask;
class deoglRenderPlanMasked;
class deoglRenderTaskParticles;



/**
 * World renderer.
 */
class deoglRenderWorld : public deoglRenderBase{
private:
	deoglSPBSingleUse::Ref pRenderPBSingleUse;
	deoglSPBlockUBO::Ref pRenderPB;
	deoglRenderTask *pRenderTask;
	deoglAddToRenderTask *pAddToRenderTask;
	deoglParticleSorter *pParticleSorter;
	deoglRenderTaskParticles *pRenderTaskParticles;
	deoglAddToRenderTaskParticles *pAddToRenderTaskParticles;
	
	const deoglPipeline *pPipelineFinalize;
	const deoglPipeline *pPipelineFinalizeStereo;
	const deoglPipeline *pPipelineFinalizeBlend;
	const deoglPipeline *pPipelineFinalizeBlendStereo;
	const deoglPipeline *pPipelineFinalizeSplit;
	
	deoglRenderWorldInfo pDebugInfo;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderer. */
	deoglRenderWorld(deoglRenderThread &renderThread);
	
	/** Clean up renderer. */
	virtual ~deoglRenderWorld();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/** Debug info. */
	inline deoglRenderWorldInfo &GetDebugInfo(){ return pDebugInfo; }
	
	/** Render parameter block. */
	inline const deoglSPBlockUBO::Ref &GetRenderPB() const{ return pRenderPB; }
	
	/** Render task. */
	inline deoglRenderTask *GetRenderTask() const{ return pRenderTask; }
	
	/** Render task for particles. */
	inline deoglRenderTaskParticles *GetRenderTaskParticles() const{ return pRenderTaskParticles; }
	
	/** Add to render task. */
	inline deoglAddToRenderTask *GetAddToRenderTask() const{ return pAddToRenderTask; }
	
	/** Add to render task particles. */
	inline deoglAddToRenderTaskParticles *GetAddToRenderTaskParticles() const{ return pAddToRenderTaskParticles; }
	
	/** Particle sorter. */
	inline deoglParticleSorter *GetParticleSorter() const{ return pParticleSorter; }
	
	
	
	/** Render black screen. */
	void RenderBlackScreen(deoglRenderPlan &plan);
	
	/** Render world. */
	void RenderWorld(deoglRenderPlan &plan, const deoglRenderPlanMasked *mask);
	
	/** Prepare render parameter shader parameter block. */
	void PrepareRenderParamBlock(deoglRenderPlan &plan, const deoglRenderPlanMasked *mask);
	
	/** Render masked pass. */
	void RenderMaskedPass(deoglRenderPlan &plan);
	
	/** Render debug drawers. */
	void RenderDebugDrawers(deoglRenderPlan &plan);
	
	/** Render effects. */
	void RenderEffects(deoglRenderPlan &plan);
	
	/** Render anti aliasing pass. */
	void RenderAntiAliasingPass();
	
	/** Render finalize pass to an FBO without color correction. */
	void RenderFinalizeFBO(deoglRenderPlan &plan, bool withColorCorrection, bool withGammaCorrection);
	
	/** Render finalize pass to active graphics context with color correction. */
	void RenderFinalizeContext(deoglRenderPlan &plan);
	
	
	
	/** Add top level debug information in the right order. */
	virtual void AddTopLevelDebugInfo();
	
	/** Developer mode debug information changed. */
	virtual void DevModeDebugInfoChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
