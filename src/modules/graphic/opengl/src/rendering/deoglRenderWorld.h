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

#ifndef _DEOGLRENDERWORLD_H_
#define _DEOGLRENDERWORLD_H_

#include "deoglRenderBase.h"
#include "deoglRenderWorldInfo.h"
#include "../shaders/deoglShaderProgramUsage.h"

class deoglAddToRenderTask;
class deoglAddToRenderTaskParticles;
class deoglParticleSorter;
class deoglRenderTask;
class deoglRenderPlanMasked;
class deoglRenderTaskParticles;
class deoglSPBlockUBO;



/**
 * World renderer.
 */
class deoglRenderWorld : public deoglRenderBase{
private:
	deoglSPBlockUBO *pRenderPB;
	deoglSPBlockUBO *pRenderLuminancePB;
	deoglSPBlockUBO *pRenderCubePB;
	deoglRenderTask *pRenderTask;
	deoglAddToRenderTask *pAddToRenderTask;
	deoglParticleSorter *pParticleSorter;
	deoglRenderTaskParticles *pRenderTaskParticles;
	deoglAddToRenderTaskParticles *pAddToRenderTaskParticles;
	
	deoglShaderProgramUsage pShaderFinalize;
	
	deoglRenderWorldInfo pDebugInfo;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderer. */
	deoglRenderWorld( deoglRenderThread &renderThread );
	
	/** Clean up renderer. */
	virtual ~deoglRenderWorld();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/** Debug info. */
	inline deoglRenderWorldInfo &GetDebugInfo(){ return pDebugInfo; }
	
	/** Render parameter block. */
	inline deoglSPBlockUBO *GetRenderPB() const{ return pRenderPB; }
	
	/** Render luminance parameter block. */
	inline deoglSPBlockUBO *GetRenderLuminancePB() const{ return pRenderLuminancePB; }
	
	/** Render parameter block cube map. */
	inline deoglSPBlockUBO *GetRenderCubePB() const{ return pRenderCubePB; }
	
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
	void RenderBlackScreen( deoglRenderPlan &plan );
	
	/** Render world. */
	void RenderWorld( deoglRenderPlan &plan, deoglRenderPlanMasked *mask );
	
	/** Prepare render parameter shader parameter block. */
	void PrepareRenderParamBlock( deoglRenderPlan &plan, deoglRenderPlanMasked *mask );
	
	/** Render masked pass. */
	void RenderMaskedPass( deoglRenderPlan &plan );
	
	/** Render debug drawers. */
	void RenderDebugDrawers( deoglRenderPlan &plan );
	
	/** Render effects. */
	void RenderEffects( deoglRenderPlan &plan );
	
	/** Render anti aliasing pass. */
	void RenderAntiAliasingPass();
	
	/** Render finalize pass to an FBO without color correction. */
	void RenderFinalizeFBO( deoglRenderPlan &plan );
	
	/** Render finalize pass to active graphics context with color correction. */
	void RenderFinalizeContext( deoglRenderPlan &plan );
	
	
	
	/** Add top level debug information in the right order. */
	virtual void AddTopLevelDebugInfo();
	
	/** Developer mode debug information changed. */
	virtual void DevModeDebugInfoChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
